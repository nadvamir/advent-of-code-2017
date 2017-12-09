#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cassert>

int clean_garbage(std::string line) {
    int removed = 0;
    bool inGarbage = false;
    for (auto it = begin(line); it != end(line); ++it) {
        if (!inGarbage && *it == '<') inGarbage = true;
        else if (*it == '>') inGarbage = false;
        else if (*it == '!') ++it;
        else if (inGarbage) ++removed;
    }
    return removed;
}

//------------------------------------------------------------------------------
#define ASSERT_EQ(a, e) { \
    std::cout << #a << " == " << (a)  << " == " << (e) << std::endl; \
    assert((a == e)); }

void test() {
    ASSERT_EQ(clean_garbage("<>"), 0);
    ASSERT_EQ(clean_garbage("{<>}"), 0);
    ASSERT_EQ(clean_garbage("<random characters>"), 17);
    ASSERT_EQ(clean_garbage("<<<<>"), 3);
    ASSERT_EQ(clean_garbage("<{!>}>"), 2);
    ASSERT_EQ(clean_garbage("<!!>"), 0);
    ASSERT_EQ(clean_garbage("<!!!>>"), 0);
    ASSERT_EQ(clean_garbage("<{o\"i!a,<{i<a>"), 10);
}

void run() {
    std::string line;
    getline(std::cin, line);
    std::cout << clean_garbage(line) << "\n";
}

int main() {
    test();
    run();
}

