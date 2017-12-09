#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cassert>

std::string clean_garbage(std::string line) {
    std::stringstream cleaned;
    bool inGarbage = false;
    for (auto it = begin(line); it != end(line); ++it) {
        if (*it == '<') inGarbage = true;
        else if (*it == '>') inGarbage = false;
        else if (*it == '!') ++it;
        else if (!inGarbage) cleaned << *it;
    }
    return cleaned.str();
}

int get_score(const std::string& line) {
    int score = 0, level = 0;
    for (auto c: line) {
        if (c == '{') {
            ++level;
        }
        else if (c == '}') {
            score += level;
            --level;
        }
    }
    return score;
}

//------------------------------------------------------------------------------
#define ASSERT_EQ(a, e) { \
    std::cout << #a << " == " << (a)  << " == " << (e) << std::endl; \
    assert((a == e)); }

void test() {
    ASSERT_EQ(clean_garbage("<>"), "");
    ASSERT_EQ(clean_garbage("{<>}"), "{}");
    ASSERT_EQ(clean_garbage("<random characters>"), "");
    ASSERT_EQ(clean_garbage("<<<<>"), "");
    ASSERT_EQ(clean_garbage("<{!>}>"), "");
    ASSERT_EQ(clean_garbage("<!!>"), "");
    ASSERT_EQ(clean_garbage("<!!!>>"), "");
    ASSERT_EQ(clean_garbage("<{o\"i!a,<{i<a>"), "");

    ASSERT_EQ(get_score("{}"), 1);
    ASSERT_EQ(get_score("{{{}}}"), 6);
    ASSERT_EQ(get_score("{{},{}}"), 5);
    ASSERT_EQ(get_score("{{{},{},{{}}}}"), 16);
    ASSERT_EQ(get_score(clean_garbage("{<a>,<a>,<a>,<a>}")), 1);
    ASSERT_EQ(get_score(clean_garbage("{{<ab>},{<ab>},{<ab>},{<ab>}}")), 9);
    ASSERT_EQ(get_score(clean_garbage("{{<!!>},{<!!>},{<!!>},{<!!>}}")), 9);
    ASSERT_EQ(get_score(clean_garbage("{{<a!>},{<a!>},{<a!>},{<ab>}}")), 3);
}

void run() {
    std::string line;
    getline(std::cin, line);
    line = clean_garbage(line);
    std::cout << get_score(line) << "\n";
}

int main() {
    test();
    run();
}

