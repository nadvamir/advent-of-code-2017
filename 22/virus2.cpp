#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <tuple>
#include <cassert>

enum class Direction {
    N, E, S, W
};

enum class State {
    Clean, Weakened, Infected, Flagged
};

using Coord = std::pair<int64_t, int64_t>;
using Grid = std::map<Coord, State>;


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
                grid.insert({{i, j}, State::Infected});
            }
        }
    }

    return {grid, {N/2, N/2}};
}

int64_t move(Grid& g, Coord& position, Direction& d) {
    int64_t infected = 0;

    State state = g[position];
    switch (state) {
    case State::Clean:
        g[position] = State::Weakened;
        d = static_cast<Direction>((int(d) + 4 - 1) % 4); // turn left
        break;
    case State::Weakened:
        infected = 1;
        g[position] = State::Infected;
        break;
    case State::Infected:
        g[position] = State::Flagged;
        d = static_cast<Direction>((int(d) + 4 + 1) % 4); // turn right
        break;
    case State::Flagged:
        g[position] = State::Clean;
        d = static_cast<Direction>((int(d) + 4 + 2) % 4); // turn around
        break;
    };

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

    ASSERT_EQ(num_infected_after(grid, center, 100), 26);
    ASSERT_EQ(num_infected_after(grid, center, 10'000'000), 2511944);
}

void run() {
    auto [grid, center] = create_grid(std::cin);
    std::cout << num_infected_after(grid, center, 10'000'000) << "\n";
}

int main() {
    test();
    run();
}

