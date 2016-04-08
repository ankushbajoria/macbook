#define BOOST_TEST_MODULE MyTest
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

#include "PostFixCalculator.h"

BOOST_AUTO_TEST_CASE( my_test )
{
  PostFixCalculator calc;

  std::string input;
  bool success = true;

  // ERRONEOUS INPUT
  input = "3 +";
  calc.calculate(success, input);
  BOOST_CHECK_EQUAL(success, false);

  input = "3 4 + 4 6 5 -";
  calc.calculate(success, input);
  BOOST_CHECK_EQUAL(success, false);

  input = "a b +";
  calc.calculate(success, input);
  BOOST_CHECK_EQUAL(success, false);

  // Addition
  input = "3 4 +";
  BOOST_CHECK_EQUAL(calc.calculate(success, input), 7);

  // Minus
  input = "4 3 -";
  BOOST_CHECK_EQUAL(calc.calculate(success, input), 1);

  // Divison
  input = "4 2 /";
  BOOST_CHECK_EQUAL(calc.calculate(success, input), 2);

  // negative double
  input = "-1.5 -3 /";
  BOOST_CHECK_EQUAL(calc.calculate(success, input), 0.5);

  // Multiply
  input = "-3 -0.33 *";
  BOOST_CHECK_EQUAL(calc.calculate(success, input), 0.99);
}
