#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cassert>

int division(const std::string line) {
    std::istringstream iss(line);
    std::vector<int> numbers;
    copy(std::istream_iterator<int>(iss), std::istream_iterator<int>(), back_inserter(numbers));
    sort(begin(numbers), end(numbers), std::greater<int>());

    for (const auto num: numbers) {
        for (auto div = rbegin(numbers); num / *div > 1; ++div) {
            if (num % *div == 0) return num / *div;
        }
    }
    return 0;
}

//------------------------------------------------------------------------------
#define ASSERT_EQ(a, e) { \
    std::cout << #a << " == " << (a)  << " == " << (e) << std::endl; \
    assert((a == e)); }

void test() {
    ASSERT_EQ(division("5 9 2 8"), 4);
    ASSERT_EQ(division("9 4 7 3"), 3);
    ASSERT_EQ(division("3 8 6 5"), 2);
    ASSERT_EQ(division("2 3 5"), 0);
    ASSERT_EQ(division(""), 0);
    ASSERT_EQ(division("1"), 0);
}

void run() {
    std::string line;
    int sum = 0;
    while (getline(std::cin, line)) {
        sum += division(line);
    }
    std::cout << sum << "\n";
}

int main() {
    test();
    run();
}

