//
// Created by jiahua on 2019/9/25.
//
#include "hash_map.h"
#include <cstddef>
#include <cstdint>

template<>
HashMap<uint64_t>::HashMap(size_t size): arcs(size) {
    for (auto &p : arcs) p.store(nullptr);
}

template<>
bool HashMap<uint64_t>::Get(size_t key, uint64_t &value) {
    Rc target = arcs[key].load();
    if (target == nullptr) {
        return false;
    }
    value = *target;
    return true;
}

template<>
bool HashMap<uint64_t>::Put(size_t key, const uint64_t &value, bool overwrite) {
    Rc data(new uint64_t(value));
    if (!overwrite) {
        Rc target = arcs[key].load();
        if (target != nullptr) {
            return false;
        }
        for (int i = 0; i < 20; i++) {
            if (arcs[key].compare_exchange_strong(target, data)) {
                return true;
            }
        }
        return false;
    }
    arcs[key].store(data);
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
    return arcs.size();
}

template<>
bool HashMap<uint64_t>::Check() const {
    return true;
}

