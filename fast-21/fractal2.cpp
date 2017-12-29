#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <unordered_map>
#include <cmath>
#include <cassert>

using Rulebook = std::unordered_map<std::string, std::string>;
using Drawing = std::vector<std::string>;

std::vector<std::string> get_keys(std::string);

std::string remove_slashes(std::string inputD) {
    inputD.erase(remove(begin(inputD), end(inputD), '/'), end(inputD));
    return inputD;
}

Rulebook read_rule_book(std::istream& is) {
    Rulebook rulebook;
    std::string rule, sep, result;
    while (is >> rule >> sep >> result) {
        // pre-generate all rules
        result = remove_slashes(result);
        for (const auto& rotatedRule: get_keys(remove_slashes(rule))) {
            rulebook.insert({rotatedRule, result});
        }
    }
    return rulebook;
}

int get_size(const std::string& drawing) {
    return round(sqrt(drawing.size()));
}

// rotate 180
/* std::string mirror(std::string drawing) { */
/*     reverse(begin(drawing), end(drawing)); */
/*     return drawing; */
/* } */
std::string flip(std::string d) {
    std::string f = d;
    int N = get_size(d);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            f[(N - i - 1) * N + j] = d[i * N + j];
        }
    }
    return f;
}


std::string rotate(std::string d) {
    std::string r = d;
    int N = get_size(d);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            r[j * N + N - i - 1] = d[i * N + j];
        }
    }
    return r;
}

std::vector<std::string> get_keys(std::string d) {
    std::vector<std::string> keys;

    keys.push_back(d);
    d = rotate(d);
    keys.push_back(d);
    d = rotate(d);
    keys.push_back(d);
    d = rotate(d);
    keys.push_back(d);

    d = rotate(d); // back to start
    d = flip(d);
    keys.push_back(d);
    d = rotate(d);
    keys.push_back(d);
    d = rotate(d);
    keys.push_back(d);
    d = rotate(d);
    keys.push_back(d);
    return keys;
}

std::vector<std::string> split(const std::string& d, size_t N) {
    int dN = get_size(d);
    int nparts = dN / N;
    if (nparts == 1) {
        return {d};
    }

    std::vector<std::string> result;
    
    // create nparts empty grids
    for (int i = 0; i < nparts * nparts; ++i) {
        result.push_back(std::string(N * N, '.'));
    }

    // populate the grid
    for (int i = 0; i < dN; ++i) {
        for (int j = 0; j < dN; ++j) {
            int n = i / N * nparts + j / N;
            result[n][i % N * N + j % N] = d[i * dN + j];
        }
    }
    return result;
}

std::string get_line(const std::vector<std::string>& drawings, int lineN, int nparts) {
    std::ostringstream line;
    int dN = get_size(drawings[0]);
    int i = lineN % dN;
    for (int n = lineN / dN * nparts; nparts--; ++n) {
        line << drawings[n].substr(i*dN, dN);
    }
    return line.str();
}

std::string combine(const std::vector<std::string>& drawings, int nparts) {
    if (nparts == 1) return drawings[0];

    std::ostringstream hashed;
    int dN = get_size(drawings[0]);
    for (int i = 0; i < nparts * dN; ++i) {
        auto line = get_line(drawings, i, nparts);
        hashed << line;
    }
    return hashed.str();
}

int get_split_size(int size, int divBy) {
    if (size == 3) return 3;
    int result = divBy;

    while (size % result != 0 || (size / result) % divBy != 0) ++result;

    return size / result;
}

std::string iteration(Rulebook& rulebook, std::string in, int divBy) {
    const int inSize = get_size(in);
    int splitSize = get_split_size(inSize, divBy);

    auto s = split(in, splitSize);
    transform(begin(s), end(s), begin(s), [&](const auto& d) {
            const auto ruleIt = rulebook.find(d);
            if (ruleIt != end(rulebook)) {
                return ruleIt->second;
            }
            else {
                return iteration(rulebook, d, divBy);
            }
    });

    const auto combined = combine(s, inSize / splitSize);
    rulebook.insert({in, combined});
    return combined;
}

std::string iterate(Rulebook r, size_t n) {
    std::string d = ".#...####";
    while (n--) {
        int divBy = d.size() % 2 == 0 ? 2 : 3;
        d = iteration(r, std::move(d), divBy);
    }
    return d;
}

size_t num_pixels(const std::string& drawing) {
    return count(begin(drawing), end(drawing), '#');
}

//------------------------------------------------------------------------------
#define ASSERT_EQ(a, e) { \
    std::cout << #a << " == " << (a)  << " == " << (e) << std::endl; \
    assert((a == e)); }

void test() {
    ASSERT_EQ(remove_slashes("../.#"), "...#");
    ASSERT_EQ(get_size(".#.....#."), 3);
    /* ASSERT_EQ(mirror(".##....#."), ".#....##."); */
    ASSERT_EQ(flip(".##....#."), ".#.....##");
    ASSERT_EQ(rotate(".#...####"), "#..#.###.");

    auto splitDrawings = split("#####.......####", 2);
    ASSERT_EQ(splitDrawings.size(), 4);
    ASSERT_EQ(splitDrawings[0], "###.");
    ASSERT_EQ(splitDrawings[1], "##..");
    ASSERT_EQ(splitDrawings[2], "..##");
    ASSERT_EQ(splitDrawings[3], "..##");

    ASSERT_EQ(combine(split("#..#........#..#", 2), 2), "#..#........#..#");
    ASSERT_EQ(combine(split("##.##.........#..###..#.....#...###.", 2), 3),
                            "##.##.........#..###..#.....#...###.");
    ASSERT_EQ(combine(split("####............###.##.....#...#.##.", 2), 3),
                            "####............###.##.....#...#.##.");

    std::istringstream ruleInput(
            "../.# => ##./#../...\n"
            ".#./..#/### => #..#/..../..../#..#\n"
            "##/.# => .../.#./...\n"
            "../.. => .../.../...\n"
            "##/.. => .../.../...");
    auto rules = read_rule_book(ruleInput);
    ASSERT_EQ(rules.size(), 21);

    ASSERT_EQ(get_split_size(12, 2) % 2, 0);
    ASSERT_EQ(get_split_size(81, 3) % 3, 0);
    auto largest_divisor = [](int n, int divisor, int step) {
        for (divisor = divisor + step; divisor < n; divisor += step) {
            if (n % divisor == 0) return false;
        }
        return true;
    };
    ASSERT_EQ((largest_divisor(81, get_split_size(81, 3), 3)), true);
    ASSERT_EQ((largest_divisor(54, get_split_size(54, 2), 2)), true);
    ASSERT_EQ((largest_divisor(18, get_split_size(18, 2), 2)), true);

    auto drawing = iterate(rules, 2);
    ASSERT_EQ(drawing.size(), 6*6);

    auto npixels = num_pixels(drawing);
    ASSERT_EQ(npixels, 12);
}

void run() {
    auto rules = read_rule_book(std::cin);
    auto drawing = iterate(rules, 18);
    std::cout << num_pixels(drawing) << "\n";
}

int main() {
    test();
    run();
}

