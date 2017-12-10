#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <regex>
#include <cassert>

std::vector<int> line2vec(const std::string& line) {
    std::vector<int> nums;
    std::regex numRx {"\\d+"};
    std::sregex_token_iterator matches(begin(line), end(line), numRx);
    transform(matches, std::sregex_token_iterator(), back_inserter(nums), [](const auto& match) {
            return stoi(match);
    });
    return nums;
}

void reverse(std::vector<int>& nums, size_t pos, size_t length) {
    size_t N = nums.size();
    auto next = [&](size_t i) { return (i + 1) % N; };
    auto prev = [&](size_t i) { return (i == 0) ? N - 1 : (i - 1); };
    for (size_t i = pos, j = (pos + length - 1) % N, iters = length / 2;
         iters--;
         i = next(i), j = prev(j))
    {
        std::swap(nums[i], nums[j]);
    }
}

int get_hash(std::vector<int> input, const std::vector<int>& sizes) {
    size_t pos = 0, skip = 0;
    for (auto s: sizes) {
        reverse(input, pos, s);
        pos = (pos + s + skip) % input.size();
        ++skip;
    }
    return input[0] * input[1];
}

//------------------------------------------------------------------------------
#define ASSERT_EQ(a, e) { \
    assert((a == e)); }

void test() {
    auto sizes = line2vec("3,4,1,5");
    std::vector<int> expected {3, 4, 1, 5};
    ASSERT_EQ(sizes, expected);

    std::vector<int> input       {0, 1, 2, 3, 4};
    std::vector<int> transformed {3, 1, 2, 0, 4};
    reverse(input, 3, 3);
    ASSERT_EQ(input, transformed);

    std::vector<int> input2       {0, 1, 2, 3, 4};
    std::vector<int> transformed2 {4, 3, 2, 1, 0};
    reverse(input2, 3, 4);
    ASSERT_EQ(input2, transformed2);

    std::vector<int> hash_input {0, 1, 2, 3, 4};
    ASSERT_EQ(get_hash(hash_input, sizes), 12);
}

void run() {
    std::string line;
    getline(std::cin, line);
    auto sizes = line2vec(line);
    std::vector<int> input(256);
    iota(begin(input), end(input), 0);
    std::cout << get_hash(input, sizes) << "\n";
}

int main() {
    test();
    run();
}

