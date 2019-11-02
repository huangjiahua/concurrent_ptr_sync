//
// Created by jiahua on 2019/10/30.
//

#include "general_bench.h"
#include <set>
#include <iostream>

using namespace std;

int main() {
    RandomGenerator rng;
    set<size_t> s;

    for (size_t i = 0; i < 100000000; i++) {
        auto j = rng.GenZipf<size_t>(1000000000ull, 1.2);
        s.insert(j);
    }

    cout << s.size() << endl;
}

