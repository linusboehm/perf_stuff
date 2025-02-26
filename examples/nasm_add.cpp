#include <cstdint>
#include <iostream>

extern "C" int64_t add_numbers(int64_t a, int64_t b);

int main() {
  int64_t result = add_numbers(10, 20);
  std::cout << "Result: " << result << std::endl;
  return 0;
}
