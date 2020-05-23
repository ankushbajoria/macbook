#pragma once

#include "StringHelper.h"
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

namespace impl {

enum class MsgType {
    ADD = 'A',
    REMOVE = 'X',
    MODIFY = 'M',
    TRADE = 'T',
};

template <MsgType MType>
struct RawMsg {
    uint32_t    order_id;
    char        side;
    size_t      qty;
    double      price;
};

template <>
struct RawMsg<MsgType::TRADE> {
    size_t qty;
    double price;
};

using AddMsg = RawMsg<MsgType::ADD>;
using RemoveMsg = RawMsg<MsgType::REMOVE>;
using ModifyMsg = RawMsg<MsgType::MODIFY>;
using TradeMsg = RawMsg<MsgType::TRADE>;

template <typename T>
T createMsg(const std::vector<std::string>& tokens) {
    if (__builtin_expect(tokens.size() != 5, false)) {
        throw std::invalid_argument("invalid msg");
    }

    uint32_t order_id = std::stoul(tokens[1]);
    char side = tokens[2][0];
    size_t qty = std::stoul(tokens[3]);
    double price = std::stod(tokens[4]);

    return T{order_id, side, qty, price};
}

template <>
TradeMsg createMsg<TradeMsg>(const std::vector<std::string>& tokens);

template <typename Handler>
void parse(Handler& handler, std::string const& msg) {
    std::vector<std::string> tokens;
    boost::split(tokens, msg, boost::is_any_of(","));
    auto msgtype = static_cast<MsgType>(tokens.front()[0]);

    switch (msgtype) {
        case MsgType::ADD: {
            handler.handle(createMsg<AddMsg>(tokens));
        } break;
        case MsgType::REMOVE: {
            handler.handle(createMsg<RemoveMsg>(tokens));
        } break;
        case MsgType::MODIFY: {
            handler.handle(createMsg<ModifyMsg>(tokens));
        } break;
        case MsgType::TRADE: {
            handler.handle(createMsg<TradeMsg>(tokens));
        } break;
        default:
            throw std::invalid_argument("unidentified msgtype " + tokens.front());
    }
}

}
