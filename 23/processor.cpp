#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <unordered_map>
#include <cassert>

using Registers = std::unordered_map<std::string, int64_t>;

int get(const Registers& registers, const std::string& r2) {
    // assuming valid input, r2 is either a valid register or a number
    if (registers.count(r2) == 0) {
        return std::stoi(r2);
    }
    return registers.find(r2)->second;
}

int execute(const std::vector<std::string>& program, Registers& registers, int pc)
{
    std::string op, r1, r2;
    std::stringstream ss(program[pc]);
    ss >> op >> r1 >> r2;

    int i2 = !r2.empty() ? get(registers, r2) : 0;
    if      (op == "set") registers[r1] = i2;
    else if (op == "sub") registers[r1] -= i2;
    else if (op == "mul") { registers[r1] *= i2; ++registers["__mul"]; }
    else if (op == "jnz") { if (get(registers, r1) != 0) return pc + i2; }

    return pc + 1;
}

int execute(const std::vector<std::string>& program, Registers& registers) {
    std::string init[] = {"a", "b", "c", "d", "e", "f", "g", "h"};
    for (const auto& r: init) registers[r] = 0;

    int pc = 0;
    while (pc != program.size()) {
        pc = execute(program, registers, pc);
    }
    return registers["__mul"];
}

//------------------------------------------------------------------------------
#define ASSERT_EQ(a, e) { \
    std::cout << #a << " == " << (a)  << " == " << (e) << std::endl; \
    assert((a == e)); }

void test() {
    Registers r;
    std::vector<std::string> program {
        "set a 1",
        "mul a a",
        "jnz a 2",
        "set b 2",
        "set a 3",
    };

    int rcv = execute(program, r);
    ASSERT_EQ(r["a"], 3);
    ASSERT_EQ(r["b"], 0);
    ASSERT_EQ(rcv, 1);
}

void run() {
    Registers registers;
    std::vector<std::string> program;
    std::string line;
    while (getline(std::cin, line)) {
        program.push_back(line);
    }
    std::cout << execute(program, registers) << "\n";
}

int main() {
    test();
    run();
}

