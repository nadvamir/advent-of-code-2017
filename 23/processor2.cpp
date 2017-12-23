#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cassert>

bool is_prime(int N) {
    for (int d = 2; d * d <= N; ++d) {
        if (N % d == 0) {
            return false;
        }
    }
    return true;
}

int num_not_prime(int from, int to, int step) {
    int N = 0;
    for (; from <= to; from += step) {
        if (!is_prime(from)) {
            ++N;
        }
    }
    return N;
}

//------------------------------------------------------------------------------
#define ASSERT_EQ(a, e) { \
    std::cout << #a << " == " << (a)  << " == " << (e) << std::endl; \
    assert((a == e)); }

void test() {
    ASSERT_EQ(num_not_prime(57, 57, 17), 1);
    ASSERT_EQ(num_not_prime(105700, 122700, 17), 915);
}

void run() {
    int from, to, step;
    std::cin >> from >> to >> step;
    std::cout << num_not_prime(from, to, step) << "\n";
}

int main() {
    test();
    run();
}

