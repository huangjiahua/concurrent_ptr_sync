//
// Created by jiahua on 2019/10/16.
//
#include "hash_map/concurrent_hash_map.h"
#include "hash_map/concurrent_hash_map.h"
#include "general_bench.h"
#include <vector>

using BucketMap = bucket::BucketMap<uint64_t, uint64_t, std::hash<uint64_t>, std::equal_to<>>;
using std::vector;

constexpr size_t TOTAL = 10000000;
constexpr size_t THREAD = 8;
constexpr size_t TASKS = TOTAL / THREAD;

std::atomic<size_t> haz_debug_cnt;

int main() {
    RandomGenerator rng;
    vector<uint64_t> keys;
    vector<int> coins;
    BucketMap map(64, 1.0, 10000);

    for (size_t i = 0; i < TOTAL + 10000; i++) {
        keys.push_back(rng.Gen(0, 1000000000));
        coins.push_back(rng.FlipCoin(0.5));
    }

    auto worker = [](int id, BucketMap &map, const uint64_t *keys, const int *coins, size_t &time) {
        auto t1 = SystemTime::Now();
        for (size_t j = 0; j < TASKS; j++) {
            auto key = keys[j];
            auto coin = coins[j];
            BucketMap::Iterator iter;
            if (coin) {
                map.Find(iter, key);
            } else {
                map.Insert(iter, key, key + id, InsertType::ANY);
            }
        }
        time = SystemTime::Now().DurationSince<std::chrono::microseconds>(t1);
    };

    vector<std::thread> threads(THREAD);
    vector<size_t> time(THREAD, 0);
    for (int i = 0; i < (int) THREAD; i++) {
        threads[i] = std::thread(worker, i, std::ref(map), keys.data() + i * TASKS, coins.data() + i * TASKS,
                                 std::ref(time[i]));
    }

    for (auto &t : threads) t.join();

    std::cout << haz_debug_cnt.load() << std::endl;
}

