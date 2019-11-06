//
// Created by jiahua on 2019/11/2.
//

#pragma once

#include <atomic>
#include <memory>

#include "haz_ptr/haz_ptr.h"

template<typename K, typename V>
class FastTableNode : public hazptr_obj_base<FastTableNode<K, V>> {
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

template<typename N>
class  FastTableSlot {
public:
    template<typename T> using Atom = std::atomic<T>;
    Atom<N *> atom_ptr_;

    FastTableSlot() : atom_ptr_{nullptr} {}

    ~FastTableSlot() = default;
};

template<typename KeyType, typename ValueType>
class alignas(128) FastTable {
private:
    template<typename T> using Atom = std::atomic<T>;
    using Node = FastTableNode<KeyType, ValueType>;
    using Slot = FastTableSlot<Node>;
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
        if (old) old->retire();
    }

    bool CheckedInsert(size_t hash, const KeyType &key, const ValueType &value) {
        size_t idx = GetIdx(hash);
        hazptr_holder<> holder;
        Node *old_node = holder.get_protected(table_[idx].atom_ptr_);
        if (old_node && old_node->Key() != key) {
            return false;
        }
        Node *node = new Node(key, value);
        bool res = table_[idx].atom_ptr_.compare_exchange_strong(old_node, node, std::memory_order_acq_rel);
        if (!res) {
            delete node;
        }
        return res;
    }

    bool TryUpdate(size_t hash, const KeyType &key, const ValueType &value) {
        size_t idx = GetIdx(hash);
        hazptr_holder<> holder;
        Node *old_node = holder.get_protected(table_[idx].atom_ptr_);
        if (old_node && old_node->Key() == key) {
            Node *node = new Node(key, value);
            bool res = table_[idx].atom_ptr_.compare_exchange_strong(old_node, node, std::memory_order_acq_rel);
            if (!res) {
                delete node;
            }
            return res;
        }
        return false;
    }

    Node *PinnedFind(size_t hash, const KeyType &key, hazptr_holder<> &holder) {
        size_t idx = GetIdx(hash);
        Node *node = holder.get_protected(table_[idx].atom_ptr_);
        if (!node || node->Key() != key) {
            holder.reset(nullptr);
            return nullptr;
        }
        return node;
    }

private:
    inline size_t GetIdx(size_t hash) {
        return hash & (size_ - 1);
    }
};
