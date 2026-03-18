#include "vertex.hpp"

Vertex::Vertex(int id) : id(id), degree(0), beta(4) {
    neighbors.resize(beta);
    hashes.resize(beta, HashEntry(-1, EMPTY));
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

void Vertex::AddNeighbor(int neighbor_id, int weight) {
    neighbors[degree] = Neighbor(neighbor_id, weight);
    hashes[neighbor_id] = HashEntry(degree, OCCUPIED);
    degree++;
    if (degree >= beta/2) {
        beta *= 2;
        neighbors.resize(beta);
        hashes.resize(beta, HashEntry(-1, EMPTY));
    }
}

void Vertex::ChangeWeight(int neighbor_id, int new_weight) {
    int index = hashes[neighbor_id].index;
    HashValue hash_value = hashes[neighbor_id].hash_value;
    if (hash_value == HashValue::OCCUPIED) {
        neighbors[index].weight = new_weight;
    }
}

void Vertex::RemoveNeighbor(int neighbor_id) {
    int index = hashes[neighbor_id].index;
    if (HashValue::OCCUPIED == hashes[neighbor_id].hash_value) {
        // Move the last neighbor to the removed spot
        neighbors[index] = neighbors[degree - 1];
        // Update the hash entry for the moved neighbor
        hashes[neighbors[index].vertex_id] = HashEntry(index, OCCUPIED);
        degree--;
        hashes[neighbor_id] = HashEntry(-1, DELETED);
    }
}

std::vector<Neighbor>::const_iterator Vertex::NeighbourIterator() const {
    return neighbors.cbegin();
}

bool Vertex::IsNeighbor(int neighbor_id) const {
    return hashes[neighbor_id].hash_value == OCCUPIED;
}

int Vertex::GetWeight(int neighbor_id) const {
    int index = hashes[neighbor_id].index;
    if (index >= 0 && index < degree) {
        return neighbors[index].weight;
    }
    return -1; // or throw an exception
}

void Vertex::SortEdges() {
    // Sort neighbors by vertex_id
    std::sort(neighbors.begin(), neighbors.begin() + degree, [](const Neighbor& a, const Neighbor& b) {
        return a.vertex_id < b.vertex_id;
    });

    // reset hash table
    for (int i = 0; i < hashes.size(); ++i) {
        hashes[i] = HashEntry(-1, EMPTY);
    }

    // rebuild the hash table after sorting
    for (int i = 0; i < degree; ++i) {
        int neighbor_id = neighbors[i].vertex_id;
        hashes[neighbor_id] = HashEntry(i, OCCUPIED);
    }
}