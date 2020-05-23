//
// Created by Ankush Bajoria on 10/05/20.
//

#include "RawMsg.h"

namespace impl {

template <>
TradeMsg createMsg<TradeMsg>(const std::vector<std::string>& tokens) {
    if (__builtin_expect(tokens.size() != 3, false)) {
        throw std::invalid_argument("invalid msg");
    }

    size_t qty = std::stoul(tokens[1]);
    double price = std::stod(tokens[2]);

    return TradeMsg{qty, price};
}

}