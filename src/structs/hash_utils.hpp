#pragma once

#include <cstdint>

namespace hash_utils{
    // Fast modulo reduction
    // Source https://github.com/hu-macsy/dhb.git
    inline std::uint64_t hash64(std::uint64_t x) {
        constexpr std::uint64_t op_a = 0xbf58476d1ce4e5b9;
        constexpr std::uint64_t op_b = 0x94d049bb133111eb;

        x = (x ^ (x >> 30)) * op_a;
        x = (x ^ (x >> 27)) * op_b;
        x = x ^ (x >> 31);
        return x;
    }
} // namespace hash_utils