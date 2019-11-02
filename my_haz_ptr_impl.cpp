//
// Created by jiahua on 2019/10/12.
//
#include "hash_map.h"
#include <iostream>
#include <cstddef>
#include <cstdint>
#include <atomic>

template<>
HashMap<uint64_t>::HashMap(size_t size): haz_atomics_(size) {
    for (auto &p : haz_atomics_) p.store(nullptr);
    HazPtrInit();
}

template<>
bool HashMap<uint64_t>::Get(size_t key, uint64_t &value) {
    HazPtrHolder h;
    const uint64_t *ptr = h.Pin(haz_atomics_[key]);
    if (!ptr) return false;
    value = *ptr;
    return true;
}

template<>
bool HashMap<uint64_t>::Put(size_t key, const uint64_t &value, bool overwrite) {
    if (!overwrite) {
        HazPtrHolder h;
        const uint64_t *ptr = h.Pin(haz_atomics_[key]);
        if (ptr) return false;
    }
    uint64_t *new_w = new uint64_t(value);
    const uint64_t *old = haz_atomics_[key].exchange(new_w);
    HazPtrRetire(old);
    return true;
}

template<>
bool HashMap<uint64_t>::Put(size_t key, uint64_t &&value, bool overwrite) {
    if (!overwrite) {
        HazPtrHolder h;
        const uint64_t *ptr = h.Pin(haz_atomics_[key]);
        if (ptr) return false;
    }
    uint64_t *new_w = new uint64_t(value);
    const uint64_t *old = haz_atomics_[key].exchange(new_w);
    HazPtrRetire(old);
    return true;
}

template<>
bool HashMap<uint64_t>::Delete(size_t key) {
    return false;
}

template<>
size_t HashMap<uint64_t>::Size() const {
    return haz_atomics_.size();
}

template<>
bool HashMap<uint64_t>::Check() const {
    for (size_t i = 0; i < haz_atomics_.size(); i++) {
        const uint64_t *k = haz_atomics_[i].load();
        if (!k || *k != i + 1) {
            return false;
        }
    }
    return true;
}


ENABLE_LOCAL_DOMAIN
