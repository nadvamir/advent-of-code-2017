#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <cassert>

void spin(std::string& p, int n, int) {
    std::rotate(p.rbegin(), p.rbegin() + n, p.rend());
}

void exchange(std::string& p, int a, int b) {
    std::swap(p[a], p[b]);
}

void partner(std::string& p, int a, int b) {
    auto ait = find(begin(p), end(p), char(a));
    auto bit = find(begin(p), end(p), char(b));
    std::iter_swap(ait, bit);
}

struct Dance {
    int a;
    int b;
    void (*f)(std::string&, int, int);
};

std::vector<Dance> getDance(const std::string& line) {
    char c, tmp, a, b;
    int pa, pb;

    std::vector<Dance> dance;
    std::stringstream ss(line);
    while (ss >> c) {
        if (c == 's') {
            ss >> pa;
            dance.push_back(Dance{pa, 0, spin});
        }
        else if (c == 'x') {
            ss >> pa >> tmp >> pb;
            dance.push_back(Dance{pa, pb, exchange});
        }
        else if (c == 'p') {
            ss >> a >> tmp >> b;
            dance.push_back(Dance{a, b, partner});
        }
        ss >> tmp;
    }

    return dance;
}

std::string dance(std::string programs, const std::vector<Dance>& dance) {
    for (const auto& d: dance) {
        d.f(programs, d.a, d.b);
    }

    return programs;
}

int skip_cycles(int i, int lasti, int N) {
    int diff = i + 1 - lasti;
    int ncycles = (N - i - 2) / diff;
    int ni = i + ncycles * diff;
    if (ncycles > 0) i = ni;
    return i;
}

std::string final_order(std::string programs, const std::string& line, int N) {
    auto compiledDance = getDance(line);
    std::unordered_map<std::string, int> seen;
    for (int i = 0; i < N; ++i) {
        seen.insert({programs, i});

        programs = dance(programs, compiledDance);

        if (seen.count(programs)) {
            i = skip_cycles(i, seen[programs], N);
        }
    }
    return programs;
}

//------------------------------------------------------------------------------
#define ASSERT_EQ(a, e) { \
    std::cout << #a << " == " << (a)  << " == " << (e) << std::endl; \
    assert((a == e)); }

void test() {
    std::string p1 = "abcde";
    partner(p1, 'b', 'e');
    ASSERT_EQ(p1, "aecdb");
    p1 = "bceda";
    partner(p1, 'b', 'e');
    ASSERT_EQ(p1, "ecbda");

    ASSERT_EQ(skip_cycles(3, 0, 100), 95);
    ASSERT_EQ(skip_cycles(35, 0, 1'000'000'000), 999999971);

    std::string programs = "abcde";
    std::string dance = "s1,x3/4,pe/b";
    ASSERT_EQ(final_order(programs, dance, 2), "ceadb");
    ASSERT_EQ(final_order(programs, dance, 3), "ecbda");
    ASSERT_EQ(final_order(programs, dance, 100), "abcde");
}

void run() {
    std::string programs = "abcdefghijklmnop";
    std::string line;
    getline(std::cin, line);
    std::cout << final_order(programs, line, 1'000'000'000) << "\n";
}

int main() {
    test();
    run();
}

