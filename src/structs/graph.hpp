#pragma once

#include "vertex.hpp"

namespace dg {
    class Graph {
    public:
        Graph() = default;

        int VertexCount() const;
        VertexID AddVertex();
        void RemoveVertex(VertexID vertex_id);
        void AddEdge(VertexID vertex_id1, VertexID vertex_id2, Weight weight = 1);
        void RemoveEdge(VertexID vertex_id1, VertexID vertex_id2);
        void ChangeEdgeWeight(VertexID vertex_id1, VertexID vertex_id2, Weight new_weight);
        bool AreNeighbors(VertexID vertex_id1, VertexID vertex_id2) const;
        Weight GetEdgeWeight(VertexID vertex_id1, VertexID vertex_id2) const;

    private:
        std::vector<Vertex> vertices;
        int vertex_count = 0;

    };
}