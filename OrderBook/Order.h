#pragma once

#include <boost/intrusive/unordered_set.hpp>

namespace impl {

struct Order : public boost::intrusive::unordered_set_base_hook<> {
    Order(size_t _id, char _side, size_t _qty, double _price)
        : id(_id),
          side(_side),
          qty(_qty),
          price(_price)
    {}

    size_t id{};
    char side{};
    size_t qty{};
    double price{};
};

struct OrderKey {
    using type = size_t;
    type operator() (const Order& order) { return order.id; }
};

}