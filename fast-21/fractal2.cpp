#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <unordered_map>
#include <cmath>
#include <chrono>
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

long long t_get_size = 0;
int get_size(const std::string& drawing) {
    auto t1 = std::chrono::high_resolution_clock::now();
    auto r = round(sqrt(drawing.size()));
    auto t2 = std::chrono::high_resolution_clock::now();
    t_get_size += std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    return r;
}

std::string rotate180(std::string drawing) {
    reverse(begin(drawing), end(drawing));
    return drawing;
}

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

long long t_get_keys = 0;
std::vector<std::string> get_keys(std::string d) {
    auto t1 = std::chrono::high_resolution_clock::now();
    std::vector<std::string> keys;
    keys.reserve(8);

    keys.push_back(d);
    keys.push_back(rotate180(d));
    d = rotate(d);
    keys.push_back(d);
    keys.push_back(rotate180(d));

    d = flip(d);

    keys.push_back(d);
    keys.push_back(rotate180(d));
    d = rotate(d);
    keys.push_back(d);
    keys.push_back(rotate180(d));
    auto t2 = std::chrono::high_resolution_clock::now();
    t_get_keys += std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    return keys;
}

long long t_split = 0;
long long t_split_empty = 0;
long long t_split_populate = 0;
std::vector<std::string> split(const std::string& d, size_t N) {
    auto t1 = std::chrono::high_resolution_clock::now();
    int dN = get_size(d);
    int nparts = dN / N;
    if (nparts == 1) {
        return {d};
    }

    auto t3 = std::chrono::high_resolution_clock::now();
    std::vector<std::string> result(nparts * nparts);
    auto t4 = std::chrono::high_resolution_clock::now();
    t_split_empty += std::chrono::duration_cast<std::chrono::microseconds>(t4 - t3).count();

    for (int n = 0; n < nparts * nparts; ++n) {
        result[n].reserve(N * N);
        for (int i = n / nparts * (dN * N) + n % nparts * N, times = N; times--; i += dN) {
            result[n] += d.substr(i, N);
        }
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    t_split += std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    t_split_populate += std::chrono::duration_cast<std::chrono::microseconds>(t2 - t3).count();
    return result;
}

long long t_combine = 0;
std::string combine(const std::vector<std::string>& drawings, int nparts) {
    auto t1 = std::chrono::high_resolution_clock::now();
    if (nparts == 1) return drawings[0];

    std::ostringstream combined;
    int dN = get_size(drawings[0]);
    for (int i = 0; i < nparts * dN; ++i) {
        int ii = i % dN;
        for (int n = i / dN * nparts, times = nparts; times--; ++n) {
            combined << drawings[n].substr(ii*dN, dN);
        }
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    t_combine += std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    return combined.str();
}

long long t_get_split_size = 0;
int get_split_size(int size, int divBy) {
    auto t1 = std::chrono::high_resolution_clock::now();
    if (size == 3) return 3;
    int result = divBy;

    while (size % result != 0 || (size / result) % divBy != 0) ++result;

    auto t2 = std::chrono::high_resolution_clock::now();
    t_get_split_size += std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    return size / result;
}

long long t_rulebook_find = 0;
long long t_rulebook_insert = 0;
std::string iteration(Rulebook& rulebook, std::string in, int divBy) {
    const int inSize = get_size(in);
    int splitSize = get_split_size(inSize, divBy);

    auto s = split(in, splitSize);

    for (auto& d: s) {
        auto t1 = std::chrono::high_resolution_clock::now();
        const auto ruleIt = rulebook.find(d);
        auto t2 = std::chrono::high_resolution_clock::now();
        t_rulebook_find += std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

        if (ruleIt != end(rulebook)) {
            d = ruleIt->second;
        }
        else {
            d = iteration(rulebook, std::move(d), divBy);
        }
    };

    const auto combined = combine(s, inSize / splitSize);
    auto t1 = std::chrono::high_resolution_clock::now();
    rulebook.insert({std::move(in), combined});
    auto t2 = std::chrono::high_resolution_clock::now();
    t_rulebook_insert += std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
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
    auto t1 = std::chrono::high_resolution_clock::now();
    auto rules = read_rule_book(std::cin);
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "Reading took "
              << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << "us.\n";

    auto t3 = std::chrono::high_resolution_clock::now();
    auto drawing = iterate(rules, 18);
    auto t4 = std::chrono::high_resolution_clock::now();
    std::cout << "Working took "
              << std::chrono::duration_cast<std::chrono::microseconds>(t4 - t3).count() << "us.\n";

    std::cout << "Out of which: \n"
              << "t_get_size=" << t_get_size << "\n"
              << "t_get_keys=" << t_get_keys << "\n"
              << "t_split=" << t_split << "\n"
              << "t_split_empty=" << t_split_empty << "\n"
              << "t_split_populate=" << t_split_populate << "\n"
              << "t_combine=" << t_combine << "\n"
              << "t_get_split_size=" << t_get_split_size << "\n"
              << "t_rulebook_find=" << t_rulebook_find << "\n"
              << "t_rulebook_insert=" << t_rulebook_insert << "\n";
    std::cout << num_pixels(drawing) << "\n";
}

int main() {
    test();
    run();
}

