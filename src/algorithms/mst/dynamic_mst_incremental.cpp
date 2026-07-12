#include "dynamic_mst_incremental.hpp"

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

    void DynamicMSTIncremental::rebuildMST() {
        mst_edges.clear();
        mst_weight = 0;

        std::vector<Edge> edges;

        for (auto it = graph.VertexIterator();
            it != graph.VertexEndIterator();
            ++it)
        {
            dg::VertexID u = it->Id();

            for (auto nit = graph.NeighbourIterator(u);
                nit != graph.NeighbourEndIterator(u);
                ++nit)
            {
                dg::VertexID v = nit->vertex_id;

                if (u < v) {
                    edges.push_back({u, v, nit->weight});
                }
            }
        }

        std::sort(edges.begin(), edges.end(),
                [](const Edge& a, const Edge& b) {
                    return a.w < b.w;
                });

        parent.resize(graph.VertexCount());
        rank.resize(graph.VertexCount());

        for (int i = 0; i < graph.VertexCount(); i++)
        {
            parent[i] = i;
            rank[i] = 0;
        }

        for (const auto& e : edges) {
            if (unite(e.u, e.v)) {
                mst_edges.push_back(e);
                mst_weight += e.w;
            }
        }
    }

    void DynamicMSTIncremental::AddEdge(dg::VertexID u,
                                        dg::VertexID v,
                                        dg::Weight w)
    {
        graph.AddEdge(u, v, w);
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