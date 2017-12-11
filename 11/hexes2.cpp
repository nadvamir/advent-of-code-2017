#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <regex>
#include <unordered_map>
#include <cassert>

std::vector<std::string> line2vec(const std::string& line) {
    std::vector<std::string> path;
    std::regex pathRx {"\\w+"};
    std::sregex_token_iterator matches(begin(line), end(line), pathRx);
    copy(matches, std::sregex_token_iterator(), back_inserter(path));
    return path;
}

bool cancel(int& a, int& b) {
    int m = std::min(a, b);
    a -= m; b -= m;
    return m > 0;
}

bool reduce(int& a, int& b, int& c) {
    int m = std::min(a, b);
    a -= m; b -= m; c += m;
    return m > 0;
}

void reduce_hexes(std::unordered_map<std::string, int>& hexes) {
    bool reduced = false;
    do {
        reduced = false;
        reduced |= reduce(hexes["nw"], hexes["ne"], hexes["n"]);
        reduced |= reduce(hexes["sw"], hexes["se"], hexes["s"]);

        reduced |= reduce(hexes["ne"], hexes["s"], hexes["se"]);
        reduced |= reduce(hexes["se"], hexes["n"], hexes["ne"]);
        reduced |= reduce(hexes["nw"], hexes["s"], hexes["sw"]);
        reduced |= reduce(hexes["sw"], hexes["n"], hexes["nw"]);

        reduced |= cancel(hexes["ne"], hexes["sw"]);
        reduced |= cancel(hexes["n"], hexes["s"]);
        reduced |= cancel(hexes["nw"], hexes["se"]);
    } while (reduced);
}

size_t sum_occurences(const std::unordered_map<std::string, int>& hexes) {
    size_t distance = 0;
    for (const auto& [_, occurence]: hexes) distance += occurence;
    return distance;
}

size_t get_furthest(const std::string& input) {
    const auto path = line2vec(input);
    std::unordered_map<std::string, int> hexes;

    size_t distance = 0;
    for (const auto& p: path) {
        ++hexes[p];
        reduce_hexes(hexes);
        distance = std::max(distance, sum_occurences(hexes));
    }
    return distance;
}

//------------------------------------------------------------------------------
#define ASSERT_EQ(a, e) { \
    std::cout << #a << " == " << (a) << " == " << (e) << std::endl; \
    assert((a == e)); }

void test() {
    auto hexes = line2vec("n,nw,sw,se,e");
    std::vector<std::string> expected {"n", "nw", "sw", "se", "e"};
    assert(hexes == expected);

    int a = 3, b = 4;
    cancel(a, b);
    ASSERT_EQ(a, 0);
    ASSERT_EQ(b, 1);
    a = 4, b = 3;
    cancel(a, b);
    ASSERT_EQ(a, 1);
    ASSERT_EQ(b, 0);

    int c = 0;
    a = 4, b = 3;
    reduce(a, b, c);
    ASSERT_EQ(a, 1);
    ASSERT_EQ(b, 0);
    ASSERT_EQ(c, 3);
    a = 3, b = 4, c = 1;
    reduce(a, b, c);
    ASSERT_EQ(a, 0);
    ASSERT_EQ(b, 1);
    ASSERT_EQ(c, 4);

    ASSERT_EQ(get_furthest("ne,ne,ne"), 3);
    ASSERT_EQ(get_furthest("ne,ne,sw,sw"), 2);
    ASSERT_EQ(get_furthest("ne,ne,s,s"), 2);
    ASSERT_EQ(get_furthest("se,sw,se,sw,sw"), 3);
    ASSERT_EQ(get_furthest("se,n,nw,sw,s,se,ne,nw"), 1);
}

void run() {
    std::string line;
    getline(std::cin, line);
    std::cout << get_furthest(line) << "\n";
}

int main() {
    test();
    run();
}

