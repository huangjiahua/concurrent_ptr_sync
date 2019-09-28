//
// Created by jiahua on 2019/9/23.
//

#include "hash_map.h"
#include <cstddef>
#include <cstdint>

template<>
HashMap<uint64_t>::HashMap(size_t size) {
    mps.reserve(size);
    for (size_t i = 0; i < size; i++) {
        mps.push_back(std::move(Mrsw(new uint64_t(0))));
    }
}

template<>
bool HashMap<uint64_t>::Get(size_t key, uint64_t &value) {
    auto r = mps[key].GetReader();
    if (!r) {
        return false;
    }
    value = *r;
    return true;
}

template<>
bool HashMap<uint64_t>::Put(size_t key, const uint64_t &value, bool overwrite) {
    auto w = mps[key].GetWriter();
    uint64_t *data = new uint64_t(value);
    uint64_t *old = w.Swap(data);
    delete old;
    return true;
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
    return mps.size();
}

template<>
bool HashMap<uint64_t>::Check() const {
    return true;
}
