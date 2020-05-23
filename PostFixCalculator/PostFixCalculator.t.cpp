#include <gtest/gtest.h>

#include "PostFixCalculator.h"

TEST(PostFixCalculatorTest, theOne) {
    PostFixCalculator calc;

    ASSERT_ANY_THROW(calc.evaluate("1 +"));
    ASSERT_ANY_THROW(calc.evaluate("*"));

    std::string input;

    input = "3 4 +";
    EXPECT_EQ(calc.evaluate(input), 7);

    input = "4 3 -";
    EXPECT_EQ(calc.evaluate(input), 1);

    input = "4 2 /";
    EXPECT_EQ(calc.evaluate(input), 2);

    input = "-1.5 -3 /";
    EXPECT_EQ(calc.evaluate(input), 0.5);

    input = "-3 -0.33 *";
    EXPECT_EQ(calc.evaluate(input), 0.99);
}
