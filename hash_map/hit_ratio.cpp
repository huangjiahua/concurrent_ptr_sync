//
// Created by jiahua on 2019/11/16.
//

#include "general_bench.h"
#include "heavyhitter/GeneralLazySS.h"
#include <unordered_set>

struct HMBConfig {
    size_t thread_count = 4;
    size_t operations = kDefaultOperations;
    size_t initial_size = 0;
    size_t key_range = 0;
    double read_ratio = 1.0;
    size_t max_depth = 20;
    double zipf_factor = 0.99;
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
            } else if (arg == "--zipf") {
                if (i + 1 > argc) {
                    Panic("param error");
                }
                i++;
                auto s = std::string(argv[i]);
                zipf_factor = std::stod(s);
            } else {
                Panic("param error");
            }
        }
    }
};

int main(int argc, const char *argv[]) {
    const size_t total = 20000000;
    HMBConfig config;
    config.LoadConfig(argc, argv);
    GeneralLazySS<uint64_t> ss(0.0001);
    RandomGenerator rng;
    for (size_t i = 0; i < 5000000; i++) {
        ss.put(rng.GenZipf<uint64_t>(1000000000ull, config.zipf_factor));
    }

    std::unordered_set<uint64_t> set;
    auto p = ss.output(true);
    for (size_t i = 0; i < 65536; i++) {
        if (p[i].getItem() != std::numeric_limits<uint64_t>::max()) {
            set.insert(p[i].getItem());
        }
    }

    size_t hit = 0;
    for (size_t i = 0; i < total; i++) {
        auto n = rng.GenZipf<uint64_t>(1000000000ull, config.zipf_factor);
        if (set.find(n) != set.end()) hit++;
    }

    std::cout << (double) hit / (double) total * 100 << "%" << std::endl;
}

