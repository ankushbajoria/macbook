#include "PostFixCalculator.h"

#include <iostream>


#include <stack>
#include <unordered_set>

int main() {
  bool success = false;

  PostFixCalculator calc;
  calc.addOperator('%', new OperatorModulo());

  std::cout << "Enter expr:" << std::endl;
  std::string inputLine;
  std::getline(std::cin, inputLine);

  while (inputLine != "exit") {
    double val = calc.calculate(success, inputLine);
    if (success) {
      std::cout << "result = " << val << std::endl;
      std::cout << "\nEnter expr:" << std::endl;
    } else {
      std::cout << "error in expr! Enter expr again:" << std::endl;
    }

    std::getline(std::cin, inputLine);
  }

  return 0;
}
