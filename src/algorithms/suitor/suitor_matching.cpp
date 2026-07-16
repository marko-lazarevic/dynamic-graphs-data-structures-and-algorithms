#include "suitor_matching.hpp"
#include <iterator>
#include <algorithm>

namespace matching {

    dg::VertexID SuitorMatching::AddVertex() {
        return graph.AddVertex();
    }

    void SuitorMatching::AddEdge(dg::VertexID vertex_id1,
                                dg::VertexID vertex_id2,
                                dg::Weight weight)
    {
        graph.AddEdge(vertex_id1, vertex_id2, weight);
        graph.AddEdge(vertex_id2, vertex_id1, weight);
        rebuildMatching();
    }

    void SuitorMatching::RemoveEdge(dg::VertexID vertex_id1,
                                    dg::VertexID vertex_id2)
    {
        graph.RemoveEdge(vertex_id1, vertex_id2);
        graph.RemoveEdge(vertex_id2, vertex_id1);
        rebuildMatching();
    }

    void SuitorMatching::RemoveVertex(dg::VertexID vertex_id)
    {
        graph.RemoveVertex(vertex_id);
        rebuildMatching();
    }

    void SuitorMatching::UpdateEdgeWeight(dg::VertexID vertex_id1,
                                        dg::VertexID vertex_id2,
                                        dg::Weight new_weight)
    {
        graph.UpdateEdgeWeight(vertex_id1, vertex_id2, new_weight);
        graph.UpdateEdgeWeight(vertex_id2, vertex_id1, new_weight);
        rebuildMatching();
    }

    void SuitorMatching::rebuildMatching() {
        int n = graph.VertexCount();
        matching_edges.clear();
        total_matching_weight = 0;

        if (n == 0) return;

        suitor.assign(n, -1);
        suitor_weight.assign(n, 0);

        std::vector<dg::VertexID> active_vertices;
        active_vertices.reserve(n);
        for (int i = 0; i < n; ++i) {
            active_vertices.push_back(i);
        }

        while (!active_vertices.empty()) {
            dg::VertexID vertex_id = active_vertices.back();
            active_vertices.pop_back();

            auto begin_it = graph.NeighbourIterator(vertex_id);
            auto end_it = graph.NeighbourEndIterator(vertex_id);

            std::reverse_iterator<std::vector<dg::Neighbor>::const_iterator> rev_it(end_it);
            std::reverse_iterator<std::vector<dg::Neighbor>::const_iterator> rev_end(begin_it);

            for (; rev_it != rev_end; ++rev_it) {
                dg::VertexID neighbor_id = rev_it->vertex_id;
                dg::Weight w = rev_it->weight;

                if (w <= suitor_weight[neighbor_id]) {
                    break;
                }

                dg::VertexID current_suitor = suitor[neighbor_id];
                suitor[neighbor_id] = vertex_id;
                suitor_weight[neighbor_id] = w;

                if (current_suitor != -1) {
                    active_vertices.push_back(current_suitor);
                }
                break;
            }
        }

        std::vector<bool> visited(n, false);
        for (dg::VertexID vertex_id = 0; vertex_id < n; ++vertex_id) {
            if (!visited[vertex_id]) {
                dg::VertexID partner = suitor[vertex_id];
                if (partner != -1 && suitor[partner] == vertex_id) {
                    matching_edges.push_back({vertex_id, partner});
                    total_matching_weight += suitor_weight[vertex_id];
                    visited[vertex_id] = true;
                    visited[partner] = true;
                }
            }
        }
    }

    const std::vector<std::pair<dg::VertexID, dg::VertexID>>& SuitorMatching::GetMatchingEdges() const {
        return matching_edges;
    }

    dg::Weight SuitorMatching::GetTotalMatchingWeight() const {
        return total_matching_weight;
    }

    bool SuitorMatching::IsMatched(dg::VertexID vertex_id) const {
        if (vertex_id < 0 || vertex_id >= static_cast<int>(suitor.size())) {
            return false;
        }
        dg::VertexID partner = suitor[vertex_id];
        return partner != -1 && suitor[partner] == vertex_id;
    }

    dg::VertexID SuitorMatching::GetPartner(dg::VertexID vertex_id) const {
        if (IsMatched(vertex_id)) {
            return suitor[vertex_id];
        }
        return -1;
    }

} // namespace matching