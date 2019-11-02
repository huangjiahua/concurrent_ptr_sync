//
// Created by jiahua on 2019/11/2.
//

#pragma once

#include <atomic>
#include <memory>

#include "my_haz_ptr/haz_ptr.h"

template<typename K, typename V>
class FastTableNode {
public:
    using KVPair = std::pair<K, V>;
    KVPair data_;

    FastTableNode(const K &k, const V &v) : data_{k, v} {}

    const K &Key() const {
        return data_.first;
    }

    const V &Value() const {
        return data_.second;
    }

    ~FastTableNode() = default;
};

template<typename K, typename V>
class alignas(128) FastTableSlot {
public:
    template<typename T> using Atom = std::atomic<T>;
    Atom<FastTableNode<K, V> *> atom_ptr_;

    FastTableSlot() : atom_ptr_{nullptr} {}

    ~FastTableSlot() = default;
};

template<typename KeyType, typename ValueType>
class FastTable {
private:
    template<typename T> using Atom = std::atomic<T>;
    using Node = FastTableNode<KeyType, ValueType>;
    using Slot = FastTableSlot<KeyType, ValueType>;
    size_t size_;
    Slot *table_;
public:
    // size must be power of 2
    explicit FastTable(size_t size) : size_(size) {
        table_ = (Slot *) std::allocator<uint8_t>().allocate(sizeof(Slot) * size_);
        for (size_t i = 0; i < size_; i++) {
            new(table_ + i) Slot;
        }
    }

    ~FastTable() noexcept {
        for (size_t i = 0; i < size_; i++) {
            (table_ + i)->~Slot();
        }
        std::allocator<uint8_t>().deallocate((uint8_t *) table_, sizeof(Slot) * size_);
    }

    void Insert(size_t hash, const KeyType &key, const ValueType &value) {
        Node *node = new Node(key, value);
        size_t idx = GetIdx(hash);
        Node *old = table_[idx].atom_ptr_.exchange(node);
        HazPtrRetire(old);
    }

    Node *PinnedFind(size_t hash, const KeyType &key, HazPtrHolder &holder) {
        size_t idx = GetIdx(hash);
        Node *node = holder.Pin(table_[idx].atom_ptr_);
        if (!node || node->Key() != key) {
            holder.Reset();
            return nullptr;
        }
        return node;
    }

private:
    inline size_t GetIdx(size_t hash) {
        return hash & (size_ - 1);
    }
};

