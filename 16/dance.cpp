#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cassert>

void spin(std::string& p, int n) {
    std::rotate(p.rbegin(), p.rbegin() + n, p.rend());
}

void exchange(std::string& p, int a, int b) {
    std::swap(p[a], p[b]);
}

void partner(std::string& p, char a, char b) {
    auto ait = find(begin(p), end(p), a);
    auto bit = find(begin(p), end(p), b);
    std::iter_swap(ait, bit);
}

std::string final_order(std::string programs, const std::string& line) {
    char c, tmp, a, b;
    int pa, pb;

    std::stringstream ss(line);
    while (ss >> c) {
        if (c == 's') {
            ss >> pa;
            spin(programs, pa);
        }
        else if (c == 'x') {
            ss >> pa >> tmp >> pb;
            exchange(programs, pa, pb);
        }
        else if (c == 'p') {
            ss >> a >> tmp >> b;
            partner(programs, a, b);
        }
        ss >> tmp;
    }

    return programs;
}

//------------------------------------------------------------------------------
#define ASSERT_EQ(a, e) { \
    std::cout << #a << " == " << (a)  << " == " << (e) << std::endl; \
    assert((a == e)); }

void test() {
    std::string programs = "abcde";
    std::string dance = "s1,x3/4,pe/b";
    ASSERT_EQ(final_order(programs, dance), "baedc");
}

void run() {
    std::string programs = "abcdefghijklmnop";
    std::string line;
    getline(std::cin, line);
    std::cout << final_order(programs, line) << "\n";
}

int main() {
    test();
    run();
}

