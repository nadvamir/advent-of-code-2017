#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <unordered_map>
#include <queue>
#include <cassert>

using Registers = std::unordered_map<std::string, int64_t>;

int64_t get(const Registers& registers, const std::string& r2) {
    // assuming valid input, r2 is either a valid register or a number
    if (registers.count(r2) == 0) {
        return std::stoi(r2);
    }
    return registers.find(r2)->second;
}

int64_t execute(
        const std::vector<std::string>& program,
        Registers& registers,
        std::queue<int64_t>& q_this,
        std::queue<int64_t>& q_other,
        int64_t pc)
{
    if (pc >= program.size()) return pc; // end of program

    std::string op, r1, r2;
    std::stringstream ss(program[pc]);
    ss >> op >> r1 >> r2;

    int64_t i2 = !r2.empty() ? get(registers, r2) : 0;

    if      (op == "set") registers[r1] = i2;
    else if (op == "add") registers[r1] += i2;
    else if (op == "mul") registers[r1] *= i2;
    else if (op == "mod") registers[r1] %= i2;
    else if (op == "set") registers[r1] = i2;
    else if (op == "jgz") {
        if (get(registers, r1) > 0) return pc + i2;
    }
    else if (op == "snd") {
        q_other.push(get(registers, r1));
        ++registers["__nsnd"];
    }
    else if (op == "rcv") {
        if (q_this.empty()) return pc; // not received yet
        registers[r1] = q_this.front(); q_this.pop();
    }

    return pc + 1;
}

int64_t execute(const std::vector<std::string>& program) {
    int64_t pc0 = 0, pc1 = 0;
    Registers r0, r1;
    r0["p"] = 0; r1["p"] = 1;
    std::queue<int64_t> q0, q1;

    while (true) {
        int64_t modPc0 = execute(program, r0, q0, q1, pc0);
        int64_t modPc1 = execute(program, r1, q1, q0, pc1);
        if (modPc0 == pc0 && modPc1 == pc1) break;
        pc0 = modPc0;
        pc1 = modPc1;
    }
    return r1["__nsnd"];
}

//------------------------------------------------------------------------------
#define ASSERT_EQ(a, e) { \
    std::cout << #a << " == " << (a)  << " == " << (e) << std::endl; \
    assert((a == e)); }

void test() {
    std::vector<std::string> program {
        "snd 1",
        "snd 2",
        "snd p",
        "rcv a",
        "rcv b",
        "rcv c",
        "rcv d"
    };

    int64_t rcv = execute(program);
    ASSERT_EQ(rcv, 3);
}

void run() {
    std::vector<std::string> program;
    std::string line;
    while (getline(std::cin, line)) {
        program.push_back(line);
    }
    std::cout << execute(program) << "\n";
}

int main() {
    test();
    run();
}

