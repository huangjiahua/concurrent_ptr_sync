//
// Created by jiahua on 2019/10/14.
//

#include "hash_map/concurrent_hash_map.h"
#include <iostream>
#include <vector>

using namespace std;

int main() {
    ConcurrentHashMap<size_t, size_t, std::hash<size_t>, std::equal_to<>> hash_map(16, 3);
    size_t value = 0;
    size_t key = 1;
    for (size_t i = 0; i < 655360; i++) {
        hash_map.Insert(i, i);
    }

    for (size_t i = 65536; i > 0; i--) {
        hash_map.Find(i - 1, value);
        cout << value << endl;
    }

//    using BucketsMap = bucket::BucketMap<size_t, size_t, std::hash<size_t>, std::equal_to<>>;
//    std::vector<BucketsMap *> maps;
//    for (size_t i = 0; i < 64; i++) {
//        maps.push_back(new BucketsMap(4, 1.0, 10000000));
//    }
//
//    typename BucketsMap::Iterator iter;
//    for (size_t i = 0; i < 6553600; i++) {
//        if ((i & 3ull) == 0) continue;
//        size_t idx = i % maps.size();
//        maps[idx]->Insert(iter, i, i, InsertType::ANY);
//    }
    return 0;
}

