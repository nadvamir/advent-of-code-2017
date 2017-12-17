#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cassert>

struct CircularList {
    std::vector<int> data;
    int position {0};

    void move(int n) {
        if (!data.empty()) {
            position = (position + n) % data.size();
        }
    }

    void insert(int val) {
        if (data.empty()) {
            data.push_back(val);
        }
        else {
            data.insert(begin(data) + position + 1, val);
            move(1);
        }
    }
};

int position_after(int N, int step) {
    CircularList cl;
    cl.insert(0);

    for (int i = 1; i <= N; ++i) {
        cl.move(step);
        cl.insert(i);
    }

    cl.move(1);
    return cl.data[cl.position];
}

//------------------------------------------------------------------------------
#define ASSERT_EQ(a, e) { \
    std::cout << #a << " == " << (a)  << " == " << (e) << std::endl; \
    assert((a == e)); }

void test() {
    CircularList cl;
    ASSERT_EQ(cl.position, 0);
    cl.insert(1);
    cl.insert(2);
    cl.insert(3);
    ASSERT_EQ(cl.data[1], 2);
    ASSERT_EQ(cl.data.size(), 3);

    cl.move(1);
    ASSERT_EQ(cl.position, 0);
    cl.insert(4);
    ASSERT_EQ(cl.data[1], 4);
    ASSERT_EQ(cl.data.size(), 4);

    ASSERT_EQ(position_after(1, 3), 0);
    ASSERT_EQ(position_after(3, 3), 1);
    ASSERT_EQ(position_after(9, 3), 5);
    ASSERT_EQ(position_after(2017, 3), 638);
}

void run() {
    int step;
    std::cin >> step;
    std::cout << position_after(2017, step) << "\n";
}

int main() {
    test();
    run();
}

