#pragma once

#include "types.hpp"
#include <vector>
#include <string>
#include <algorithm>

namespace dg {
    
    class Vertex {
    private:
        VertexID id;
        int degree;
        int beta;
        std::vector<Neighbor> neighbors;
        std::vector<HashEntry> hashes;

        int FindSlot(VertexID neighbor_id) const;
        int FindInsertSlot(VertexID neighbor_id, bool& already_present) const;
        int HashToSlot(VertexID neighbor_id) const;
        void RebuildHashIndex();
        void GrowAndRehash();

    public:
        Vertex() = delete;
        explicit Vertex(VertexID id);

        VertexID Id() const;
        int Degree() const;
        int Beta() const;

        void AddNeighbor(VertexID neighbor_id, Weight weight = 1, bool keep_sorted_by_weight = false);
        void ChangeWeight(VertexID neighbor_id, Weight new_weight);
        void RemoveNeighbor(VertexID neighbor_id, bool preserve_order = false);
        bool IsNeighbor(VertexID neighbor_id) const;
        Weight GetWeight(VertexID neighbor_id) const;

        std::vector<Neighbor>::const_iterator NeighbourIterator() const;
        std::vector<Neighbor>::const_iterator NeighbourEndIterator() const;

        
        void SortEdges();
        void SetId(VertexID new_id);
        void RemapNeighborIdsAfterVertexRemoval(VertexID removed_vertex_id);
    };
} // namespace dg