#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cassert>

size_t difference(const std::string line) {
    std::istringstream iss(line);
    std::vector<int> numbers;
    copy(std::istream_iterator<int>(iss), std::istream_iterator<int>(), back_inserter(numbers));
    if (numbers.size() < 2) return 0;
    return *max_element(begin(numbers), end(numbers)) - *min_element(begin(numbers), end(numbers));
}

//------------------------------------------------------------------------------
#define ASSERT_EQ(a, e) { \
    std::cout << #a << " == " << (a)  << " == " << (e) << std::endl; \
    assert((a == e)); }

void test() {
    ASSERT_EQ(difference("5 1 9 5"), 8);
    ASSERT_EQ(difference("7 5 3"), 4);
    ASSERT_EQ(difference("2 4 6 8"), 6);
    ASSERT_EQ(difference(""), 0);
    ASSERT_EQ(difference("1"), 0);
}

void run() {
    std::string line;
    size_t sum = 0;
    while (getline(std::cin, line)) {
        sum += difference(line);
    }
    std::cout << sum << "\n";
}

int main() {
    test();
    run();
}

