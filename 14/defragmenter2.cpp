#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <bitset>
#include <cassert>

//------------------------------------------------------------------------------
// Code for knot hash
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
// Exercise
std::string to_binary(const std::string hex) {
    std::stringstream ss;
    for (const char c: hex) {
        unsigned long i = (c < 'a') ? (c - '0') : (10 + c - 'a');
        ss << std::bitset<4>(i);
    }
    return ss.str();
}

std::string get_row(const std::string& input, int row) {
    auto row_in_hex = hash(input + "-" + std::to_string(row));
    return to_binary(row_in_hex);
}

std::vector<std::string> form_grid(const std::string& input) {
    std::vector<std::string> grid;
    for (int i = 0; i < 128; ++i) {
        grid.push_back(get_row(input, i));
    }
    return grid;
}

void floodfill(std::vector<std::string>& grid, int i, int j) {
    grid[i][j] = '0';
    auto step = [&grid](int i, int j) {
        if (i >= 0 && i < grid.size() && j >= 0 && j < grid[i].size() && grid[i][j] == '1') {
            floodfill(grid, i, j);
        }
    };
    step(i - 1, j);
    step(i + 1, j);
    step(i, j - 1);
    step(i, j + 1);
}

bool flood(std::vector<std::string>& grid) {
    for (size_t i = 0; i < grid.size(); ++i) {
        for (size_t j = 0; j < grid[i].size(); ++j) {
            if (grid[i][j] == '1') {
                floodfill(grid, i, j);
                return true;
            }
        }
    }
    return false;
}

size_t num_regions(const std::string& input) {
    auto grid = form_grid(input);
    size_t regions = 0;
    while (flood(grid)) ++regions;
    return regions;
}
//------------------------------------------------------------------------------
#define ASSERT_EQ(a, e) { \
    std::cout << #a << " == " << (a) << " == " << (e) << std::endl; \
    assert((a == e)); }

void test() {
    ASSERT_EQ(to_binary("a0c2017"), "1010000011000010000000010111");

    auto row0 = get_row("flqrgnkx", 0);
    auto row1 = get_row("flqrgnkx", 1);
    ASSERT_EQ(row0.substr(0, 8), "11010100");
    ASSERT_EQ(row1.substr(0, 8), "01010101");

    ASSERT_EQ(num_regions("flqrgnkx"), 1242)
}

void run() {
    std::string line;
    getline(std::cin, line);
    std::cout << num_regions(line) << "\n";
}

int main() {
    test();
    run();
}

