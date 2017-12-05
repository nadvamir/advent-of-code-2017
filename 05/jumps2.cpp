#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cassert>

size_t num_steps(std::vector<int> jumps) {
    size_t i = 0;
    size_t steps = 0;
    while (i < jumps.size()) {
        i += (jumps[i] >= 3) ? jumps[i]-- : jumps[i]++;
        ++steps;
    }
    return steps;
}

//------------------------------------------------------------------------------
#define ASSERT_EQ(a, e) { \
    std::cout << #a << " == " << (a)  << " == " << (e) << std::endl; \
    assert((a == e)); }

void test() {
    ASSERT_EQ(num_steps({0, 3, 0, 1, -3}), 10);
}

void run() {
    std::vector<int> jumps;
    copy(std::istream_iterator<int>(std::cin), std::istream_iterator<int>(), back_inserter(jumps));
    std::cout << num_steps(jumps) << "\n";
}

int main() {
    test();
    run();
}

