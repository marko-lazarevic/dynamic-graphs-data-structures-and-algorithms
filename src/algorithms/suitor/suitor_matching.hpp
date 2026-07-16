#pragma once

#include "../../structs/graph.hpp"
#include <vector>
#include <utility>

namespace matching {

    class SuitorMatching {
    private:
        dg::Graph graph;
        std::vector<dg::VertexID> suitor;
        std::vector<dg::Weight> suitor_weight;
        std::vector<std::pair<dg::VertexID, dg::VertexID>> matching_edges;
        dg::Weight total_matching_weight;

        void rebuildMatching();

    public:
        SuitorMatching() : graph(true), total_matching_weight(0) {}

        dg::VertexID AddVertex();
        void AddEdge(dg::VertexID vertex_id1,
                    dg::VertexID vertex_id2,
                    dg::Weight weight = 1);
        void RemoveEdge(dg::VertexID vertex_id1,
                dg::VertexID vertex_id2);
        void RemoveVertex(dg::VertexID vertex_id);
        void UpdateEdgeWeight(dg::VertexID vertex_id1,
                            dg::VertexID vertex_id2,
                            dg::Weight new_weight);

        const std::vector<std::pair<dg::VertexID, dg::VertexID>>& GetMatchingEdges() const;
        dg::Weight GetTotalMatchingWeight() const;
        bool IsMatched(dg::VertexID vertex_id) const;
        dg::VertexID GetPartner(dg::VertexID vertex_id) const;
    };

} // namespace matching