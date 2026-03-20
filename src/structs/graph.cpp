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
        vertices[vertex_id1].AddNeighbor(vertex_id2, weight);
    }

    void Graph::RemoveEdge(VertexID vertex_id1, VertexID vertex_id2) {
        vertices[vertex_id1].RemoveNeighbor(vertex_id2);
    }

    void Graph::ChangeEdgeWeight(VertexID vertex_id1, VertexID vertex_id2, Weight new_weight) {
        vertices[vertex_id1].ChangeWeight(vertex_id2, new_weight);
    }

    bool Graph::AreNeighbors(VertexID vertex_id1, VertexID vertex_id2) const {
        return vertices[vertex_id1].IsNeighbor(vertex_id2);
    }

    Weight Graph::GetEdgeWeight(VertexID vertex_id1, VertexID vertex_id2) const {
        return vertices[vertex_id1].GetWeight(vertex_id2);
    }
}// namespace dg