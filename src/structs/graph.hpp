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
        Weight GetEdgeWeight(VertexID vertex_id1, VertexID vertex_id2) const;
        void UpdateEdgeWeight(VertexID vertex_id1, VertexID vertex_id2, Weight new_weight);

        bool AreNeighbors(VertexID vertex_id1, VertexID vertex_id2) const;
        std::vector<Vertex>::const_iterator VertexIterator() const;
        std::vector<Vertex>::const_iterator VertexEndIterator() const;
        std::vector<Neighbor>::const_iterator NeighbourIterator(VertexID vertex_id) const;
        std::vector<Neighbor>::const_iterator NeighbourEndIterator(VertexID vertex_id) const;

    private:
        std::vector<Vertex> vertices;
        int vertex_count = 0;

    };
}