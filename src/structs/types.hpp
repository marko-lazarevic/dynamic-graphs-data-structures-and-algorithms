#pragma once

namespace dg{
    using VertexID = int;
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


}// namespace dg