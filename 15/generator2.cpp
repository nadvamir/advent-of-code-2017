#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cassert>

uint64_t generate(uint64_t num, uint64_t factor) {
    return num * factor % 2147483647;
}

uint64_t generate_divisible(uint64_t num, uint64_t factor, uint64_t divisor) {
    uint64_t generated = generate(num, factor);
    while (generated % divisor != 0) generated = generate(generated, factor);
    return generated;
}

bool match(uint64_t A, uint64_t B) {
    uint64_t mask = (1 << 16) - 1;
    return (A & mask) == (B & mask);
}

uint64_t num_matches(uint64_t A, uint64_t B, uint64_t N) {
    uint64_t nMatches = 0;
    for (size_t i = 0; i < N; ++i) {
        A = generate_divisible(A, 16807, 4);
        B = generate_divisible(B, 48271, 8);
        nMatches += match(A, B);
    }
    return nMatches;
}

//------------------------------------------------------------------------------
#define ASSERT_EQ(a, e) { \
    std::cout << #a << " == " << (a)  << " == " << (e) << std::endl; \
    assert((a == e)); }

void test() {
    ASSERT_EQ(generate(65, 16807), 1092455);
    ASSERT_EQ(generate(8921, 48271), 430625591);

    ASSERT_EQ(generate_divisible(65, 16807, 4), 1352636452);
    ASSERT_EQ(generate_divisible(8921, 48271, 8), 1233683848);

    ASSERT_EQ(match(1092455, 430625591), false)
    ASSERT_EQ(match(245556042, 1431495498), true)

    ASSERT_EQ(num_matches(65, 8921, 5'000'000), 309);
}

void run() {
    uint64_t A, B;
    std::cin >> A >> B;
    std::cout << num_matches(A, B, 5'000'000) << "\n";
}

int main() {
    test();
    run();
}

