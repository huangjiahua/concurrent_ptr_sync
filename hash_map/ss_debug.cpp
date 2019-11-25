//
// Created by jiahua on 2019/11/6.
//

#include "hash_map/concurrent_hash_map.h"
#include "hash_map/thread.h"
#include "general_bench.h"


int main() {
    GeneralLazySS<size_t> ss(0.00001);
    RandomGenerator rng;
    for (size_t i = 0; i < 5000000; i++) {
        ss.put(rng.GenZipf<size_t>(1000000000ull, 0.99));
    }

    auto p = ss.output(false);
    for (size_t i = 0; i < 100; i++) {
        std::cout << p[i].getItem() << std::endl;
    }
}

