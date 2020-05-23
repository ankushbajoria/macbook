#pragma once

#include <cmath>

class OperatorBase {
public:
    virtual double operate(double arg1, double arg2) = 0;
    virtual ~OperatorBase() = default;
};

class OperatorAdd : public OperatorBase {
public:
    double operate(double arg1, double arg2) override {
        return arg1 + arg2;
    }
};

class OperatorMinus : public OperatorBase {
public:
    double operate(double arg1, double arg2) override {
        return arg1 - arg2;
    }
};

class OperatorDivide : public OperatorBase {
public:
    double operate(double arg1, double arg2) override {
        return arg1 / arg2;
    }
};

class OperatorMultiply : public OperatorBase {
public:
    double operate(double arg1, double arg2) override {
        return arg1 * arg2;
    }
};

class OperatorModulo : public OperatorBase {
public:
    double operate(double arg1, double arg2) override {
        return fmod(arg1, arg2);
    }
};
