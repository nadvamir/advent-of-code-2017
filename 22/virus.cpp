#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <tuple>
#include <cassert>

using Coord = std::pair<int64_t, int64_t>;
using Grid = std::set<Coord>;

enum class Direction {
    N, E, S, W
};

std::tuple<Grid, Coord> create_grid(std::istream& is) {
    std::vector<std::string> input;
    std::string line;
    while (getline(is, line)) {
        input.push_back(line);
    }

    Grid grid;
    const int N = input.size();
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (input[i][j] == '#') {
                grid.insert({i, j});
            }
        }
    }

    return {grid, {N/2, N/2}};
}

int64_t move(Grid& g, Coord& position, Direction& d) {
    int64_t infected = 0;
    const bool presentlyInfected = g.count(position);
    if (!presentlyInfected) {
        infected = 1;
        g.insert(position);
        d = static_cast<Direction>((int(d) + 4 - 1) % 4); // turn left
    }
    else {
        g.erase(position);
        d = static_cast<Direction>((int(d) + 4 + 1) % 4); // turn right
    }

    switch (d) {
    case Direction::N: --position.first; break;
    case Direction::E: ++position.second; break;
    case Direction::S: ++position.first; break;
    case Direction::W: --position.second; break;
    }
    return infected;
}

int64_t num_infected_after(Grid g, Coord position, size_t N) {
    Direction d = Direction::N;
    int64_t nInfected = 0;
    while (N--) {
        nInfected += move(g, position, d);
    }
    return nInfected;
}

//------------------------------------------------------------------------------
#define ASSERT_EQ(a, e) { \
    std::cout << #a << " == " << (a)  << " == " << (e) << std::endl; \
    assert((a == e)); }

void test() {
    std::istringstream iss("..#\n#..\n...");
    auto [grid, center] = create_grid(iss);
    ASSERT_EQ(grid.size(), 2);
    ASSERT_EQ(center.first, 1);
    ASSERT_EQ(center.second, 1);

    ASSERT_EQ(num_infected_after(grid, center, 7), 5);
    ASSERT_EQ(num_infected_after(grid, center, 70), 41);
    ASSERT_EQ(num_infected_after(grid, center, 10000), 5587);
}

void run() {
    auto [grid, center] = create_grid(std::cin);
    std::cout << num_infected_after(grid, center, 10000) << "\n";
}

int main() {
    test();
    run();
}

