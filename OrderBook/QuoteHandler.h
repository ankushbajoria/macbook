//
// Created by Ankush Bajoria on 10/05/20.
//

#include "Book.h"
#include "Order.h"

#include <boost/intrusive/unordered_set.hpp>
#include <boost/pool/object_pool.hpp>
#include <iostream>

#pragma once

namespace impl {

struct QuoteHandler {
    QuoteHandler() = default;

    template <typename T>
    void add(T msg) {
        if (m_orders.count(msg.order_id) != 0) {
            throw std::runtime_error("order_id exists " + std::to_string(msg.order_id));
        }

        auto * order_ref = m_pool.construct(Order{msg.order_id, msg.side, msg.qty, msg.price});
        auto [order, success] = m_orders.insert(*order_ref);

        Side side = static_cast<Side>(msg.side);
        auto * level = Side::BUY == side ? m_book.find_or_insert_level<Side::BUY, true>(msg.price)
                                         : m_book.find_or_insert_level<Side::SELL, true>(msg.price);

        level->amend_qty(msg.qty);
        level->get_level_data()->push_back(order_ref);
    }

    template <typename T>
    void modify(T msg) {
        if (m_orders.count(msg.order_id) == 0) {
            throw std::runtime_error("unknown order_id " + std::to_string(msg.order_id));
        }

        auto order = m_orders.find(msg.order_id);
        order->qty = msg.qty;

        Side side = static_cast<Side>(msg.side);
        auto * old_level = Side::BUY == side ? m_book.find_or_insert_level<Side::BUY>(order->price)
                                             : m_book.find_or_insert_level<Side::SELL>(order->price);

        assert(old_level != nullptr);

        auto * new_level = Side::BUY == side ? m_book.find_or_insert_level<Side::BUY, true>(msg.price)
                                             : m_book.find_or_insert_level<Side::SELL, true>(msg.price);

        old_level->amend_qty(-1 * order->qty);
        new_level->amend_qty(msg.qty);

        if (old_level->qty() == 0) {
            Side::BUY == side ? m_book.remove_level<Side::BUY>(old_level) : m_book.remove_level<Side::SELL>(old_level);
        }
    }

    template <typename T>
    void remove(T msg) {
        if (m_orders.count(msg.order_id) == 0) {
            throw std::runtime_error("unknown order_id " + std::to_string(msg.order_id));
        }

        auto order = m_orders.find(msg.order_id);

        auto side = static_cast<Side>(order->side);

        auto * level = Side::BUY == side ? m_book.find_or_insert_level<Side::BUY>(order->price)
                                         : m_book.find_or_insert_level<Side::SELL>(order->price);

        level->amend_qty(-1 * order->qty);

        if (level->qty() == 0) {
            Side::BUY == side ? m_book.remove_level<Side::BUY>(level) : m_book.remove_level<Side::SELL>(level);
        }

        m_orders.erase(msg.order_id);
        m_pool.free(&*order);
    }

    template <typename stream_type>
    void print_book(stream_type& s) const {
        m_book.print_book(s);
    }

private:
    using order_pool_type = boost::object_pool<Order>;
    using order_map_type = boost::intrusive::unordered_set<Order, boost::intrusive::key_of_value<OrderKey>>;
    using bucket_type = order_map_type::bucket_type;
    using bucket_traits = order_map_type::bucket_traits;

    std::vector<bucket_type> m_buckets{100};
    order_pool_type m_pool{};
    order_map_type m_orders{bucket_traits(m_buckets.data(), m_buckets.size())};
    Book<std::vector<Order*>> m_book;
};

}
