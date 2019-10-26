//
// Created by jiahua on 2019/10/24.
//
#include "hash_map/concurrent_hash_map.h"
#include "general_bench.h"
#include <vector>

constexpr static size_t kDefaultInitSize = 65536;
constexpr static size_t kDefaultKeyRange = 10000000;

struct HMBConfig {
    size_t thread_count = 12;
    size_t operations = kDefaultOperations;
    size_t initial_size = kDefaultInitSize;
    size_t key_range = kDefaultKeyRange;
    double read_ratio = 0.9;
    size_t max_depth = 20;
    bool only_tp = false;

    void LoadConfig(int argc, const char *argv[]) {
        for (int i = 1; i < argc; i++) {
            auto arg = std::string(argv[i]);
            if (arg == "--thread") {
                if (i + 1 >= argc) {
                    Panic("param error");
                }
                i++;
                auto s = std::string(argv[i]);
                thread_count = std::stoull(s);
            } else if (arg == "--operations") {
                if (i + 1 >= argc) {
                    Panic("param error");
                }
                i++;
                auto s = std::string(argv[i]);
                operations = std::stoull(s);
            } else if (arg == "--keyrange") {
                if (i + 1 >= argc) {
                    Panic("param error");
                }
                i++;
                auto s = std::string(argv[i]);
                key_range = std::stoull(s);
            } else if (arg == "--read") {
                if (i + 1 > argc) {
                    Panic("param error");
                }
                i++;
                auto s = std::string(argv[i]);
                read_ratio = std::stod(s);
                if (!(read_ratio >= 0.0 && read_ratio <= 1.0)) {
                    Panic("param error");
                }
            } else if (arg == "--onlytp") {
                if (i + 1 > argc) {
                    Panic("param error");
                }
                i++;
                only_tp = true;
            } else {
                Panic("param error");
            }
        }
    }
};

using namespace std;
using Map = ConcurrentHashMap<uint64_t, uint64_t, std::hash<uint64_t>, std::equal_to<>>;

constexpr static size_t kROUND = 10;
constexpr static size_t kFtRootSize = 65536;
constexpr static size_t kFtRange = 10000;

int main(int argc, const char *argv[]) {
    HMBConfig config;
    config.LoadConfig(argc, argv);
    RandomGenerator rng;
    size_t per_thread_task = config.operations / config.thread_count;

    ConcurrentHashMap<uint64_t, uint64_t, std::hash<uint64_t>, std::equal_to<>>
            map(config.initial_size, config.max_depth);
    ConcurrentHashMap<uint64_t, uint64_t, std::hash<uint64_t>, std::equal_to<>>
            ft(kFtRootSize, 20);

    for (size_t i = 0; i < config.operations; i++) {
        map.Insert(rng.GenZipf<uint64_t>(1000000000ull, 1.5), 0);
    }

    vector<uint64_t> keys(config.operations + 1000);
    for (auto &key : keys) key = rng.GenZipf<uint64_t>(1000000000ull, 1.5);
    vector<int> coins(config.operations + 1000);
    for (auto &coin: coins) coin = rng.FlipCoin(config.read_ratio);
    vector<thread> threads(config.thread_count);
    vector<size_t> times(config.thread_count, 0);

    auto worker = [per_thread_task](size_t idx, Map &map, Map &ft,
                                    uint64_t *keys, int *coins, size_t &time) {
        auto t = SystemTime::Now();
        uint64_t value = 0;
        for (size_t i = 0; i < kROUND; i++) {
            for (size_t j = 0; j < per_thread_task; j++) {
                uint64_t key = keys[j];
                Map *curr = key < 60000 ? &ft : &map;
                if (coins[j]) {
                    curr->Find(key, value);
                } else {
                    curr->Insert(key, key + idx);
                }
            }
        }
        time = SystemTime::Now().DurationSince<std::chrono::microseconds>(t);
    };

    for (size_t i = 0; i < threads.size(); i++) {
        threads[i] = thread(worker, i, std::ref(map), std::ref(ft),
                            keys.data() + i * per_thread_task, coins.data() + i * per_thread_task,
                            std::ref(times[i]));
    }

    for (auto &t : threads) t.join();

    size_t average_time = std::accumulate(times.begin(), times.end(), 0ull) / times.size();
    double tp = (double) config.operations * (double) kROUND / (double) average_time;

    cout << tp << endl;

    return 0;
}

