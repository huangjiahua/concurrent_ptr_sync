//
// Created by jiahua on 2019/10/12.
//
#include "hash_map.h"
#include <iostream>
#include <cstddef>
#include <cstdint>
#include <atomic>


template<>
HashMap<uint64_t>::HashMap(size_t size): haz_ptrs_(size) {
    for (auto &p : haz_ptrs_) p.store(nullptr);
}

template<>
bool HashMap<uint64_t>::Get(size_t key, uint64_t &value) {
    hazptr_holder<> h;
    Wrapper *ptr = h.get_protected(haz_ptrs_[key]);
    if (!ptr) return false;
    value = ptr->Get();
    return true;
}

template<>
bool HashMap<uint64_t>::Put(size_t key, const uint64_t &value, bool overwrite) {
    if (!overwrite) {
        hazptr_holder<> h;
        Wrapper *ptr = h.get_protected(haz_ptrs_[key]);
        if (ptr) return false;
    }
    Wrapper *new_w = new Wrapper(value);
    Wrapper *old = haz_ptrs_[key].exchange(new_w);
    old->retire();
    return true;
}

template<>
bool HashMap<uint64_t>::Put(size_t key, uint64_t &&value, bool overwrite) {
    if (!overwrite) {
        hazptr_holder<> h;
        Wrapper *ptr = h.get_protected(haz_ptrs_[key]);
        if (ptr) return false;
    }
    Wrapper *new_w = new Wrapper(value);
    Wrapper *old = haz_ptrs_[key].exchange(new_w);
    if (old) old->retire();
    return true;
}

template<>
bool HashMap<uint64_t>::Delete(size_t key) {
    return false;
}

template<>
size_t HashMap<uint64_t>::Size() const {
    return haz_ptrs_.size();
}

template<>
bool HashMap<uint64_t>::Check() const {
    for (size_t i = 0; i < haz_ptrs_.size(); i++) {
        uint64_t k = haz_ptrs_[i].load()->Get();
        if (k != i + 1) {
            return false;
        }
    }
    return true;
}

