#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cassert>

int position_after_0(int N, int step) {
    int size = 1;
    int result = 0;
    int position = 0;

    for (int i = 1; i <= N; ++i) {
        position = (position + step) % size; // move
        if (position == 0) result = i;
        ++size; // insert
        ++position; // move 1
    }

    return result;
}

//------------------------------------------------------------------------------
#define ASSERT_EQ(a, e) { \
    std::cout << #a << " == " << (a)  << " == " << (e) << std::endl; \
    assert((a == e)); }

void test() {
    ASSERT_EQ(position_after_0(10, 3), 9);
    ASSERT_EQ(position_after_0(100, 3), 16);
    ASSERT_EQ(position_after_0(1000, 3), 517);
}

void run() {
    int step;
    std::cin >> step;
    std::cout << position_after_0(50'000'000, step) << "\n";
}

int main() {
    test();
    run();
}

