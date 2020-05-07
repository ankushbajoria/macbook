#ifndef POSTFIXCALCULATOR_H_
#define POSTFIXCALCULATOR_H_

#include "Operator.h"

#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class PostFixCalculator {
 public:
  PostFixCalculator();

  ~PostFixCalculator();

  void addOperator(char operatorStr, OperatorBase* oper);

  double calculate(bool& success, const std::string& input);

 private:
  std::vector<std::string> splitString(std::string input, char delimiter);

  bool checkForDouble(double& val, const std::string& str);

  typedef std::unordered_map<char, OperatorBase*> OperatorMapType;
  typedef OperatorMapType::iterator OperatorMapIterType;
  typedef std::pair<char, OperatorBase*> OperatorPairType;
  OperatorMapType m_operatorMap;
};

inline
PostFixCalculator::PostFixCalculator()
  : m_operatorMap() {
  addOperator('+', new OperatorAdd());
  addOperator('-', new OperatorMinus());
  addOperator('*', new OperatorMultiply());
  addOperator('/', new OperatorDivide());
}

inline
PostFixCalculator::~PostFixCalculator() {
  for (OperatorMapIterType iter = m_operatorMap.begin();
        iter != m_operatorMap.end(); iter++) {
    delete iter->second;
  }
}

inline
void
PostFixCalculator::addOperator(char operatorStr, OperatorBase* oper) {
  if (m_operatorMap.count(operatorStr) != 0) {
    m_operatorMap[operatorStr] = oper;
  } else {
    m_operatorMap.insert(OperatorPairType(operatorStr, oper));
  }
}

inline
std::vector<std::string>
PostFixCalculator::splitString(std::string input, char delimiter) {
  std::vector<std::string> ret;
  int prevPos = 0;
  int pos     = input.find(delimiter);

  while (pos != std::string::npos) {
    ret.push_back(input.substr(prevPos, pos - prevPos));
    prevPos = pos + 1;
    pos = input.find(delimiter, prevPos);
  }

  ret.push_back(input.substr(prevPos));

  return ret;
}

inline
bool
PostFixCalculator::checkForDouble(double& val, const std::string& str) {
  try {
    val = std::stod(str);
  } catch (std::invalid_argument arg) {
    return false;
  }

  return true;
}

inline
double
PostFixCalculator::calculate(bool& success, const std::string& input) {
  std::vector<std::string> args(std::move(splitString(input, ' ')));
  double val;
  std::stack<double> tokens;

  for (size_t idx = 0; idx < args.size(); idx++) {
    double val;
    if (checkForDouble(val, args[idx])) {
      tokens.push(val);
    } else if (args[idx].size() == 1 && 
               m_operatorMap.count(args[idx][0]) != 0) {
      double arg1, arg2;
      if (!tokens.empty()) {
        arg2 = tokens.top(); tokens.pop();
      } else {
        success = false;
        return 0;
      }

      if (!tokens.empty()) {
        arg1 = tokens.top(); tokens.pop();
      } else {
        success = false;
        return 0;
      }
      
      char oper = args[idx][0];
      tokens.push(m_operatorMap[oper]->operate(arg1, arg2));
    } else {
      success = false;
      return 0;
    }
  }

  if (tokens.size() != 1) {
    success = false;
    return 0;
  }

  success = true;
  return tokens.top();
}
#endif // POSTFIXCALCULATOR_H_
