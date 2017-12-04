#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cassert>

bool is_valid(const std::string line) {
    std::istringstream iss(line);
    std::vector<std::string> passphrase;
    copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), back_inserter(passphrase));
    sort(begin(passphrase), end(passphrase));
    return adjacent_find(begin(passphrase), end(passphrase)) == end(passphrase);
}

//------------------------------------------------------------------------------
#define ASSERT_EQ(a, e) { \
    std::cout << #a << " == " << (a)  << " == " << (e) << std::endl; \
    assert((a == e)); }

void test() {
    ASSERT_EQ(is_valid("aa bb cc dd ee"), true);
    ASSERT_EQ(is_valid("aa bb cc dd aa"), false);
    ASSERT_EQ(is_valid("aa bb cc dd aaa"), true);
}

void run() {
    std::string line;
    size_t num_valid = 0;
    while (getline(std::cin, line)) {
        num_valid += is_valid(line);
    }
    std::cout << num_valid << "\n";
}

int main() {
    test();
    run();
}

