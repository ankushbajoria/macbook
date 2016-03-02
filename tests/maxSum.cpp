#include <boost/lambda/lambda.hpp>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>

int main() {
  std::vector<int> input;
  typedef std::istream_iterator<int> in;

  std::for_each(
    in(std::cin), in(), input.push_back(_1));

  for (auto & integer : input) {
    std::cout << integer << std::endl;
  }
}
