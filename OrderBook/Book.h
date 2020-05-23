#pragma once

#include "IntPriceUtil.h"
#include "Order.h"

#include <boost/pool/object_pool.hpp>
#include <iostream>
#include <unordered_map>
#include <vector>

namespace impl {

enum class Side : char {
    BUY = 'B',
    SELL = 'S'
};

template<typename LevelData>
class Level {
public:
    explicit Level(double price) : m_price(price) {}

    template <typename U, typename T>
    friend bool operator>(const Level<U> &lhs, const Level<T> &rhs) {
        return lhs.m_price > rhs.m_price;
    }

    template <typename U, typename T>
    friend bool operator==(const Level<U> & lhs, const Level<T> & rhs) {
        return lhs.m_price == rhs.m_price;
    }

    LevelData * get_level_data() { return &m_data; }

    auto amend_qty(int32_t delta) { m_qty += delta; return m_qty; }
    auto qty() const { return m_qty; }
    auto price() const { return m_price; }

private:
    double m_price;
    size_t m_qty{};
    LevelData m_data{};
};

template <>
class Level<void> {
public:
    explicit Level(double price) : m_price(price) {};
    double m_price{};
};

template <typename LevelData>
class Book {
    using level_type = Level<LevelData>;
    using level_container_type = std::vector<level_type>;
    using level_iterator_type = typename level_container_type::iterator;

public:
    Book() {
        m_levels[0].reserve(128);
        m_levels[1].reserve(128);
    };

    template <Side side, bool can_insert = false>
    level_type * find_or_insert_level(double price) {
        auto iter = locate<side>(price);
        auto& levels = Side::BUY == side ? m_levels[0] : m_levels[1];

        if (iter == levels.end() || (*iter).price() != price) {
            if constexpr (can_insert) {
                iter = levels.insert(iter, level_type(price));
            } else {
                return nullptr;
            }
        }

        return &(*iter);
    }

    template <Side side>
    void remove_level(double price) {
        auto iter = locate<side>(price);
        auto & levels = Side::BUY == side ? m_levels[0] : m_levels[1];

        if (iter == levels.end() || (*iter).price() != price) {
            throw std::runtime_error("level not found, side : " + std::string(static_cast<char>(side))
                                     + " price " + std::to_string(price));
        }

        levels.erase(iter);
    }

    template <Side side>
    void remove_level(level_type * level) {
        if constexpr (side == Side::BUY) {
            auto & levels = m_levels[0];
            levels.erase(std::remove(levels.begin(), levels.end(), *level), levels.end());
        }
        else {
            auto &levels = m_levels[1];
            levels.erase(std::remove(levels.begin(), levels.end(), *level), levels.end());
        }
    }

    template <typename stream_type>
    void print_book(stream_type& stream) const {
        stream << "BUY:\n";
        for (auto & level : m_levels[0]) {
            stream << level.price() << " " << level.qty() << "\n";
        }

        stream << "\nSELL:\n";
        for (auto & level : m_levels[1]) {
            stream << level.price() << " " << level.qty() << "\n";
        }

        stream << "\n";

        std::flush(stream);
    }

private:
    template <Side side>
    level_iterator_type locate(double price)
    {
        level_iterator_type iter;
        Level<void> level(price);

        if constexpr (side == Side::BUY) {
            auto & levels = m_levels[0];
            for (iter = levels.begin(); iter != levels.end() && *iter > level; ++iter) {}
        }
        else {
            auto & levels = m_levels[1];
            for (iter = levels.begin(); iter != levels.end() && level > *iter;  ++iter) {}
        }

        return iter;
    }

    level_container_type m_levels[2];
};

}