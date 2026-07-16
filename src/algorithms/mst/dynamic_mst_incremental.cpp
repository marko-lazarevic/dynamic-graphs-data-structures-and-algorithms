#include "dynamic_mst_incremental.hpp"
#include <algorithm>
#include <queue>

namespace dynamic_mst {

    int DynamicMSTIncremental::find(int x) {
        if (x < 0 || x >= static_cast<int>(parent.size())) return -1;
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }

    bool DynamicMSTIncremental::unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == -1 || b == -1) return false;
        if (a == b) return false;
        if (rank[a] < rank[b]) parent[a] = b;
        else if (rank[a] > rank[b]) parent[b] = a;
        else {
            parent[b] = a;
            rank[a]++;
        }
        return true;
    }

    int DynamicMSTIncremental::VertexCount() const {
        return graph.VertexCount();
    }

    dg::VertexID DynamicMSTIncremental::AddVertex() {
        dg::VertexID vertex_id = graph.AddVertex();
        parent.push_back(vertex_id);
        rank.push_back(0);
        return vertex_id;
    }

    struct HeapEntry {
        dg::VertexID u;
        std::vector<dg::Neighbor>::const_iterator current;
        std::vector<dg::Neighbor>::const_iterator end;

        bool operator>(const HeapEntry& other) const {
            return current->weight > other.current->weight;
        }
    };

    void DynamicMSTIncremental::rebuildMST() {
        mst_edges.clear();
        mst_weight = 0;

        int n = graph.VertexCount();
        if (n <= 1) return;

        parent.resize(n);
        rank.resize(n);
        for (int i = 0; i < n; i++) {
            parent[i] = i;
            rank[i] = 0;
        }

        std::priority_queue<HeapEntry, std::vector<HeapEntry>, std::greater<HeapEntry>> min_heap;

        for (int u = 0; u < n; ++u) {
            auto it = graph.NeighbourIterator(u);
            auto end = graph.NeighbourEndIterator(u);
            if (it != end) {
                min_heap.push({static_cast<dg::VertexID>(u), it, end});
            }
        }

        int edges_added = 0;
        int target_edges = n - 1;

        while (!min_heap.empty() && edges_added < target_edges) {
            HeapEntry top = min_heap.top();
            min_heap.pop();

            dg::VertexID u = top.u;
            dg::VertexID v = top.current->vertex_id;
            dg::Weight w = top.current->weight;

            if (u < v) {
                if (unite(u, v)) {
                    mst_edges.push_back({u, v, w});
                    mst_weight += w;
                    edges_added++;
                }
            }

            auto next_it = top.current + 1;
            if (next_it != top.end) {
                min_heap.push({u, next_it, top.end});
            }
        }
    }

    void DynamicMSTIncremental::AddEdge(dg::VertexID u,
                                        dg::VertexID v,
                                        dg::Weight w)
    {
        graph.AddEdge(u, v, w);
        graph.AddEdge(v, u, w);
        rebuildMST();
    }

    dg::Weight DynamicMSTIncremental::GetEdgeWeight(dg::VertexID vertex_id1, dg::VertexID vertex_id2) const {
        return graph.GetEdgeWeight(vertex_id1, vertex_id2);
    }

    void DynamicMSTIncremental::UpdateEdgeWeight(dg::VertexID u,
                                                 dg::VertexID v,
                                                 dg::Weight new_w)
    {
        graph.UpdateEdgeWeight(u, v, new_w);
        graph.UpdateEdgeWeight(v, u, new_w);

        graph.RemoveEdge(u, v);
        graph.RemoveEdge(v, u);
        graph.AddEdge(u, v, new_w);
        graph.AddEdge(v, u, new_w);

        rebuildMST();
    }

    dg::Weight DynamicMSTIncremental::GetMSTWeight() const {
        return mst_weight;
    }

    bool DynamicMSTIncremental::IsInMST(dg::VertexID u,
                                        dg::VertexID v) const
    {
        for (const auto& e : mst_edges) {
            if ((e.u == u && e.v == v) ||
                (e.u == v && e.v == u))
                return true;
        }
        return false;
    }

} // namespace dynamic_mst