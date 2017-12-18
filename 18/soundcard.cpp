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
    else if (op == "add") registers[r1] += i2;
    else if (op == "mul") registers[r1] *= i2;
    else if (op == "mod") registers[r1] %= i2;
    else if (op == "snd") registers["__snd"] = get(registers, r1);
    else if (op == "set") registers[r1] = i2;
    else if (op == "jgz") { if (get(registers, r1) != 0) return pc + i2; }
    else if (op == "rcv") { if (get(registers, r1) != 0) return program.size(); }

    return pc + 1;
}

int execute(const std::vector<std::string>& program, Registers& registers) {
    int pc = 0;
    while (pc != program.size()) {
        pc = execute(program, registers, pc);
    }
    return registers["__snd"];
}

//------------------------------------------------------------------------------
#define ASSERT_EQ(a, e) { \
    std::cout << #a << " == " << (a)  << " == " << (e) << std::endl; \
    assert((a == e)); }

void test() {
    Registers r;
    std::vector<std::string> program {
        "set a 1",
        "add a 2",
        "mul a a",
        "mod a 5",
        "snd a",
        "set a 0",
        "rcv a",
        "jgz a -1",
        "set a 1",
        "jgz a -2"
    };

    int rcv = execute(program, r);
    ASSERT_EQ(rcv, 4);
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

