#pragma once

#include "Operator.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class PostFixCalculator {
public:
    PostFixCalculator();

    ~PostFixCalculator() = default;

    void add_operator(char operatorStr, std::unique_ptr<OperatorBase>&& oper);

    double evaluate(const std::string &input);

private:
    using OperatorMapType = std::unordered_map<char, std::unique_ptr<OperatorBase>>;
    OperatorMapType m_operatorMap{};
};

inline PostFixCalculator::PostFixCalculator() {
    add_operator('+', std::make_unique<OperatorAdd>());
    add_operator('-', std::make_unique<OperatorMinus>());
    add_operator('*', std::make_unique<OperatorMultiply>());
    add_operator('/', std::make_unique<OperatorDivide>());
}

inline void
PostFixCalculator::add_operator(char operatorStr, std::unique_ptr<OperatorBase>&& oper) {
    m_operatorMap.try_emplace(operatorStr, std::move(oper));
}

inline double
PostFixCalculator::evaluate(const std::string &input) {
    std::vector<std::string> args;
    boost::split(args, input, boost::is_any_of(" "));

    std::stack<double> tokens;
    auto top = [&] () {
        if (tokens.empty()) {
            throw std::runtime_error("stack empty");
        }

        auto ret = tokens.top();
        tokens.pop();
        return ret;
    };

    for (auto & arg : args) {
        try {
            auto val = boost::lexical_cast<double>(arg);
            tokens.push(val);
        } catch (boost::bad_lexical_cast const&) {
            assert(arg.size() == 1);
            auto oper_char = arg[0];
            auto oper = m_operatorMap[oper_char].get();

            if (oper == nullptr) {
                throw std::invalid_argument("invalid input string - " + input);
            }

            auto arg2 = top();
            auto arg1 = top();
            tokens.push(oper->operate(arg1, arg2));
        }
    }

    if (tokens.size() != 1) {
        throw std::invalid_argument("invalid expression " + input);
    }

    return tokens.top();
}