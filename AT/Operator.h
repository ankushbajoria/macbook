#ifndef OPERAdoubleOR_H_
#define OPERAdoubleOR_H_

#include <math.h>

class OperatorBase {
 public:
  virtual double operate(double arg1, double arg2) = 0;
  virtual ~OperatorBase() {}
};

class OperatorAdd : public OperatorBase {
 public:
  double operate(double arg1, double arg2) {
    return arg1 + arg2;
  }
};

class OperatorMinus : public OperatorBase {
 public:
  double operate(double arg1, double arg2) {
    return arg1 - arg2;
  }
};

class OperatorDivide : public OperatorBase {
 public:
  double operate(double arg1, double arg2) {
    return arg1 / arg2;
  }
};

class OperatorMultiply : public OperatorBase {
 public:
  double operate(double arg1, double arg2) {
    return arg1 * arg2;
  }
};

class OperatorModulo : public OperatorBase {
 public:
  double operate(double arg1, double arg2) {
    return fmod(arg1, arg2);
  }
};

#endif // OPERAdoubleOR_H_
