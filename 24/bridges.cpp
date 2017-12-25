#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <list>
#include <cassert>

struct Component {
    int port1, port2;
    bool used;
};

using Components = std::list<Component>;
using Index = std::unordered_map<int, std::vector<Components::iterator>>;

Components read_components(std::istream& is) {
    Components comps;
    int port1, port2; char separator;
    while (is >> port1 >> separator >> port2) {
        comps.push_back({port1, port2, false});
    }
    return comps;
}

Index build_index(Components& comps) {
    Index index;
    for (auto it = begin(comps); it != end(comps); ++it) {
        index[it->port1].push_back(it);
        index[it->port2].push_back(it);
    }
    return index;
}

int strongest_bridge(Index& index, int incoming_port) {
    int maximum = 0;
    if (index.count(incoming_port) == 0) {
        return maximum;
    }

    for (auto it: index[incoming_port]) {
        if (it->used) continue;
        int next_port = it->port1 == incoming_port ? it->port2 : it->port1;
        it->used = true;
        maximum = std::max(maximum, it->port1 + it->port2 + strongest_bridge(index, next_port));
        it->used = false;
    }
    return maximum;
}

//------------------------------------------------------------------------------
#define ASSERT_EQ(a, e) { \
    std::cout << #a << " == " << (a)  << " == " << (e) << std::endl; \
    assert((a == e)); }

void test() {
    std::istringstream iss("0/2\n2/2\n2/3\n3/4\n3/5\n0/1\n10/1\n9/10");
    auto componenets = read_components(iss);
    ASSERT_EQ(componenets.size(), 8);
    ASSERT_EQ(componenets.front().port1, 0);
    ASSERT_EQ(componenets.front().port2, 2);
    ASSERT_EQ(componenets.front().used, false);

    auto index = build_index(componenets);
    ASSERT_EQ(index.size(), 8);

    ASSERT_EQ(strongest_bridge(index, 0), 31);
}

void run() {
    auto componenets = read_components(std::cin);
    auto index = build_index(componenets);
    std::cout << strongest_bridge(index, 0) << "\n";
}

int main() {
    test();
    run();
}

