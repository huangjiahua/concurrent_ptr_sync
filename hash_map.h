//
// Created by jiahua on 2019/9/23.
//

#pragma once
#include <vector>
#include <mutex>
#include <atomic>
#include <memory>

#ifdef BOOST_ARC
#include <boost/smart_ptr/atomic_shared_ptr.hpp>
#endif

// hash map related
template<typename T, typename V = T>
class HashMap {
public:
    std::vector<V> values_;
    std::vector<std::unique_ptr<V>> ptrs_;
    std::vector<std::mutex> locks_;
    std::vector<std::atomic<const T*>> atomic_ptrs_;
#ifdef BOOST_ARC
    using Arc = boost::atomic_shared_ptr<T>;
    using Rc = boost::shared_ptr<T>;
    std::vector<Arc> arcs;
#endif

    explicit HashMap(size_t size);

    bool Get(size_t key, T &value);

    bool Put(size_t key, const T &value, bool overwrite = true);

    bool Put(size_t key, T &&value, bool overwrite = true);

    bool Delete(size_t key);

    bool Check() const;

    size_t Size() const;
};

//template<typename T>
//HashMap<T>::HashMap(size_t size) {
//
//}
//
//template<typename T>
//bool HashMap<T>::Get(size_t key, T &value) {
//    return false;
//}
//
//template<typename T>
//bool HashMap<T>::Put(size_t key, const T &value, bool overwrite) {
//    return false;
//}
//
//template<typename T>
//bool HashMap<T>::Put(size_t key, T &&value, bool overwrite) {
//    return false;
//}
//
//template<typename T>
//bool HashMap<T>::Delete(size_t key) {
//    return false;
//}
//
//template<typename T>
//size_t HashMap<T>::Size() const {
//    return 0;
//}
