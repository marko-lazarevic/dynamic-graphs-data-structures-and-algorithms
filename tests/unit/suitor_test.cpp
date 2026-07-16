#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include "../../src/algorithms/suitor/suitor_matching.hpp"

TEST_CASE("Initialization and empty graph") {
    matching::SuitorMatching matching;

    REQUIRE(matching.AddVertex() == 0);
    REQUIRE(matching.AddVertex() == 1);
    REQUIRE(matching.AddVertex() == 2);

    REQUIRE(matching.GetMatchingEdges().empty());
    REQUIRE(matching.GetTotalMatchingWeight() == 0);
}

TEST_CASE("Two vertices with edge creates valid matching") {
    matching::SuitorMatching matching;

    matching.AddVertex();
    matching.AddVertex();
    matching.AddEdge(0, 1, 5);

    const auto& edges = matching.GetMatchingEdges();
    REQUIRE(edges.size() == 1);
    REQUIRE(matching.GetTotalMatchingWeight() == 5);

    REQUIRE(matching.IsMatched(0));
    REQUIRE(matching.IsMatched(1));
    REQUIRE(matching.GetPartner(0) == 1);
    REQUIRE(matching.GetPartner(1) == 0);
    REQUIRE(matching.GetPartner(2) == -1);  // Non-existent vertex
}

TEST_CASE("Maximum weight matching selection") {
    matching::SuitorMatching matching;

    for (int i = 0; i < 3; ++i) {
        matching.AddVertex();
    }

    matching.AddEdge(0, 1, 10);
    matching.AddEdge(1, 2, 8);
    matching.AddEdge(0, 2, 6);

    REQUIRE(matching.GetTotalMatchingWeight() == 10);
    REQUIRE(matching.IsMatched(0));
    REQUIRE(matching.IsMatched(1));
    REQUIRE_FALSE(matching.IsMatched(2));
}

TEST_CASE("Star graph with central vertex") {
    matching::SuitorMatching matching;

    for (int i = 0; i < 5; ++i) {
        matching.AddVertex();
    }

    // Vertex 0 connected to all others
    matching.AddEdge(0, 1, 5);
    matching.AddEdge(0, 2, 10);
    matching.AddEdge(0, 3, 8);
    matching.AddEdge(0, 4, 3);

    REQUIRE(matching.GetMatchingEdges().size() == 1);
    REQUIRE(matching.GetTotalMatchingWeight() == 10);
    REQUIRE(matching.IsMatched(0));
    REQUIRE(matching.IsMatched(2));
}

TEST_CASE("Triangle with one heavy edge") {
    matching::SuitorMatching matching;

    for (int i = 0; i < 3; ++i) {
        matching.AddVertex();
    }

    matching.AddEdge(0, 1, 100);
    matching.AddEdge(1, 2, 1);
    matching.AddEdge(0, 2, 1);

    REQUIRE(matching.GetTotalMatchingWeight() == 100);
    REQUIRE(matching.IsMatched(0));
    REQUIRE(matching.IsMatched(1));
    REQUIRE_FALSE(matching.IsMatched(2));
}

TEST_CASE("Adding new edge updates matching and reconfigures when beneficial") {
    matching::SuitorMatching matching;

    for (int i = 0; i < 4; ++i) {
        matching.AddVertex();
    }

    // Initial matching: 0-1 and 2-3
    matching.AddEdge(0, 1, 5);
    matching.AddEdge(2, 3, 10);
    REQUIRE(matching.GetMatchingEdges().size() == 2);
    REQUIRE(matching.GetTotalMatchingWeight() == 15);

    // Add higher weight edge that connects previous components
    matching.AddEdge(1, 2, 20);

    // Matching should reconfigure to include the new high-weight edge
    REQUIRE(matching.IsMatched(1));
    REQUIRE(matching.IsMatched(2));
    
    // Add one more edge incrementally
    matching.AddEdge(0, 3, 8);
    
    // Verify matching is always valid
    auto edges = matching.GetMatchingEdges();
    for (const auto& edge : edges) {
        REQUIRE(matching.IsMatched(edge.first));
        REQUIRE(matching.IsMatched(edge.second));
    }
}

TEST_CASE("Updating edge weight recalculates matching") {
    matching::SuitorMatching matching;

    for (int i = 0; i < 4; ++i) {
        matching.AddVertex();
    }

    matching.AddEdge(0, 1, 5);
    matching.AddEdge(1, 2, 3);
    matching.AddEdge(2, 3, 10);

    auto initial_weight = matching.GetTotalMatchingWeight();
    REQUIRE(matching.IsMatched(0));
    REQUIRE(matching.IsMatched(1));
    REQUIRE(matching.IsMatched(2));
    REQUIRE(matching.IsMatched(3));

    // Lower edge weight triggers recalculation
    matching.UpdateEdgeWeight(0, 1, 1);

    // Verify matching is still valid after update
    std::vector<int> vertex_usage(4, 0);
    for (const auto& edge : matching.GetMatchingEdges()) {
        vertex_usage[edge.first]++;
        vertex_usage[edge.second]++;
        REQUIRE(vertex_usage[edge.first] <= 1);
        REQUIRE(vertex_usage[edge.second] <= 1);
    }
}

