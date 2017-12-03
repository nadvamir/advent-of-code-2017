#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <utility>
#include <cassert>

/* 17  16  15  14  13 */
/* 18   5   4   3  12 */
/* 19   6   1   2  11 */
/* 20   7   8   9  10 */
/* 21  22  23---> ... */
int circle(int number) {
    int c = 0;
    for (int edge = 1; number > edge * edge; edge += 2, ++c);
    return c;
}

int edge(int circle) {
    return 1 + circle * 2;
}

std::pair<int, int> get_coords(int number, int circle) {
    const int e = edge(circle);
    int limit = e * e;

    // bottom
    limit = limit - e + 1;
    if (number > limit) { 
        return {number - limit - circle, -circle};
    } 
    // left
    limit = limit - e + 1;
    if (number > limit) {
        return {-circle, limit - number + circle};
    }
    // top
    limit = limit - e + 1;
    if (number > limit) {
        return {limit - number + circle, circle};
    }
    // right
    limit = limit - e + 1;
    return {circle, number - limit - circle};
}

int memory_distance(int number) {
    const auto coords = get_coords(number, circle(number));
    return abs(coords.first) + abs(coords.second);
}

int coord_to_num(const std::pair<int, int>& coord) {
    const int circle = std::max(abs(coord.first), abs(coord.second));
    const int e = edge(circle);
    int limit = e * e;
    // bottom
    limit -= e - 1;
    if (coord.second == -circle) return limit + coord.first + circle;
    // left
    limit -= e - 1;
    if (coord.first == -circle) return limit - coord.second + circle;
    // top
    limit -= e - 1;
    if (coord.second == circle) return limit - coord.first + circle;
    // bottom
    limit -= e - 1;
    return limit + coord.second + circle;
}

std::vector<int> values(1000, 0); // for memoisation
int cell_value(int number) {
    if (number == 1) return 1;
    if (values[number]) return values[number];

    const auto coord = get_coords(number, circle(number));
    const std::vector<std::pair<int, int>> neighbours = {
        {coord.first - 1, coord.second - 1},
        {coord.first - 1, coord.second},
        {coord.first - 1, coord.second + 1},
        {coord.first, coord.second + 1},
        {coord.first + 1, coord.second + 1},
        {coord.first + 1, coord.second},
        {coord.first + 1, coord.second - 1},
        {coord.first, coord.second - 1},
    };

    int sum = 0;
    for (const auto& n: neighbours) {
        const int nn = coord_to_num(n);
        if (nn < number) sum += cell_value(nn);
    }
    values[number] = sum;
    return sum;
}

//------------------------------------------------------------------------------
#define ASSERT_EQ(a, e) { \
    assert(((a) == (e))); }

void test() {
    ASSERT_EQ(circle(1), 0);
    ASSERT_EQ(circle(2), 1);
    ASSERT_EQ(circle(9), 1);
    ASSERT_EQ(circle(10), 2);

    ASSERT_EQ(edge(0), 1);
    ASSERT_EQ(edge(1), 3);
    ASSERT_EQ(edge(2), 5);

    ASSERT_EQ(get_coords(2, 1), (std::pair<int, int>(1, 0)));
    ASSERT_EQ(get_coords(3, 1), (std::pair<int, int>(1, 1)));
    ASSERT_EQ(get_coords(16, 2), (std::pair<int, int>(-1, 2)));
    ASSERT_EQ(get_coords(18, 2), (std::pair<int, int>(-2, 1)));
    ASSERT_EQ(get_coords(24, 2), (std::pair<int, int>(1, -2)));

    ASSERT_EQ(memory_distance(1), 0);
    ASSERT_EQ(memory_distance(5), 2);
    ASSERT_EQ(memory_distance(6), 1);
    ASSERT_EQ(memory_distance(13), 4);

    ASSERT_EQ(coord_to_num(std::pair<int, int>(1, 0)), 2);
    ASSERT_EQ(coord_to_num(std::pair<int, int>(-1, 2)), 16);
    ASSERT_EQ(coord_to_num(std::pair<int, int>(-2, 1)), 18);
    ASSERT_EQ(coord_to_num(std::pair<int, int>(1, -2)), 24);

    for (int i = 1; i < 1000; ++i) {
        ASSERT_EQ(coord_to_num(get_coords(i, circle(i))), i);
    }

    ASSERT_EQ(cell_value(1), 1);
    ASSERT_EQ(cell_value(2), 1);
    ASSERT_EQ(cell_value(6), 10);
    ASSERT_EQ(cell_value(17), 147);
}

void run() {
    int location;
    std::cin >> location;
    int num = 1; while (cell_value(num) < location) ++num;
    std::cout << cell_value(num) << "\n";
}

int main() {
    test();
    run();
}

