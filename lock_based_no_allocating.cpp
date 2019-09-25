//
// Created by jiahua on 2019/9/24.
//

#include "hash_map.h"
#include <cstddef>
#include <cstdint>
#include <memory>

using lock_guard = std::lock_guard<std::mutex>;

template<>
HashMap<uint64_t>::HashMap(size_t size): values_(size, 0), locks_(size) {}

template<>
bool HashMap<uint64_t>::Get(size_t key, uint64_t &value) {
    lock_guard lk(locks_[key]);
    value = values_[key];
    return true;
}

template<>
bool HashMap<uint64_t>::Put(size_t key, const uint64_t &value, bool overwrite) {
    lock_guard lk(locks_[key]);
    values_[key] = value;
    return true;
}

template<>
bool HashMap<uint64_t>::Put(size_t key, uint64_t &&value, bool overwrite) {
    lock_guard lk(locks_[key]);
    values_[key] = value;
    return true;
}

template<>
bool HashMap<uint64_t>::Delete(size_t key) {
    lock_guard lk(locks_[key]);
    return true;
}

template<>
size_t HashMap<uint64_t>::Size() const {
    return values_.size();
}

template<>
bool HashMap<uint64_t>::Check() const {
    for (size_t i = 0; i < values_.size(); i++) {
        if (values_[i] != i + 1) {
            return false;
        }
    }
    return true;
}
