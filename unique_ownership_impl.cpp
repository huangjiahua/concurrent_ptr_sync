//
// Created by jiahua on 2019/9/24.
//
#include "hash_map.h"
#include <cstddef>
#include <cstdint>

const uint64_t *kOccupied = (uint64_t *) 1;

template<>
HashMap<uint64_t>::HashMap(size_t size): atomic_ptrs_(size) {
    for (auto &p : atomic_ptrs_) p.store(nullptr);
}

template<>
bool HashMap<uint64_t>::Get(size_t key, uint64_t &value) {
    const uint64_t *target = atomic_ptrs_[key].load();
    bool flag = false;

    do {
        flag = false;

        if (target == nullptr) {
            return false;
        }

        if (target == kOccupied) {
            auto temp = kOccupied;
            target = atomic_ptrs_[key].load();
            flag = true;
            continue;
        }

        const uint64_t *expected = target;
        if (!atomic_ptrs_[key].compare_exchange_strong(expected, kOccupied)) {
            target = expected;
            flag = true;
            continue;
        }
        value = *expected;
        atomic_ptrs_[key].store(expected);
    } while (flag);
    value = *target;
    return true;
}

template<>
bool HashMap<uint64_t>::Put(size_t key, const uint64_t &value, bool overwrite) {
    const uint64_t *target = atomic_ptrs_[key].load();
    bool flag = false;
    const uint64_t *desired = new uint64_t(value);

    do {
        flag = false;

        if (target == kOccupied) {
            target = atomic_ptrs_[key].load();
            flag = true;
            continue;
        }

        if (target != nullptr && !overwrite) {
            delete desired;
            return false;
        }

        const uint64_t *expected = target;
        if (!atomic_ptrs_[key].compare_exchange_strong(expected, desired)) {
            target = expected;
            flag = true;
            continue;
        }
        delete expected;
    } while (flag);
    return true;
}

template<>
bool HashMap<uint64_t>::Put(size_t key, uint64_t &&value, bool overwrite) {
    // TODO
    return false;
}

template<>
bool HashMap<uint64_t>::Delete(size_t key) {
    return false;
}

template<>
size_t HashMap<uint64_t>::Size() const {
    return atomic_ptrs_.size();
}

template<>
bool HashMap<uint64_t>::Check() const {
    return true;
}
