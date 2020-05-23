#pragma once

//#include "Book.h"
#include "QuoteHandler.h"
#include "RawMsg.h"

#include <iostream>
#include <string>

namespace impl {

class FeedHandler {
    static const int MAX_PRINT_LEVELS = 10;

public:
    FeedHandler() = default;

    void process(const std::string& line) {
        parse(*this, line);
    }

    void handle(AddMsg msg) { m_quote_handler.add(msg); }
    void handle(RemoveMsg msg) { m_quote_handler.remove(msg); }
    void handle(ModifyMsg msg) { m_quote_handler.modify(msg); }
    void handle(TradeMsg msg) {}

    template <typename stream_type>
    void print_book(stream_type& s) const { m_quote_handler.print_book(s); }

private:
    QuoteHandler m_quote_handler{};
};

}