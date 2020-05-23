//
// Created by Ankush Bajoria on 10/05/20.
//

#include "RawMsg.h"

#include <gtest/gtest.h>

using namespace impl;

namespace {

struct Handler {
    ;
    void handle(AddMsg) { add_msg += 1; }
    void handle(RemoveMsg) { remove_msg += 1; }
    void handle(ModifyMsg) { modify_msg += 1; }
    void handle(TradeMsg) { trade_msg += 1; }

    size_t add_msg{};
    size_t remove_msg{};
    size_t modify_msg{};
    size_t trade_msg{};
};

TEST(RawMsgTest, Valid) {
    Handler handler;

    parse(handler, "A,1,B,10,100.0");
    EXPECT_EQ(1, handler.add_msg);

    parse(handler, "M,1,B,10,99.0");
    EXPECT_EQ(1, handler.modify_msg);

    parse(handler, "X,1,B,10,99.0");
    EXPECT_EQ(1, handler.remove_msg);

    parse(handler, "T,10,99.0");
    EXPECT_EQ(1, handler.trade_msg);
}

TEST(RawMsgTest, Invalid) {
    Handler handler;
    ASSERT_THROW(parse(handler, "U,1"), std::invalid_argument);
    ASSERT_THROW(parse(handler, "A,1,2,3,4,5"), std::invalid_argument);
    ASSERT_THROW(parse(handler, "T,1,2,3"), std::invalid_argument);

    ASSERT_ANY_THROW(parse(handler, "A,1,B,AAA,100.0"));
}

}