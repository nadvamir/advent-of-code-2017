#include <iostream>
#include <string>
#include <cassert>

size_t captcha(const std::string input) {
    size_t sum = 0;
    for (size_t i = 0; i < input.size(); ++i) {
        if (input[i] == input[(i + 1) % input.size()]) {
            sum += input[i] - '0';
        }
    }
    return sum;
}

//------------------------------------------------------------------------------
#define TEST(a) { std::cout << #a << " == " << (a) << std::endl; assert((a)); }

void test() {
    TEST(captcha("1122") == 3);
    TEST(captcha("1111") == 4);
    TEST(captcha("1234") == 0);
    TEST(captcha("9129") == 9);
}

void run() {
    std::string input;
    getline(std::cin, input);
    std::cout << captcha(input) << "\n";
}

int main() {
    test();
    run();
}

