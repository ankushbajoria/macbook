#include <iostream>

#include <vector>

int main() {
  std::vector<int> v;
  v.push_back(1);

  v.insert(v.begin(), 2);

  for (auto i : v) {
    std::cout << i << std::endl;
  }

  return 0;
}
