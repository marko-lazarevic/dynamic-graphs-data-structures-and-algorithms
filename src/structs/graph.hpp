#pragma once

#include "vertex.hpp"

class Graph {
public:
    Graph() = default;

    int VertexCount() const;
    void AddVertex();
    void RemoveVertex(int vertex_id);
    void AddEdge(int vertex_id1, int vertex_id2, int weight = 1);
    void RemoveEdge(int vertex_id1, int vertex_id2);
    void ChangeEdgeWeight(int vertex_id1, int vertex_id2, int new_weight);
    bool AreNeighbors(int vertex_id1, int vertex_id2) const;
    int GetEdgeWeight(int vertex_id1, int vertex_id2) const;

private:
    std::vector<Vertex> vertices;
    int vertex_count = 0;

};