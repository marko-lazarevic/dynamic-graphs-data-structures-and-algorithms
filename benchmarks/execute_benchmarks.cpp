#include <algorithm>
#include <chrono>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "benchmarks.hpp"
#include "../src/algorithms/connectivity/dynamic_connectivity.hpp"
#include "../src/algorithms/mst/dynamic_mst_incremental.hpp"
#include "../src/algorithms/suitor/suitor_matching.hpp"

namespace {

using Clock = std::chrono::high_resolution_clock;

struct ConnectivityTimings {
    double add_edge_ns = 0.0;
    double is_connected_ns = 0.0;
};

struct MSTTimings {
    double add_edge_ns = 0.0;
    double is_in_mst_ns = 0.0;
};

struct SuitorTimings {
    double add_edge_ns = 0.0;
    double is_matched_ns = 0.0;
};

// ============================================================================
// Connectivity Benchmarking
// ============================================================================

ConnectivityTimings BenchmarkConnectivitySingle(int vertex_count, int edge_count, int query_count, unsigned int seed) {
    dynamic_connectivity::DynamicConnectivityIncremental dc;

    for (int i = 0; i < vertex_count; ++i) {
        dc.AddVertex();
    }

    std::mt19937 rng(seed);
    std::uniform_int_distribution<dg::VertexID> vertex_dist(0, static_cast<dg::VertexID>(vertex_count - 1));

    std::vector<std::pair<dg::VertexID, dg::VertexID>> edges;
    edges.reserve(static_cast<std::size_t>(edge_count));
    for (int i = 0; i < edge_count; ++i) {
        edges.emplace_back(vertex_dist(rng), vertex_dist(rng));
    }

    std::vector<std::pair<dg::VertexID, dg::VertexID>> queries;
    queries.reserve(static_cast<std::size_t>(query_count));
    for (int i = 0; i < query_count; ++i) {
        queries.emplace_back(vertex_dist(rng), vertex_dist(rng));
    }

    auto add_start = Clock::now();
    for (const auto& [u, v] : edges) {
        dc.AddEdge(u, v);
    }
    auto add_end = Clock::now();

    volatile std::size_t checksum = 0;
    auto query_start = Clock::now();
    for (const auto& [u, v] : queries) {
        checksum += dc.IsConnected(u, v) ? 1u : 0u;
    }
    auto query_end = Clock::now();

    (void)checksum;

    const auto add_total_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(add_end - add_start).count();
    const auto query_total_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(query_end - query_start).count();

    ConnectivityTimings timings;
    timings.add_edge_ns = static_cast<double>(add_total_ns) / static_cast<double>(edge_count);
    timings.is_connected_ns = static_cast<double>(query_total_ns) / static_cast<double>(query_count);
    return timings;
}

void RunConnectivityBenchmark(int max_vertices, const std::string& output_file) {
    std::cout << "\n=== Dynamic Connectivity Benchmark ===" << std::endl;

    std::ofstream csv(output_file);
    if (!csv.is_open()) {
        std::cerr << "Failed to open " << output_file << " for writing.\n";
        return;
    }

    csv << "vertices,edges,avg_add_edge_ns,avg_is_connected_ns\n";

    constexpr int runs = 5;
    constexpr int query_count = 10000;

    for (int vertices = BATCH_SIZE; vertices <= max_vertices; vertices += BATCH_SIZE) {
        const int edges = std::min(MAX_EDGES, 3 * vertices);

        std::cout << "  vertices=" << vertices << ", edges=" << edges << "... ";
        std::cout.flush();

        double mean_add_edge_ns = 0.0;
        double mean_is_connected_ns = 0.0;

        for (int run = 0; run < runs; ++run) {
            const unsigned int seed = static_cast<unsigned int>(vertices * 1315423911u + run * 2654435761u);
            const ConnectivityTimings timings = BenchmarkConnectivitySingle(vertices, edges, query_count, seed);
            mean_add_edge_ns += timings.add_edge_ns;
            mean_is_connected_ns += timings.is_connected_ns;
        }

        mean_add_edge_ns /= static_cast<double>(runs);
        mean_is_connected_ns /= static_cast<double>(runs);

        csv << vertices << ','
            << edges << ','
            << std::fixed << std::setprecision(2) << mean_add_edge_ns << ','
            << mean_is_connected_ns << '\n';

        std::cout << "AddEdge: " << mean_add_edge_ns << " ns, IsConnected: "
                  << mean_is_connected_ns << " ns" << std::endl;
    }

    csv.close();
    std::cout << "Results written to " << output_file << std::endl;
}

// ============================================================================
// MST Benchmarking
// ============================================================================

MSTTimings BenchmarkMSTSingle(int vertex_count, int edge_count, int query_count, unsigned int seed) {
    dynamic_mst::DynamicMSTIncremental mst;

    for (int i = 0; i < vertex_count; ++i) {
        mst.AddVertex();
    }

    std::mt19937 rng(seed);
    std::uniform_int_distribution<dg::VertexID> vertex_dist(0, static_cast<dg::VertexID>(vertex_count - 1));
    std::uniform_real_distribution<double> weight_dist(1.0, 100.0);

    std::vector<std::tuple<dg::VertexID, dg::VertexID, dg::Weight>> edges;
    edges.reserve(static_cast<std::size_t>(edge_count));
    for (int i = 0; i < edge_count; ++i) {
        const auto u = vertex_dist(rng);
        const auto v = vertex_dist(rng);
        const auto w = static_cast<dg::Weight>(weight_dist(rng));
        edges.emplace_back(u, v, w);
    }

    std::vector<std::pair<dg::VertexID, dg::VertexID>> queries;
    queries.reserve(static_cast<std::size_t>(query_count));
    for (int i = 0; i < query_count; ++i) {
        queries.emplace_back(vertex_dist(rng), vertex_dist(rng));
    }

    auto add_start = Clock::now();
    for (const auto& edge : edges) {
        mst.AddEdge(std::get<0>(edge), std::get<1>(edge), std::get<2>(edge));
    }
    auto add_end = Clock::now();

    volatile std::size_t checksum = 0;
    auto query_start = Clock::now();
    for (const auto& [u, v] : queries) {
        checksum += mst.IsInMST(u, v) ? 1u : 0u;
    }
    auto query_end = Clock::now();

    (void)checksum;

    const auto add_total_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(add_end - add_start).count();
    const auto query_total_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(query_end - query_start).count();

    MSTTimings timings;
    timings.add_edge_ns = static_cast<double>(add_total_ns) / static_cast<double>(edge_count);
    timings.is_in_mst_ns = static_cast<double>(query_total_ns) / static_cast<double>(query_count);
    return timings;
}

void RunMSTBenchmark(int max_vertices, const std::string& output_file) {
    std::cout << "\n=== Dynamic MST Benchmark ===" << std::endl;

    std::ofstream csv(output_file);
    if (!csv.is_open()) {
        std::cerr << "Failed to open " << output_file << " for writing.\n";
        return;
    }

    csv << "vertices,edges,avg_add_edge_ns,avg_is_in_mst_ns\n";

    constexpr int runs = 5;
    constexpr int query_count = 10000;

    for (int vertices = BATCH_SIZE; vertices <= max_vertices; vertices += BATCH_SIZE) {
        const int edges = std::min(MAX_EDGES, 3 * vertices);

        std::cout << "  vertices=" << vertices << ", edges=" << edges << "... ";
        std::cout.flush();

        double mean_add_edge_ns = 0.0;
        double mean_is_in_mst_ns = 0.0;

        for (int run = 0; run < runs; ++run) {
            const unsigned int seed = static_cast<unsigned int>(vertices * 1315423911u + run * 2654435761u);
            const MSTTimings timings = BenchmarkMSTSingle(vertices, edges, query_count, seed);
            mean_add_edge_ns += timings.add_edge_ns;
            mean_is_in_mst_ns += timings.is_in_mst_ns;
        }

        mean_add_edge_ns /= static_cast<double>(runs);
        mean_is_in_mst_ns /= static_cast<double>(runs);

        csv << vertices << ','
            << edges << ','
            << std::fixed << std::setprecision(2) << mean_add_edge_ns << ','
            << mean_is_in_mst_ns << '\n';

        std::cout << "AddEdge: " << mean_add_edge_ns << " ns, IsInMST: "
                  << mean_is_in_mst_ns << " ns" << std::endl;
    }

    csv.close();
    std::cout << "Results written to " << output_file << std::endl;
}

// ============================================================================
// Suitor Matching Benchmarking
// ============================================================================

SuitorTimings BenchmarkSuitorSingle(int vertex_count, int edge_count, int query_count, unsigned int seed) {
    matching::SuitorMatching suitor;

    for (int i = 0; i < vertex_count; ++i) {
        suitor.AddVertex();
    }

    std::mt19937 rng(seed);
    std::uniform_int_distribution<dg::VertexID> vertex_dist(0, static_cast<dg::VertexID>(vertex_count - 1));
    std::uniform_real_distribution<double> weight_dist(1.0, 100.0);

    std::vector<std::tuple<dg::VertexID, dg::VertexID, dg::Weight>> edges;
    edges.reserve(static_cast<std::size_t>(edge_count));
    for (int i = 0; i < edge_count; ++i) {
        const auto u = vertex_dist(rng);
        const auto v = vertex_dist(rng);
        const auto w = static_cast<dg::Weight>(weight_dist(rng));
        edges.emplace_back(u, v, w);
    }

    std::vector<dg::VertexID> query_vertices;
    query_vertices.reserve(static_cast<std::size_t>(query_count));
    for (int i = 0; i < query_count; ++i) {
        query_vertices.emplace_back(vertex_dist(rng));
    }

    auto add_start = Clock::now();
    for (const auto& edge : edges) {
        suitor.AddEdge(std::get<0>(edge), std::get<1>(edge), std::get<2>(edge));
    }
    auto add_end = Clock::now();

    volatile std::size_t checksum = 0;
    auto query_start = Clock::now();
    for (const auto& u : query_vertices) {
        checksum += suitor.IsMatched(u) ? 1u : 0u;
    }
    auto query_end = Clock::now();

    (void)checksum;

    const auto add_total_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(add_end - add_start).count();
    const auto query_total_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(query_end - query_start).count();

    SuitorTimings timings;
    timings.add_edge_ns = static_cast<double>(add_total_ns) / static_cast<double>(edge_count);
    timings.is_matched_ns = static_cast<double>(query_total_ns) / static_cast<double>(query_count);
    return timings;
}

void RunSuitorBenchmark(int max_vertices, const std::string& output_file) {
    std::cout << "\n=== Suitor Matching Benchmark ===" << std::endl;

    std::ofstream csv(output_file);
    if (!csv.is_open()) {
        std::cerr << "Failed to open " << output_file << " for writing.\n";
        return;
    }

    csv << "vertices,edges,avg_add_edge_ns,avg_is_matched_ns\n";

    constexpr int runs = 5;
    constexpr int query_count = 10000;

    for (int vertices = BATCH_SIZE; vertices <= max_vertices; vertices += BATCH_SIZE) {
        const int edges = std::min(MAX_EDGES, 3 * vertices);

        std::cout << "  vertices=" << vertices << ", edges=" << edges << "... ";
        std::cout.flush();

        double mean_add_edge_ns = 0.0;
        double mean_is_matched_ns = 0.0;

        for (int run = 0; run < runs; ++run) {
            const unsigned int seed = static_cast<unsigned int>(vertices * 1315423911u + run * 2654435761u);
            const SuitorTimings timings = BenchmarkSuitorSingle(vertices, edges, query_count, seed);
            mean_add_edge_ns += timings.add_edge_ns;
            mean_is_matched_ns += timings.is_matched_ns;
        }

        mean_add_edge_ns /= static_cast<double>(runs);
        mean_is_matched_ns /= static_cast<double>(runs);

        csv << vertices << ','
            << edges << ','
            << std::fixed << std::setprecision(2) << mean_add_edge_ns << ','
            << mean_is_matched_ns << '\n';

        std::cout << "AddEdge: " << mean_add_edge_ns << " ns, IsMatched: "
                  << mean_is_matched_ns << " ns" << std::endl;
    }

    csv.close();
    std::cout << "Results written to " << output_file << std::endl;
}

// ============================================================================
// Utilities
// ============================================================================

void PrintUsage(const char* program_name) {
    std::cout << "Usage: " << program_name << " <algorithm> <max_vertices> [output_filename]\n"
              << "\nSupported algorithms: connectivity, mst, suitor\n"
              << "max_vertices: Maximum number of vertices to test (will step by " << BATCH_SIZE << ")\n"
              << "output_filename: Optional. Defaults to <algorithm>_bench.csv\n"
              << "\nExample:\n"
              << "  " << program_name << " connectivity 5000\n"
              << "  " << program_name << " mst 10000 my_mst_results.csv\n"
              << std::endl;
}

} // namespace

