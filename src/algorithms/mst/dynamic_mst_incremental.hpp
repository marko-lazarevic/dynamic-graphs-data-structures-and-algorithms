#pragma once

#include "../../structs/graph.hpp"
#include <vector>
#include <algorithm>

namespace dynamic_mst {

    class DynamicMSTIncremental {
    private:
        struct Edge {
            dg::VertexID u;
            dg::VertexID v;
            dg::Weight w;
        };

        dg::Graph graph;

        std::vector<Edge> mst_edges;

        std::vector<int> parent;
        std::vector<int> rank;

        dg::Weight mst_weight = 0;

        int find(int x);
        bool union_(int a, int b);

        void rebuildMST();

    public:
        DynamicMSTIncremental() : graph(true), mst_weight(0) {}

        int VertexCount() const;
        dg::VertexID AddVertex();

        void AddEdge(dg::VertexID vertex_id1,
                    dg::VertexID vertex_id2,
                    dg::Weight weight = 1);

        dg::Weight GetEdgeWeight(dg::VertexID vertex_id1,
                                dg::VertexID vertex_id2) const;

        void UpdateEdgeWeight(dg::VertexID vertex_id1,
                            dg::VertexID vertex_id2,
                            dg::Weight new_weight);

        bool IsInMST(dg::VertexID vertex_id1,
                    dg::VertexID vertex_id2) const;

        dg::Weight GetMSTWeight() const;
    };

} // namespace dynamic_mst