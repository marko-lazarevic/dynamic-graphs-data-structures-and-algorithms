#include "graph.hpp"

namespace dg {
    int Graph::VertexCount() const {
        return vertex_count;
    }

    VertexID Graph::AddVertex() {
        Vertex new_vertex(vertex_count);
        vertices.emplace_back(new_vertex);
        vertex_count++;
        return vertex_count - 1;
    }

    void Graph::RemoveVertex(VertexID vertex_id) {
        if (vertex_id < 0 || vertex_id >= vertex_count) {
            return;
        }

        for (VertexID i = 0; i < vertex_count; ++i) {
            if (i == vertex_id) {
                continue;
            }
            vertices[i].RemoveNeighbor(vertex_id);
        }

        vertices.erase(vertices.begin() + vertex_id);
        vertex_count--;

        for (VertexID i = 0; i < vertex_count; ++i) {
            vertices[i].SetId(i);
            vertices[i].RemapNeighborIdsAfterVertexRemoval(vertex_id);
        }
    }

    void Graph::AddEdge(VertexID vertex_id1, VertexID vertex_id2, Weight weight) {
        vertices[vertex_id1].AddNeighbor(vertex_id2, weight, edges_sorted_by_weight);
    }

    void Graph::RemoveEdge(VertexID vertex_id1, VertexID vertex_id2) {
        vertices[vertex_id1].RemoveNeighbor(vertex_id2);
    }

    void Graph::UpdateEdgeWeight(VertexID vertex_id1, VertexID vertex_id2, Weight new_weight) {
        vertices[vertex_id1].ChangeWeight(vertex_id2, new_weight);
    }

    bool Graph::AreNeighbors(VertexID vertex_id1, VertexID vertex_id2) const {
        return vertices[vertex_id1].IsNeighbor(vertex_id2);
    }

    Weight Graph::GetEdgeWeight(VertexID vertex_id1, VertexID vertex_id2) const {
        return vertices[vertex_id1].GetWeight(vertex_id2);
    }

    std::vector<Neighbor>::const_iterator Graph::NeighbourIterator(VertexID vertex_id) const {
        return vertices[vertex_id].NeighbourIterator();
    }

    std::vector<Neighbor>::const_iterator Graph::NeighbourEndIterator(VertexID vertex_id) const {
        return vertices[vertex_id].NeighbourEndIterator();
    }

    std::vector<Vertex>::const_iterator Graph::VertexIterator() const {
        return vertices.cbegin();
    }

    std::vector<Vertex>::const_iterator Graph::VertexEndIterator() const {
        return vertices.cend();
    }
}// namespace dg