#include <bitset>
#include <iostream>
#include <stdint.h>

int main() {
#if 0
  int * a = new int;
  *a = 2;

  int8_t * b = reinterpret_cast<int8_t*>(a);
#endif
  union {
    char array[4];
    int  integer;
  } a;

  a.integer = -2;

  if (a.array[0] == 2) {
    std::cout << "My machine is small endian" << std::endl;
  } else if (a.array[0] == 0) {
    std::cout << "My machine is big endian" << std::endl;
  } else {
    std::cout << "I do not know what's up" << std::endl;
  }

  std::bitset<32> x(a.integer);

  std::cout << x << std::endl;

  return 0;
}
