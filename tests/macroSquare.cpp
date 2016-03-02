#include <iostream>
#include <string>

using namespace std;

#define sq(x) x*x

extern int add(int x, int y);

int main() {
  // string str = "2 ";
  int str  = 2;
  std::cout << add(str, str) << std::endl;
}
