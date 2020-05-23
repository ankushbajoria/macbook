#include "PostFixCalculator.h"

#include <iostream>

#include <stack>

namespace {

std::string get_expr() {
    std::string ret{};
    std::cout << "Enter expr:" << std::endl;
    std::getline(std::cin, ret);
    return ret;
}

void print_result(double output) {
    std::cout << "result = " << output << "\n" << std::endl;
}

void print_error() {
    std::cout << "error in expr!\n" << std::endl;
}

}

int main() {
    PostFixCalculator calc{};
    calc.add_operator('%', std::make_unique<OperatorModulo>());

    for (std::string inputLine = get_expr(); inputLine != "exit"; inputLine = get_expr()) {
        try {
            print_result(calc.evaluate(inputLine));
        } catch (std::exception const&) {
            print_error();
        }
    }

    return 0;
}
