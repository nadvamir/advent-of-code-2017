#include <iostream>
#include <string>

int main() {
    std::string input;
    getline(std::cin, input);

    size_t sum = 0;
    for (size_t i = 0; i < input.size(); ++i) {
        if (input[i] == input[(i + 1) % input.size()]) {
            sum += input[i] - '0';
        }
    }

    std::cout << sum << "\n";
}
