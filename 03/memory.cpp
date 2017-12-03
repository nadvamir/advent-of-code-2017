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
}

void run() {
    int location;
    std::cin >> location;
    std::cout << memory_distance(location) << "\n";
}

int main() {
    test();
    run();
}

