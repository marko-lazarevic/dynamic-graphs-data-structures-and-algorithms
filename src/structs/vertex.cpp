#include "vertex.hpp"

namespace dg {
    Vertex::Vertex(VertexID id) : id(id), degree(0), beta(4) {
        neighbors.resize(beta);
        hashes.resize(beta, HashEntry(-1, EMPTY));
    }

    int Vertex::HashToSlot(VertexID neighbor_id) const {
        return (neighbor_id & (beta - 1));
    }

    int Vertex::FindSlot(VertexID neighbor_id) const {
        if (neighbor_id < 0 || beta == 0) {
            return -1;
        }

        int slot = HashToSlot(neighbor_id);
        for (int steps = 0; steps < beta; ++steps) {
            const HashEntry& entry = hashes[slot];
            if (entry.hash_value == EMPTY) {
                return -1;
            }

            if (entry.hash_value == OCCUPIED) {
                int index = entry.index;
                if (index >= 0 && index < degree && neighbors[index].vertex_id == neighbor_id) {
                    return slot;
                }
            }

            slot = (slot + 1) & (beta - 1);
        }

        return -1;
    }

    int Vertex::FindInsertSlot(VertexID neighbor_id, bool& already_present) const {
        already_present = false;
        if (neighbor_id < 0 || beta == 0) {
            return -1;
        }

        int slot = HashToSlot(neighbor_id);
        int first_deleted = -1;

        for (int steps = 0; steps < beta; ++steps) {
            const HashEntry& entry = hashes[slot];

            if (entry.hash_value == EMPTY) {
                return (first_deleted != -1) ? first_deleted : slot;
            }

            if (entry.hash_value == DELETED) {
                if (first_deleted == -1) {
                    first_deleted = slot;
                }
            } else {
                int index = entry.index;
                if (index >= 0 && index < degree && neighbors[index].vertex_id == neighbor_id) {
                    already_present = true;
                    return slot;
                }
            }

            slot = (slot + 1) & (beta - 1);
        }

        return first_deleted;
    }

    void Vertex::RebuildHashIndex() {
        hashes.assign(beta, HashEntry(-1, EMPTY));

        for (int i = 0; i < degree; ++i) {
            int neighbor_id = neighbors[i].vertex_id;
            int slot = HashToSlot(neighbor_id);
            while (hashes[slot].hash_value == OCCUPIED) {
                slot = (slot + 1) & (beta - 1);
            }
            hashes[slot] = HashEntry(i, OCCUPIED);
        }
    }

    void Vertex::GrowAndRehash() {
        beta *= 2;
        neighbors.resize(beta);
        RebuildHashIndex();
    }

    int Vertex::Id() const {
        return id;
    }

    int Vertex::Degree() const {
        return degree;
    }

    int Vertex::Beta() const {
        return beta;
    }

    void Vertex::AddNeighbor(VertexID neighbor_id, Weight weight) {
        if (neighbor_id < 0) {
            return;
        }

        bool already_present = false;
        FindInsertSlot(neighbor_id, already_present);
        if (already_present) {
            return;
        }

        if (degree >= beta / 2) {
            GrowAndRehash();
        }

        int insert_slot = FindInsertSlot(neighbor_id, already_present);
        if (insert_slot < 0 || already_present) {
            return;
        }

        neighbors[degree] = Neighbor(neighbor_id, weight);
        hashes[insert_slot] = HashEntry(degree, OCCUPIED);
        degree++;
    }

    void Vertex::ChangeWeight(VertexID neighbor_id, Weight new_weight) {
        int slot = FindSlot(neighbor_id);
        if (slot != -1) {
            int index = hashes[slot].index;
            neighbors[index].weight = new_weight;
        }
    }

    void Vertex::RemoveNeighbor(VertexID neighbor_id, bool preserve_order) {
        int slot = FindSlot(neighbor_id);
        if (slot == -1 || degree == 0) {
            return;
        }

        int index = hashes[slot].index;

        if (preserve_order) {
            for (int i = index; i + 1 < degree; ++i) {
                neighbors[i] = neighbors[i + 1];
            }
            degree--;
            RebuildHashIndex();
            return;
        }

        int last_index = degree - 1;

        if (index != last_index) {
            neighbors[index] = neighbors[last_index];

            int moved_neighbor_id = neighbors[index].vertex_id;
            int moved_slot = FindSlot(moved_neighbor_id);
            if (moved_slot != -1) {
                hashes[moved_slot].index = index;
            }
        }

        degree--;
        hashes[slot] = HashEntry(-1, DELETED);
    }

    std::vector<Neighbor>::const_iterator Vertex::NeighbourIterator() const {
        return neighbors.cbegin();
    }

    std::vector<Neighbor>::const_iterator Vertex::NeighbourEndIterator() const {
        return neighbors.cbegin() + degree;
    }

    bool Vertex::IsNeighbor(VertexID neighbor_id) const {
        return FindSlot(neighbor_id) != -1;
    }

    Weight Vertex::GetWeight(VertexID neighbor_id) const {
        int slot = FindSlot(neighbor_id);
        if (slot != -1) {
            int index = hashes[slot].index;
            return neighbors[index].weight;
        }
        return -1; // or throw an exception
    }

    void Vertex::SortEdges() {
        // Sort neighbors by vertex_id
        std::sort(neighbors.begin(), neighbors.begin() + degree, [](const Neighbor& a, const Neighbor& b) {
            return a.vertex_id < b.vertex_id;
        });

        RebuildHashIndex();
    }

    void Vertex::SetId(VertexID new_id) {
        id = new_id;
    }

    void Vertex::RemapNeighborIdsAfterVertexRemoval(VertexID removed_vertex_id) {
        int write_index = 0;

        for (int read_index = 0; read_index < degree; ++read_index) {
            Neighbor current_neighbor = neighbors[read_index];

            if (current_neighbor.vertex_id == removed_vertex_id) {
                continue;
            }

            if (current_neighbor.vertex_id > removed_vertex_id) {
                current_neighbor.vertex_id--;
            }

            neighbors[write_index] = current_neighbor;
            write_index++;
        }

        degree = write_index;
        RebuildHashIndex();
    }
} // namespace dg