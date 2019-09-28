//
// Created by jiahua on 2019/9/23.
//

#include "general_bench.h"

// using
using std::cout;
using std::endl;
using std::cerr;
using std::thread;
using std::vector;

using namespace std::chrono_literals;

// random number generator
RandomGenerator rng;

size_t UpdateWorker(
        size_t thread_idx,
        HashMap<uint64_t> &hash_map,
        size_t operations,
        size_t *keys,
        int *coins,
        size_t &time
) {
    auto t = SystemTime::Now();
    for (size_t i = 0; i < operations; i++) {
//        size_t key = thread_idx;
        size_t key = keys[i];
        uint64_t value = key + 1;
        if (!coins[i]) {
            hash_map.Put(key, value, true);
        } else {
            if (hash_map.Get(key, value)) {
                keys[i] = value;
            }
        }
    }
    time = SystemTime::Now().DurationSince<Microseconds>(t);
    return time;
}

int main(int argc, const char *argv[]) {
    Config config;
    config.LoadConfig(argc, argv);
    HashMap<uint64_t> hash_map(config.hash_size);

    vector<size_t> keys(config.operations + config.thread_count * 100);
    for (auto &v : keys) v = rng.Gen<size_t>(0, hash_map.Size());

    vector<int> coins(config.operations + config.thread_count * 100);
    for (auto &v: coins) v = rng.FlipCoin(config.read_ratio);

    vector<thread> threads;
    vector<size_t> epochs(config.thread_count);

    for (size_t i = 0; i < hash_map.Size(); i++) {
        hash_map.Put(i, 0);
    }

    for (size_t i = 0; i < config.thread_count; i++) {
        threads.emplace_back(
                UpdateWorker,
                i,
                std::ref(hash_map),
                config.OperationsPerThread(),
                keys.data() + i * config.OperationsPerThread(),
                coins.data() + i * config.OperationsPerThread(),
                std::ref(epochs[i])
        );
    }

    for (auto &t : threads) t.join();

    cout << "Thread Count:    " << config.thread_count << endl;
    cout << "Total Operation: " << config.operations << endl;
    cout << "Hash Range:      " << config.hash_size << endl;

    for (size_t i = 0; i < config.thread_count; i++) {
        cout << "Thread " << i << ":        " << epochs[i] << endl;
    }

    size_t average = std::accumulate(epochs.begin(), epochs.end(), 0ull) / epochs.size();
    cout << "Average Time:    " << average << endl;
    cout << "Throughput:      " << (double) config.operations / (double) average << "mop/s" << endl;

    bool check = hash_map.Check();
    cout << "Check Result:    " << std::boolalpha << check << endl;
}
