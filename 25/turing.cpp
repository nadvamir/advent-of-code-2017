#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <list>
#include <regex>
#include <cassert>

using Memory = std::list<int>;

void move_left(Memory& m, Memory::iterator& it) {
    if (it == begin(m)) {
        m.push_front(false);
    }
    --it;
}

void move_right(Memory& m, Memory::iterator& it) {
    ++it;
    if (it == end(m)) {
        m.push_back(false);
        it = --end(m);
    }
}

struct State {
    int valueToWrite[2];
    void (*move[2])(Memory&, Memory::iterator&);
    std::string nextState[2];
};

using Machine = std::unordered_map<std::string, State>;

std::tuple<std::string, long> read_inputs(std::istream& is) {
    std::string line;

    getline(is, line);
    std::regex stateRe(".*state (\\w+).");
    std::string start = *std::sregex_token_iterator(begin(line), end(line), stateRe, 1);

    getline(is, line);
    std::regex numRe("\\d+");
    long nSteps = std::stoi(*std::sregex_token_iterator(begin(line), end(line), numRe, 0));

    getline(is, line); // remove trailing empty line
    return {start, nSteps};
}

std::istream& read_state(std::istream& is, Machine& machine) {
    std::string line;

    std::regex stateRe(".*state (\\w+).*");
    std::regex numRe("\\d+");

    auto getState = [&]{ return *std::sregex_token_iterator(begin(line), end(line), stateRe, 1); };
    auto getNum = [&]{ return std::stoi(*std::sregex_token_iterator(begin(line), end(line), numRe, 0)); };

    getline(is, line);
    std::string stateName = getState();

    State state;
    auto readBranch = [&] {
        getline(is, line);
        int currentVal = getNum();
        getline(is, line);
        int valToWrite = getNum();
        getline(is, line);
        bool goRight = line.find("right") != std::string::npos;
        getline(is, line);
        std::string nextState = getState();
        
        state.valueToWrite[currentVal] = valToWrite;
        state.nextState[currentVal] = nextState;
        state.move[currentVal] = goRight ? &move_right : &move_left;
    };
    readBranch();
    readBranch();

    machine[stateName] = state;

    getline(is, line); // empty line
    return is;
}

Machine read_machine(std::istream& is) {
    Machine machine;
    while (read_state(is, machine));
    return machine;
}

Memory execute(const Machine& machine, std::string state, long steps) {
    Memory memory;
    memory.push_back(0);
    auto it = begin(memory);

    while (steps--) {
        const auto& cs = machine.find(state)->second;
        int current = *it;
        *it = cs.valueToWrite[current];
        (*cs.move[current])(memory, it);
        state = cs.nextState[current];
    }

    return memory;
}

long checksum(const Memory& memory) {
    return accumulate(begin(memory), end(memory), 0);
}

//------------------------------------------------------------------------------
#define ASSERT_EQ(a, e) { \
    std::cout << #a << " == " << (a)  << " == " << (e) << std::endl; \
    assert((a == e)); }

void test() {
    std::istringstream iss(
        "Begin in state A.\n"
        "Perform a diagnostic checksum after 6 steps.\n"
        "\n"
        "In state A:\n"
        "  If the current value is 0:\n"
        "    - Write the value 1.\n"
        "    - Move one slot to the right.\n"
        "    - Continue with state B.\n"
        "  If the current value is 1:\n"
        "    - Write the value 0.\n"
        "    - Move one slot to the left.\n"
        "    - Continue with state B.\n"
        "\n"
        "In state B:\n"
        "  If the current value is 0:\n"
        "    - Write the value 1.\n"
        "    - Move one slot to the left.\n"
        "    - Continue with state A.\n"
        "  If the current value is 1:\n"
        "    - Write the value 1.\n"
        "    - Move one slot to the right.\n"
        "    - Continue with state A.\n"
    );
    auto [start, steps] = read_inputs(iss);
    auto machine = read_machine(iss);

    ASSERT_EQ(start, "A");
    ASSERT_EQ(steps, 6);
    ASSERT_EQ(machine.size(), 2);

    Memory memory = execute(machine, start, steps);
    ASSERT_EQ(checksum(memory), 3);
}

void run() {
    auto [start, steps] = read_inputs(std::cin);
    auto machine = read_machine(std::cin);
    Memory memory = execute(machine, start, steps);
    std::cout << checksum(memory) << "\n";
}

int main() {
    test();
    run();
}

