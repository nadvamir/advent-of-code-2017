#include <iostream>
#include <iomanip>
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
    for (char c: line) nums.push_back(c);
    nums.push_back(17);
    nums.push_back(31);
    nums.push_back(73);
    nums.push_back(47);
    nums.push_back(23);
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

void perform_iteration(
        std::vector<int>& input,
        const std::vector<int>& sizes,
        size_t& pos,
        size_t& skip)
{
    for (auto s: sizes) {
        reverse(input, pos, s);
        pos = (pos + s + skip) % input.size();
        ++skip;
    }
}

std::string to_hex(const std::vector<int>& input) {
    std::stringstream ss;
    for (auto i: input) {
        ss << std::setfill('0') << std::setw(2) << std::hex << i;
    }
    return ss.str();
}

template <typename It>
int condense(It first, It last) {
    return accumulate(
            first + 1,
            last,
            *first,
            [](auto a, auto b) { return a ^ b; });
}

std::vector<int> to_dense_hash(const std::vector<int>& input) {
    assert(input.size() == 256);
    std::vector<int> dense;
    for (size_t i = 0; i < 256; i += 16) {
        dense.push_back(condense(begin(input) + i, begin(input) + i + 16));
    }
    return dense;
}

std::string hash(const std::string& input) {
    auto rotations = line2vec(input);
    std::vector<int> nums(256);
    iota(begin(nums), end(nums), 0);

    size_t pos = 0, skip = 0;
    for (size_t i = 0; i < 64; ++i) perform_iteration(nums, rotations, pos, skip);

    return to_hex(to_dense_hash(nums));
}

//------------------------------------------------------------------------------
#define ASSERT_EQ(a, e) { \
    assert((a == e)); }

void test() {
    auto sizes = line2vec("1,2,3");
    std::vector<int> expected {49, 44, 50, 44, 51, 17, 31, 73, 47, 23};
    ASSERT_EQ(sizes, expected);

    std::vector<int> input       {0, 1, 2, 3, 4};
    std::vector<int> transformed {3, 1, 2, 0, 4};
    reverse(input, 3, 3);
    ASSERT_EQ(input, transformed);

    std::vector<int> input2       {0, 1, 2, 3, 4};
    std::vector<int> transformed2 {4, 3, 2, 1, 0};
    reverse(input2, 3, 4);
    ASSERT_EQ(input2, transformed2);

    std::vector<int> decimal {64, 7, 255};
    ASSERT_EQ(to_hex(decimal), "4007ff");

    std::vector<int> hex_input {65, 27, 9, 1, 4, 3, 40, 50, 91, 7, 6, 0, 2, 5, 68, 22};
    ASSERT_EQ(condense(begin(hex_input), end(hex_input)), 64);

    ASSERT_EQ(hash(""), "a2582a3a0e66e6e86e3812dcb672a272");
    ASSERT_EQ(hash("AoC 2017"), "33efeb34ea91902bb2f59c9920caa6cd");
    ASSERT_EQ(hash("1,2,3"), "3efbe78a8d82f29979031a4aa0b16a9d");
    ASSERT_EQ(hash("1,2,4"), "63960835bcdc130f0b66d7ff4f6a5a8e");
}

void run() {
    std::string line;
    getline(std::cin, line);
    std::cout << hash(line) << "\n";
}

int main() {
    test();
    run();
}

