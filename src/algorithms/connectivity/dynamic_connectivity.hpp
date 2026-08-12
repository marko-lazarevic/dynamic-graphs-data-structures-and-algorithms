#pragma once
#include "../../structs/graph.hpp"

namespace dynamic_connectivity {

    class DynamicConnectivityIncremental {
    private:
        dg::Graph graph;
        std::vector<int> parent;
        std::vector<int> rank;

        int find(int x);
        void union_(int a, int b);

    public:
        DynamicConnectivityIncremental() = default;

        int VertexCount() const;
        dg::VertexID AddVertex();
        
        void AddEdge(dg::VertexID vertex_id1, dg::VertexID vertex_id2, dg::Weight weight = 1);
        dg::Weight GetEdgeWeight(dg::VertexID vertex_id1, dg::VertexID vertex_id2) const;
        void UpdateEdgeWeight(dg::VertexID vertex_id1, dg::VertexID vertex_id2, dg::Weight new_weight);

        bool IsConnected(dg::VertexID vertex_id1, dg::VertexID vertex_id2);
    };

} // namespace dynamic_connectivity