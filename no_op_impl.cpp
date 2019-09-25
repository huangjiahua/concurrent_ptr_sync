//
// Created by jiahua on 2019/9/23.
//

#include "hash_map.h"
#include <cstddef>
#include <cstdint>

template<>
HashMap<uint64_t>::HashMap(size_t size): ptrs_(size) {}

template<>
bool HashMap<uint64_t>::Get(size_t key, uint64_t &value) {
    return false;
}

template<>
bool HashMap<uint64_t>::Put(size_t key, const uint64_t &value, bool overwrite) {
    return false;
}

template<>
bool HashMap<uint64_t>::Put(size_t key, uint64_t &&value, bool overwrite) {
    return false;
}

template<>
bool HashMap<uint64_t>::Delete(size_t key) {
    return false;
}

template<>
size_t HashMap<uint64_t>::Size() const {
    return ptrs_.size();
}

template<>
bool HashMap<uint64_t>::Check() const {
    return true;
}

