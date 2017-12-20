#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cassert>

enum class Direction {
    N, E, S, W, FIN
};

std::vector<std::string> read_labyrinth(std::istream& is) {
    std::vector<std::string> labyrinth;
    std::string line;
    while (getline(std::cin, line)) {
        labyrinth.push_back(line);
    }
    return labyrinth;
}

int find_top(const std::vector<std::string>& labyrinth) {
    return labyrinth.front().find('|');
}

int get_path(
        const std::vector<std::string>& labyrinth,
        int i, 
        int j,
        Direction dir)
{
    auto get = [&labyrinth](int i, int j) {
        if (i < 0 || i >= labyrinth.size() || j < 0 || j >= labyrinth[i].size()) {
            return ' ';
        }
        return labyrinth[i][j];
    };

    auto new_direction = [&](Direction dir) {
        if (labyrinth[i][j] != '+') return dir;
        if (dir == Direction::N || dir == Direction::S) {
            if (get(i, j-1) != ' ') return Direction::W;
            else if (get(i, j+1) != ' ') return Direction::E;
            else return Direction::FIN;
        }
        else {
            if (get(i-1, j) != ' ') return Direction::N;
            else if (get(i+1, j) != ' ') return Direction::S;
            else return Direction::FIN;
        }
    };

    char c = get(i, j);
    if (c == ' ') return 0;

    auto ndir = new_direction(dir);
    switch (ndir) {
        case Direction::N: return 1 + get_path(labyrinth, i-1, j, ndir);
        case Direction::E: return 1 + get_path(labyrinth, i, j+1, ndir);
        case Direction::S: return 1 + get_path(labyrinth, i+1, j, ndir);
        case Direction::W: return 1 + get_path(labyrinth, i, j-1, ndir);
        case Direction::FIN: return 0;
    }
}

int get_path(const std::vector<std::string>& labyrinth) {
    int entrance = find_top(labyrinth);
    return get_path(labyrinth, 0, entrance, Direction::S);
}

//------------------------------------------------------------------------------
#define ASSERT_EQ(a, e) { \
    std::cout << #a << " == " << (a)  << " == " << (e) << std::endl; \
    assert((a == e)); }

void test() {
    std::vector<std::string> labyrinth {
        "     |          ",
        "     |  +--+    ",
        "     A  |  C    ",
        " F---|----E|--+ ",
        "     |  |  |  D ",
        "     +B-+  +--+ "
    };
    ASSERT_EQ(find_top(labyrinth), 5);
    ASSERT_EQ(get_path(labyrinth), 38);
}

void run() {
    auto labyrinth = read_labyrinth(std::cin);
    std::cout << get_path(labyrinth) << "\n";
}

int main() {
    test();
    run();
}

