#include "dynamic_connectivity.hpp"

namespace dynamic_connectivity {

    int DynamicConnectivityIncremental::VertexCount() const {
        return graph.VertexCount();
    }

    dg::VertexID DynamicConnectivityIncremental::AddVertex() {
        int vertex_id = graph.AddVertex();
        parent.push_back(vertex_id);
        rank.push_back(0);
        return vertex_id;
    }

    int DynamicConnectivityIncremental::find(int x) {
        if (x < 0 || x >= static_cast<int>(parent.size())) return -1;
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }

    void DynamicConnectivityIncremental::union_(int a, int b) {
        int ra = find(a);
        int rb = find(b);
        if (ra == -1 || rb == -1) return;
        if (ra == rb) return;
        if (rank[ra] < rank[rb]) parent[ra] = rb;
        else if (rank[ra] > rank[rb]) parent[rb] = ra;
        else {
            parent[rb] = ra;
            rank[ra]++;
        }
    }

    void DynamicConnectivityIncremental::AddEdge(dg::VertexID vertex_id1, dg::VertexID vertex_id2, dg::Weight weight) {
        graph.AddEdge(vertex_id1, vertex_id2, weight);
        graph.AddEdge(vertex_id2, vertex_id1, weight);
        union_(vertex_id1, vertex_id2);
    }

    dg::Weight DynamicConnectivityIncremental::GetEdgeWeight(dg::VertexID vertex_id1, dg::VertexID vertex_id2) const {
        return graph.GetEdgeWeight(vertex_id1, vertex_id2);
    }

    void DynamicConnectivityIncremental::UpdateEdgeWeight(dg::VertexID vertex_id1, dg::VertexID vertex_id2, dg::Weight new_weight) {
        graph.UpdateEdgeWeight(vertex_id1, vertex_id2, new_weight);
        graph.UpdateEdgeWeight(vertex_id2, vertex_id1, new_weight);
    }

    bool DynamicConnectivityIncremental::IsConnected(dg::VertexID vertex_id1, dg::VertexID vertex_id2) {
        int r1 = find(vertex_id1);
        int r2 = find(vertex_id2);
        if (r1 == -1 || r2 == -1) return false;
        return r1 == r2;
    }
}