TEST_CASE("Removing an edge recalculates matching and keeps it valid") {
    matching::SuitorMatching matching;

    for (int i = 0; i < 4; ++i) {
        matching.AddVertex();
    }

    matching.AddEdge(0, 1, 7);
    matching.AddEdge(2, 3, 9);
    REQUIRE(matching.GetMatchingEdges().size() == 2);
    REQUIRE(matching.GetTotalMatchingWeight() == 16);

    matching.RemoveEdge(0, 1);

    REQUIRE_FALSE(matching.IsMatched(0));
    REQUIRE_FALSE(matching.IsMatched(1));
    REQUIRE(matching.IsMatched(2));
    REQUIRE(matching.IsMatched(3));

    const auto& edges = matching.GetMatchingEdges();
    REQUIRE(edges.size() == 1);
    REQUIRE(matching.GetTotalMatchingWeight() == 9);

    std::vector<int> vertex_usage(4, 0);
    for (const auto& edge : edges) {
        vertex_usage[edge.first]++;
        vertex_usage[edge.second]++;
        REQUIRE(vertex_usage[edge.first] <= 1);
        REQUIRE(vertex_usage[edge.second] <= 1);
    }
}

TEST_CASE("Removing a vertex recalculates matching and remaps ids correctly") {
    matching::SuitorMatching matching;

    for (int i = 0; i < 5; ++i) {
        matching.AddVertex();
    }

    matching.AddEdge(0, 1, 4);
    matching.AddEdge(2, 3, 11);
    matching.AddEdge(3, 4, 6);

    REQUIRE(matching.GetMatchingEdges().size() == 2);

    matching.RemoveVertex(2);

    // Vertex ids after removal are remapped: old 3 -> 2, old 4 -> 3
    REQUIRE(matching.IsMatched(0));
    REQUIRE(matching.IsMatched(1));
    REQUIRE(matching.IsMatched(2));
    REQUIRE(matching.IsMatched(3));
    REQUIRE(matching.GetPartner(0) == 1);
    REQUIRE(matching.GetPartner(1) == 0);
    REQUIRE(matching.GetPartner(2) == 3);
    REQUIRE(matching.GetPartner(3) == 2);

    const auto& edges = matching.GetMatchingEdges();
    REQUIRE(edges.size() == 2);
    REQUIRE(matching.GetTotalMatchingWeight() == 10);

    std::vector<int> vertex_usage(4, 0);
    for (const auto& edge : edges) {
        REQUIRE(edge.first >= 0);
        REQUIRE(edge.second >= 0);
        REQUIRE(edge.first < 4);
        REQUIRE(edge.second < 4);
        vertex_usage[edge.first]++;
        vertex_usage[edge.second]++;
        REQUIRE(vertex_usage[edge.first] <= 1);
        REQUIRE(vertex_usage[edge.second] <= 1);
    }
}

TEST_CASE("Disconnected components handled correctly") {
    matching::SuitorMatching matching;

    for (int i = 0; i < 6; ++i) {
        matching.AddVertex();
    }

    // Component 1: vertices 0, 1, 2
    matching.AddEdge(0, 1, 10);
    matching.AddEdge(1, 2, 5);

    // Component 2: vertices 3, 4, 5
    matching.AddEdge(3, 4, 8);
    matching.AddEdge(4, 5, 6);

    auto edges = matching.GetMatchingEdges();
    REQUIRE(edges.size() == 2);

    // Verify validity
    std::vector<int> vertex_count(6, 0);
    for (const auto& edge : edges) {
        vertex_count[edge.first]++;
        vertex_count[edge.second]++;
    }

    for (int count : vertex_count) {
        REQUIRE(count <= 1);
    }
}

TEST_CASE("Multiple sequential edge additions maintain valid matching") {
    matching::SuitorMatching matching;

    for (int i = 0; i < 6; ++i) {
        matching.AddVertex();
    }

    // Add edges sequentially and verify validity at each step
    matching.AddEdge(0, 1, 5);
    REQUIRE(matching.GetMatchingEdges().size() == 1);
    REQUIRE(matching.GetTotalMatchingWeight() == 5);

    matching.AddEdge(2, 3, 8);
    REQUIRE(matching.GetMatchingEdges().size() == 2);
    REQUIRE(matching.GetTotalMatchingWeight() == 13);

    matching.AddEdge(4, 5, 12);
    REQUIRE(matching.GetMatchingEdges().size() == 3);
    REQUIRE(matching.GetTotalMatchingWeight() == 25);

    // Add higher weight edge that may reconfigure
    matching.AddEdge(1, 2, 20);
    
    // Verify final matching is valid
    auto edges = matching.GetMatchingEdges();
    std::vector<bool> matched(6, false);
    for (const auto& edge : edges) {
        REQUIRE_FALSE(matched[edge.first]);
        REQUIRE_FALSE(matched[edge.second]);
        matched[edge.first] = true;
        matched[edge.second] = true;
    }
}

TEST_CASE("Multiple edge weight updates preserve matching invariant") {
    matching::SuitorMatching matching;

    for (int i = 0; i < 4; ++i) {
        matching.AddVertex();
    }

    matching.AddEdge(0, 1, 5);
    matching.AddEdge(2, 3, 10);

    // Perform multiple updates and verify validity
    for (int iter = 0; iter < 3; ++iter) {
        matching.UpdateEdgeWeight(0, 1, 5 + iter);
        
        std::vector<int> vertex_usage(4, 0);
        for (const auto& edge : matching.GetMatchingEdges()) {
            vertex_usage[edge.first]++;
            vertex_usage[edge.second]++;
            REQUIRE(vertex_usage[edge.first] <= 1);
            REQUIRE(vertex_usage[edge.second] <= 1);
        }
    }

    // No vertex should appear in matching more than once
    for (int v = 0; v < 4; ++v) {
        auto partner = matching.GetPartner(v);
        if (partner != -1) {
            // If matched, verify reciprocal relationship
            REQUIRE(matching.GetPartner(partner) == v);
        }
    }
}
