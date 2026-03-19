#include "graph.hpp"

int Graph::VertexCount() const {
    return vertex_count;
}

void Graph::AddVertex() {
    Vertex new_vertex(vertex_count);
    vertices.emplace_back(new_vertex);
    vertex_count++;
}

void Graph::RemoveVertex(int vertex_id) {
    if (vertex_id < 0 || vertex_id >= vertex_count) {
        return;
    }

    for (int i = 0; i < vertex_count; ++i) {
        if (i == vertex_id) {
            continue;
        }
        vertices[i].RemoveNeighbor(vertex_id);
    }

    vertices.erase(vertices.begin() + vertex_id);
    vertex_count--;

    for (int i = 0; i < vertex_count; ++i) {
        vertices[i].SetId(i);
        vertices[i].RemapNeighborIdsAfterVertexRemoval(vertex_id);
    }
}

void Graph::AddEdge(int vertex_id1, int vertex_id2, int weight) {
    vertices[vertex_id1].AddNeighbor(vertex_id2, weight);
}

void Graph::RemoveEdge(int vertex_id1, int vertex_id2) {
    vertices[vertex_id1].RemoveNeighbor(vertex_id2);
}

void Graph::ChangeEdgeWeight(int vertex_id1, int vertex_id2, int new_weight) {
    vertices[vertex_id1].ChangeWeight(vertex_id2, new_weight);
}

bool Graph::AreNeighbors(int vertex_id1, int vertex_id2) const {
    return vertices[vertex_id1].IsNeighbor(vertex_id2);
}

int Graph::GetEdgeWeight(int vertex_id1, int vertex_id2) const {
    return vertices[vertex_id1].GetWeight(vertex_id2);
}
