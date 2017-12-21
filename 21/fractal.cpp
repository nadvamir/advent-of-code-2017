#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <unordered_map>
#include <cassert>

using Rulebook = std::unordered_map<std::string, std::string>;
using Drawing = std::vector<std::string>;

Rulebook read_rule_book(std::istream& is) {
    Rulebook rulebook;
    std::string rule, sep, result;
    while (is >> rule >> sep >> result) {
        rulebook.insert({rule, result});
    }
    return rulebook;
}

Drawing to_drawing(const std::string& condensed) {
    Drawing d;
    auto it = begin(condensed);
    while (it != end(condensed)) {
        auto next = find(it, end(condensed), '/');
        std::string line(it, next);
        d.push_back(line);
        it = (next != end(condensed)) ? next + 1 : next;
    }
    return d;
}

std::string to_string(const Drawing& d) {
    std::ostringstream ss;
    for (auto it = begin(d); it != end(d); ++it) {
        if (it == begin(d)) ss << *it;
        else ss << '/' << *it;
    }
    return ss.str();
}

Drawing empty(size_t N) {
    Drawing d(N);
    for (auto& line: d) {
        line = std::string(N, '.');
    }
    return d;
}

std::vector<Drawing> split(const Drawing& d, size_t N) {
    int nparts = d.size() / N;
    if (nparts == 1) {
        return {d};
    }

    std::vector<Drawing> result;
    
    // create nparts empty grids
    for (int i = 0; i < nparts * nparts; ++i) {
        result.push_back(empty(N));
    }

    // populate the grid
    for (int i = 0; i < d.size(); ++i) {
        for (int j = 0; j < d[i].size(); ++j) {
            int n = i / N * nparts + j / N;
            result[n][i % N][j % N] = d[i][j];
        }
    }
    return result;
}

std::string get_line(const std::vector<Drawing>& drawings, int lineN, int nparts) {
    std::ostringstream line;
    int i = lineN % drawings[0].size();
    for (int n = lineN / drawings[0].size() * nparts; nparts--; ++n) {
        line << drawings[n][i];
    }
    return line.str();
}

Drawing combine(const std::vector<Drawing>& drawings, int nparts) {
    if (nparts == 1) return drawings[0];

    std::ostringstream hashed;
    for (int i = 0; i < nparts * drawings[0].size(); ++i) {
        auto line = get_line(drawings, i, nparts);
        if (i == 0) hashed << line;
        else hashed << '/' << line;
    }
    return to_drawing(hashed.str());
}

Drawing rotate(Drawing d) {
    Drawing r = d;
    int N = d.size();
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            r[j][N - i - 1] = d[i][j];
        }
    }
    return r;
}

Drawing flip(Drawing d) {
    Drawing f = d;
    int N = d.size();
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            f[N - i - 1][j] = d[i][j];
        }
    }
    return f;
}

std::vector<std::string> get_keys(Drawing d) {
    std::vector<std::string> keys;

    keys.push_back(to_string(d));
    d = rotate(d);
    keys.push_back(to_string(d));
    d = rotate(d);
    keys.push_back(to_string(d));
    d = rotate(d);
    keys.push_back(to_string(d));

    d = rotate(d); // back to start
    d = flip(d);
    keys.push_back(to_string(d));
    d = rotate(d);
    keys.push_back(to_string(d));
    d = rotate(d);
    keys.push_back(to_string(d));
    d = rotate(d);
    keys.push_back(to_string(d));
    return keys;
}

Drawing iteration(const Rulebook& r, Drawing in) {
    int splitSize = (in.size() % 2 == 0) ? 2 : 3;
    auto s = split(in, splitSize);
    transform(begin(s), end(s), begin(s), [&](const auto& d) {
            const auto keys = get_keys(d);
            for (const auto& k: keys) {
                const auto ruleIt = r.find(k);
                if (ruleIt != end(r)) {
                    return to_drawing(ruleIt->second);
                }
            }
            throw "Rule not found!";
    });
    return combine(s, in.size() / splitSize);
}

Drawing iterate(const Rulebook& r, size_t n) {
    Drawing d = to_drawing(".#./..#/###");
    while (n--) d = iteration(r, std::move(d));
    return d;
}

size_t num_pixels(const Drawing& drawing) {
    size_t n = 0;
    for (const auto& l: drawing) {
        n += count(begin(l), end(l), '#');
    }
    return n;
}

//------------------------------------------------------------------------------
#define ASSERT_EQ(a, e) { \
    std::cout << #a << " == " << (a)  << " == " << (e) << std::endl; \
    assert((a == e)); }

void test() {
    std::istringstream ruleInput(
            "../.# => ##./#../...\n"
            ".#./..#/### => #..#/..../..../#..#\n"
            "##/.# => .../.#./...\n"
            "../.. => .../.../...\n"
            "##/.. => .../.../...");
    auto rules = read_rule_book(ruleInput);
    ASSERT_EQ(rules.size(), 5);

    auto d1 = to_drawing(".#/#.");
    ASSERT_EQ(d1[0][0], '.');
    ASSERT_EQ(d1[0][1], '#');
    ASSERT_EQ(d1[1][0], '#');
    ASSERT_EQ(d1[1][1], '.');

    ASSERT_EQ(to_string(d1), ".#/#.");
    ASSERT_EQ(to_string(to_drawing(".#./..#/###")), ".#./..#/###");

    auto d3 = to_drawing("#..#/..../..../#..#");
    auto s = split(d3, 2);
    ASSERT_EQ(s.size(), 4);
    ASSERT_EQ(to_string(combine(s, 2)), "#..#/..../..../#..#");

    auto rotated = to_string(rotate(to_drawing(".#./..#/###")));
    ASSERT_EQ(rotated, "#../#.#/##.");
    auto flipped = to_string(flip(to_drawing(".#./..#/###")));
    ASSERT_EQ(flipped, "###/..#/.#.");

    auto drawing = iterate(rules, 2);
    ASSERT_EQ(drawing.size(), 6);

    auto npixels = num_pixels(drawing);
    ASSERT_EQ(npixels, 12);
}

void run() {
    auto rules = read_rule_book(std::cin);
    auto drawing = iterate(rules, 5);
    std::cout << num_pixels(drawing) << "\n";
}

int main() {
    test();
    run();
}

