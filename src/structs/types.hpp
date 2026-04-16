#pragma once

#include <cstdint>

namespace dg{
    using VertexID = std::int32_t;
    using Weight = int;

    struct Neighbor {
        VertexID vertex_id;
        Weight weight;

        Neighbor() : vertex_id(-1), weight(0) {}
        Neighbor(VertexID vertex_id, Weight weight) : vertex_id(vertex_id), weight(weight) {}
    };

    enum HashValue {
        OCCUPIED = 0,
        EMPTY = -1,
        DELETED = -2
    };

    struct HashEntry {
        unsigned index;
        HashValue hash_value;

        HashEntry() : index(-1), hash_value(EMPTY) {}
        HashEntry(unsigned index, HashValue hash_value) : index(index), hash_value(hash_value) {}
    };

}// namespace dg