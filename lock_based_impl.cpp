//
// Created by jiahua on 2019/9/24.
//

#include "hash_map.h"
#include <cstddef>
#include <cstdint>
#include <memory>

using lock_guard = std::lock_guard<std::mutex>;

template<>
HashMap<uint64_t>::HashMap(size_t size): ptrs_(size), locks_(size) {}

template<>
bool HashMap<uint64_t>::Get(size_t key, uint64_t &value) {
    lock_guard lk(locks_[key]);
    auto &ptr = ptrs_[key];
    if (ptr == nullptr) return false;
    value = *ptrs_[key];
    return true;
}

template<>
bool HashMap<uint64_t>::Put(size_t key, const uint64_t &value, bool overwrite) {
    lock_guard lk(locks_[key]);
    std::unique_ptr<uint64_t> &p = ptrs_[key];
    if (p != nullptr && !overwrite) return false;
    p = std::make_unique<uint64_t>(value);
    return true;
}

template<>
bool HashMap<uint64_t>::Put(size_t key, uint64_t &&value, bool overwrite) {
    lock_guard lk(locks_[key]);
    std::unique_ptr<uint64_t> &p = ptrs_[key];
    if (p != nullptr && !overwrite) return false;
    p = std::make_unique<uint64_t>(value);
    ptrs_[key] = std::make_unique<uint64_t>(value);
    return true;
}

template<>
bool HashMap<uint64_t>::Delete(size_t key) {
    lock_guard lk(locks_[key]);
    return true;
}

template<>
size_t HashMap<uint64_t>::Size() const {
    return ptrs_.size();
}

template<>
bool HashMap<uint64_t>::Check() const {
    for (size_t i = 0; i < ptrs_.size(); i++) {
        if (ptrs_[i] != nullptr && *ptrs_[i] != i + 1) {
            return false;
        }
    }
    return true;
}