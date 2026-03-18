#ifndef __VERTEX_HPP__
#define __VERTEX_HPP__ 

#include <vector>
#include <string>
#include <algorithm>

enum HashValue {
    OCCUPIED = 0,
    EMPTY = -1,
    DELETED = -2
};

struct HashEntry {
    // either index in neighbors or hash_value can be used to determine if the entry is empty or deleted or occupied
    int index;
    HashValue hash_value;

    HashEntry() : index(-1), hash_value(EMPTY) {}
    HashEntry(int index, HashValue hash_value) : index(index), hash_value(hash_value) {}
};

struct Neighbor {
    int vertex_id;
    int weight;

    Neighbor() : vertex_id(-1), weight(0) {}
    Neighbor(int vertex_id, int weight) : vertex_id(vertex_id), weight(weight) {}
};

class Vertex {
public:
    Vertex() = delete;
    explicit Vertex(int id);

    int Id() const;
    int Degree() const;
    int Beta() const;

    void AddNeighbor(int neighbor_id, int weight = 1);
    void ChangeWeight(int neighbor_id, int new_weight);
    void RemoveNeighbor(int neighbor_id);
    std::vector<Neighbor>::const_iterator NeighbourIterator() const;
    bool IsNeighbor(int neighbor_id) const;
    int GetWeight(int neighbor_id) const;
    void SortEdges();

private:
    int id;
    int degree;
    int beta;
    std::vector<Neighbor> neighbors;
    std::vector<HashEntry> hashes;
};


#endif // __VERTEX_HPP__