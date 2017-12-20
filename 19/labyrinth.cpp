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

void get_path(
        const std::vector<std::string>& labyrinth,
        int i, 
        int j,
        Direction dir,
        std::ostream& path)
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
    if (c == ' ') return;
    if (isalpha(c))  path << c; 

    auto ndir = new_direction(dir);
    switch (ndir) {
        case Direction::N: return get_path(labyrinth, i-1, j, ndir, path);
        case Direction::E: return get_path(labyrinth, i, j+1, ndir, path);
        case Direction::S: return get_path(labyrinth, i+1, j, ndir, path);
        case Direction::W: return get_path(labyrinth, i, j-1, ndir, path);
        case Direction::FIN: return;
    }
}

std::string get_path(const std::vector<std::string>& labyrinth) {
    int entrance = find_top(labyrinth);
    std::stringstream path;
    get_path(labyrinth, 0, entrance, Direction::S, path);
    return path.str();
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
    ASSERT_EQ(get_path(labyrinth), "ABCDEF");
}

void run() {
    auto labyrinth = read_labyrinth(std::cin);
    std::cout << get_path(labyrinth) << "\n";
}

int main() {
    test();
    run();
}

