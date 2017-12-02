#include <iostream>
#include <string>
#include <cassert>

size_t captcha(const std::string input) {
    size_t sum = 0;
    size_t size = input.size();
    for (size_t i = 0; i < size; ++i) {
        if (input[i] == input[(i + size / 2) % size]) {
            sum += input[i] - '0';
        }
    }
    return sum;
}

//------------------------------------------------------------------------------
#define TEST(a) { std::cout << #a << " == " << (a) << std::endl; assert((a)); }

void test() {
    TEST(captcha("1212") == 6);
    TEST(captcha("1221") == 0);
    TEST(captcha("123425") == 4);
    TEST(captcha("123123") == 12);
    TEST(captcha("12131415") == 4);
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

