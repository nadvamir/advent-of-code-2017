#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <unordered_set>
#include <unordered_map>
#include <tuple>
#include <cctype>
#include <cassert>

using Children = std::unordered_set<std::string>;
using Parents = std::unordered_set<std::string>;
using Vertex = std::tuple<std::string, int, Children, Parents>;
using Graph = std::unordered_map<std::string, Vertex>;

Vertex to_vertex(std::string line) {
    std::string name;
    int weight;
    Children children;
    Parents parents;

    // clean line from symbols
    std::string l1 = line;
    line.erase(remove_if(begin(line), end(line), [](auto c) {
        return !isalnum(c) && !isspace(c);
    }), end(line));
    std::string l2 = line;

    // name
    std::istringstream iss(line);
    iss >> name;

    // weight
    std::string weight_s; iss >> weight_s;
    weight = stoi(weight_s);

    // children
    copy(
        std::istream_iterator<std::string>(iss),
        std::istream_iterator<std::string>(),
        inserter(children, begin(children)));

    return {name, weight, children, parents};
}

void insert_vertex(Graph& g, Vertex v) {
    const auto& [name, weight, children, _1] = v;
    auto& [e_name, e_weight, e_children, _2] = g[name];
    e_name = name;
    e_weight = weight;
    e_children = children;

    for (const auto& c: children) {
        std::get<3>(g[c]).insert(name); // add parents
    }
}

Graph build_graph(std::istream& input) {
    Graph graph;
    std::string line;
    while (getline(input, line)) {
        insert_vertex(graph, to_vertex(line));
    }
    return graph;
}

std::string get_bottom(const Graph& graph) {
    for (const auto& v: graph) {
        const auto& [name, _1, _2, parents] = v.second;
        if (parents.empty()) return name;
    }
    return "";
}

std::tuple<int, int> get_wrong(const Graph& g, const std::string& node) {
    std::vector<int> weights;
    const auto& [name, weight, children, _] = g.find(node)->second;

    if (children.empty()) return {weight, -1};

    int sum = 0;
    for (const auto& c: children) {
        const auto [c_weight, wrong] = get_wrong(g, c);
        if (wrong != -1) return {-1, wrong};
        weights.push_back(c_weight);
        sum += c_weight;
    }

    if (all_of(begin(weights), end(weights), [&](auto w) { return w == weights[0]; })) {
        return {sum + weight, -1};
    }

    // find the odd number
    auto min = min_element(begin(weights), end(weights));
    auto max = max_element(begin(weights), end(weights));

    auto get_wrong_child_weight = [&](decltype(min) it) {
        auto cit = begin(children);
        for (auto wit = begin(weights); wit != it; ++cit, ++wit);
        return std::get<1>(g.find(*cit)->second);
    };

    int N = weights.size();
    if (sum - *min * N < *max * N - sum) {
        auto wc = get_wrong_child_weight(max);
        return {-1, wc + *min - *max};
    }
    else {
        auto wc = get_wrong_child_weight(min);
        return {-1, wc + *max - *min};
    }
}

//------------------------------------------------------------------------------
#define ASSERT_EQ(a, e) { \
    assert((a == e)); }

void test() {
    auto[name, weight, children, parents] = to_vertex("fwft (72) -> ktlh, cntj, xhth");
    ASSERT_EQ(name, "fwft");
    ASSERT_EQ(weight, 72);
    ASSERT_EQ(children, (Children{"ktlh", "cntj", "xhth"}));
    ASSERT_EQ(parents, (Parents{}));

    // too heavy
    std::string data
        = "pbga (66)\nxhth (57)\nebii (61)\nhavc (66)\nktlj (57)\n"
          "fwft (72) -> ktlj, cntj, xhth\nqoyq (66)\npadx (45) -> pbga, havc, qoyq\n"
          "tknk (41) -> ugml, padx, fwft\njptl (61)\nugml (68) -> gyxo, ebii, jptl\n"
          "gyxo (61)\ncntj (57)";
    std::istringstream iss(data);
    auto graph = build_graph(iss);

    ASSERT_EQ(get_bottom(graph), "tknk");
    ASSERT_EQ(std::get<1>(get_wrong(graph, "tknk")), 60);

    // too light
    std::string data2
        = "pbga (66)\nxhth (57)\nebii (51)\nhavc (66)\nktlj (57)\n"
          "fwft (72) -> ktlj, cntj, xhth\nqoyq (66)\npadx (45) -> pbga, havc, qoyq\n"
          "tknk (41) -> ugml, padx, fwft\njptl (51)\nugml (68) -> gyxo, ebii, jptl\n"
          "gyxo (51)\ncntj (57)";
    std::istringstream iss2(data2);
    auto graph2 = build_graph(iss2);

    ASSERT_EQ(get_bottom(graph2), "tknk");
    ASSERT_EQ(std::get<1>(get_wrong(graph2, "tknk")), 90);
}

void run() {
    const auto graph = build_graph(std::cin);
    std::cout << std::get<1>(get_wrong(graph, get_bottom(graph))) << "\n";
}

int main() {
    test();
    run();
}

