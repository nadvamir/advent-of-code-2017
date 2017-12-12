#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <cassert>

using Graph = std::unordered_map<int, std::unordered_set<int>>;

void addNode(Graph& g, const std::string& line) {
    std::unordered_set<int> nodes;
    std::regex nodeRx {"\\d+"};
    std::sregex_token_iterator matches(begin(line), end(line), nodeRx);

    int first = -1;
    for_each(matches, std::sregex_token_iterator(), [&](const auto& node) {
            if (first < 0) {
                first = std::stoi(node);
                g.insert({first, {}});
            }
            else {
                int inode = std::stoi(node);
                g[first].insert(inode);
                g[inode].insert(first);
            }
    });
}

// destroys the graph in the progress
size_t get_connections(Graph& g, int node) {
    size_t conns = 1;
    auto cs = g[node];
    g.erase(node);

    for (auto n: cs) {
        if (g.count(n)) {
            conns += get_connections(g, n);
        }
    }
    return conns;
}

size_t get_groups(Graph graph) {
    size_t ngroups = 0;
    while (!graph.empty()) {
        get_connections(graph, begin(graph)->first);
        ++ngroups;
    }
    return ngroups;
}

//------------------------------------------------------------------------------
#define ASSERT_EQ(a, e) { \
    std::cout << #a << " == " << (a) << " == " << (e) << std::endl; \
    assert((a == e)); }

void test() {
    Graph g;
    addNode(g, "2 <-> 0, 3, 4");
    ASSERT_EQ(g.size(), 4);
    ASSERT_EQ(g[2].size(), 3);

    addNode(g, "0 <-> 2");
    addNode(g, "1 <-> 1");
    addNode(g, "3 <-> 2, 4");
    addNode(g, "4 <-> 2, 3, 6");
    addNode(g, "5 <-> 6");
    addNode(g, "6 <-> 4, 5");
    ASSERT_EQ(g.size(), 7);
    auto ncs = get_connections(g, 0);
    ASSERT_EQ(ncs, 6);

    Graph g2;
    addNode(g2, "2 <-> 0, 3, 4");
    addNode(g2, "0 <-> 2");
    addNode(g2, "1 <-> 1");
    addNode(g2, "3 <-> 2, 4");
    addNode(g2, "4 <-> 2, 3, 6");
    addNode(g2, "5 <-> 6");
    addNode(g2, "6 <-> 4, 5");

    auto ngroups = get_groups(g2);
    ASSERT_EQ(ngroups, 2);
}

void run() {
    std::string line;
    Graph g;
    while (getline(std::cin, line)) addNode(g, line);
    std::cout << get_groups(g) << "\n";
}

int main() {
    test();
    run();
}