int main(int argc, char* argv[]) {
    if (argc < 3 || argc > 4) {
        PrintUsage(argv[0]);
        return 1;
    }

    const std::string algorithm = argv[1];
    int max_vertices = 0;

    try {
        max_vertices = std::stoi(argv[2]);
    } catch (const std::exception& e) {
        std::cerr << "Error: max_vertices must be a valid integer.\n";
        PrintUsage(argv[0]);
        return 1;
    }

    if (max_vertices <= 0 || max_vertices < BATCH_SIZE) {
        std::cerr << "Error: max_vertices must be positive and at least " << BATCH_SIZE << ".\n";
        return 1;
    }

    std::string output_file;
    if (argc == 4) {
        output_file = argv[3];
    } else {
        output_file = algorithm + "_bench.csv";
    }

    std::cout << "Graph Algorithm Benchmark Suite\n"
              << "Algorithm: " << algorithm << "\n"
              << "Max vertices: " << max_vertices << "\n"
              << "Output file: " << output_file << std::endl;

    if (algorithm == "connectivity") {
        RunConnectivityBenchmark(max_vertices, output_file);
    } else if (algorithm == "mst") {
        RunMSTBenchmark(max_vertices, output_file);
    } else if (algorithm == "suitor") {
        RunSuitorBenchmark(max_vertices, output_file);
    } else {
        std::cerr << "Error: Unknown algorithm '" << algorithm << "'.\n";
        PrintUsage(argv[0]);
        return 1;
    }

    std::cout << "\nBenchmark complete." << std::endl;
    return 0;
}