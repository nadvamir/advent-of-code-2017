#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <unordered_map>
#include <cassert>

using Registers = std::unordered_map<std::string, int64_t>;

bool condition(Registers& registers, const std::string& r, const std::string cmp, int64_t val) {
    if (cmp == "<") return registers[r] < val;
    if (cmp == ">") return registers[r] > val;
    if (cmp == ">=") return registers[r] >= val;
    if (cmp == "<=") return registers[r] <= val;
    if (cmp == "==") return registers[r] == val;
    if (cmp == "!=") return registers[r] != val;
    return false;
}

int64_t get_value(const std::string& op, int64_t val) {
    return (op == "inc") ? val : -val;
}

void process_line(Registers& registers, const std::string& line) {
    std::string r1, op, skip, r2, cmp;
    int64_t v1, v2;
    std::istringstream iss(line);
    iss >> r1 >> op >> v1 >> skip >> r2 >> cmp >> v2;

    if (condition(registers, r2, cmp, v2)) {
        registers[r1] += get_value(op, v1);
    }
}

int64_t get_max(const Registers& registers) {
    int max = begin(registers)->second;
    for (const auto& r: registers) {
        if (r.second > max) max = r.second;
    }
    return max;
}

//------------------------------------------------------------------------------
#define ASSERT_EQ(a, e) { \
    std::cout << #a << " == " << (a)  << " == " << (e) << std::endl; \
    assert((a == e)); }

void test() {
    Registers r;

    process_line(r, "b inc 5 if a > 1");
    ASSERT_EQ(r["b"], 0);
    process_line(r, "a inc 1 if b < 5");
    ASSERT_EQ(r["a"], 1);
    process_line(r, "c dec -10 if a >= 1");
    ASSERT_EQ(r["c"], 10);
    process_line(r, "c inc -20 if c == 10");
    ASSERT_EQ(r["c"], -10);
    process_line(r, "b inc 2 if c != 10");
    ASSERT_EQ(r["b"], 2);
    process_line(r, "b inc 2 if c <= -10");
    ASSERT_EQ(r["b"], 4);
}

void run() {
    Registers registers;
    std::string line;
    while (getline(std::cin, line)) {
        process_line(registers, line);
    }
    std::cout << get_max(registers) << "\n";
}

int main() {
    test();
    run();
}

