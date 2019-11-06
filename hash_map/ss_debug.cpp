//
// Created by jiahua on 2019/11/6.
//

#include "hash_map/concurrent_hash_map.h"
#include "hash_map/thread.h"
#include "general_bench.h"

int main() {
    std::vector<GeneralLazySS<size_t>> glss(64, GeneralLazySS<size_t>(0.0001));
    std::vector<std::thread> threads(8);

    size_t i = 0;
    for (auto &t : threads) {
        t = std::thread([](size_t tid, std::vector<GeneralLazySS<size_t>> &glss) {
            RandomGenerator rng;
            for (size_t k = 0; k < 100000000; k++) {
                glss[tid].put(rng.Gen<size_t>(0, 1000000000));
            }
        }, i++, std::ref(glss));
    }

    for (auto &t : threads) t.join();

}

