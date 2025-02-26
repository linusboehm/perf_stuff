#include <iostream>

extern "C" long add_numbers(long a, long b);

int main() {
    long result = add_numbers(10, 20);
    std::cout << "Result: " << result << std::endl;
    return 0;
}
