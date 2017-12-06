#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <unordered_set>
#include <cassert>

std::string hash(const std::vector<int>& numbers) {
    std::ostringstream oss;
    copy(begin(numbers), end(numbers), std::ostream_iterator<int>(oss, "|"));
    return oss.str();
}

void redistribute(std::vector<int>& banks) {
    auto largest = max_element(begin(banks), end(banks));
    int value = *largest;
    *largest = 0;

    auto next = [&](int i) { return (i + 1) % banks.size(); };
    for (int i = next(largest - begin(banks)); value--; ++banks[i], i = next(i));
}

size_t num_cycles(const std::string line) {
    std::istringstream iss(line);
    std::vector<int> numbers;
    copy(std::istream_iterator<int>(iss), std::istream_iterator<int>(), back_inserter(numbers));

    std::unordered_set<std::string> visited;
    size_t cycle = 0;
    do {
        visited.insert(hash(numbers));
        redistribute(numbers);
        ++cycle;
    }
    while (visited.count(hash(numbers)) == 0);

    return cycle;
}

//------------------------------------------------------------------------------
#define ASSERT_EQ(a, e) { \
    std::cout << #a << " == " << (a)  << " == " << (e) << std::endl; \
    assert((a == e)); }

void test() {
    ASSERT_EQ(hash({0, 2, 7, 0}), "0|2|7|0|");

    std::vector<int> banks {0, 2, 7, 0};
    redistribute(banks);
    ASSERT_EQ(hash(banks), "2|4|1|2|");

    ASSERT_EQ(num_cycles("0 2 7 0"), 5);
}

void run() {
    std::string line;
    getline(std::cin, line);
    std::cout << num_cycles(line) << "\n";
}

int main() {
    test();
    run();
}

