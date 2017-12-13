#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <unordered_map>
#include <cassert>

std::unordered_map<int, int> read_walls(std::istream& is) {
    std::unordered_map<int, int> walls;
    int distance, size; char colon;
    while (is >> distance >> colon >> size) {
        walls[distance] = size;
    }
    return walls;
}

int cost_at_a_location(int distance, int size) {
    if (size == 0) return 0;
    int pos = distance % ((size - 1) * 2);
    if (pos >= size) pos = size - (pos - size);
    return pos == 0 ? size * distance : 0;
}

int cost_of_caught(const std::unordered_map<int, int>& walls) {
    int cost = 0;
    for (const auto& [distance, size]: walls) {
        cost += cost_at_a_location(distance, size);
    }
    return cost;
}

//------------------------------------------------------------------------------
#define ASSERT_EQ(a, e) { \
    std::cout << #a << " == " << (a)  << " == " << (e) << std::endl; \
    assert((a == e)); }

void test() {
    const std::string input = "0: 3\n1: 2\n4: 4\n6: 4";
    std::istringstream iss(input);
    auto walls = read_walls(iss);
    ASSERT_EQ(walls[0], 3);
    ASSERT_EQ(walls[1], 2);
    ASSERT_EQ(walls[2], 0);
    ASSERT_EQ(walls[4], 4);
    ASSERT_EQ(walls[6], 4);

    ASSERT_EQ(cost_at_a_location(0, 3), 0);
    ASSERT_EQ(cost_at_a_location(2, 2), 4);
    ASSERT_EQ(cost_at_a_location(4, 4), 0);
    ASSERT_EQ(cost_at_a_location(6, 4), 24);

    ASSERT_EQ(cost_of_caught(walls), 24);
}

void run() {
    std::string line;
    const auto walls = read_walls(std::cin);
    std::cout << cost_of_caught(walls) << "\n";
}

int main() {
    test();
    run();
}

