//
// Created by jiahua on 2019/9/29.
//
#include <string>
#include <thread>

#include "general_bench.h"

using namespace std;

constexpr size_t total = 1000000000;

int main(int argc, const char *argv[]) {
    string arg = argv[1];
    size_t thread_cnt = stoull(arg);
    vector<thread> threads(thread_cnt);
    vector<size_t> time(thread_cnt, 0);

    size_t tasks = total / thread_cnt;

    atomic<size_t> counter(0);
    size_t i = 0;
    for (auto &t : threads) {
        t = thread([tasks](size_t &time, atomic<size_t> &counter) {
            auto t1 = SystemTime::Now();
            for (size_t j = 0; j < tasks; j++) {
                counter.fetch_add(1);
            }
            time = SystemTime::Now().DurationSince<Microseconds>(t1);
        }, std::ref(time[i++]), std::ref(counter));
    }
    for (auto &t: threads) t.join();
    size_t average = std::accumulate(time.begin(), time.end(), 0ull) / time.size();

    cout << (double)total / (double)average;
}

