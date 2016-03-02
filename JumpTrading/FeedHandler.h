#pragma once

#include "Book.h"
#include "MessageDetails.h"

#include <iostream>
#include <string>

class FeedHandler {
  static const int MAX_PRINT_LEVELS = 10;

  enum {
    ADD       = 'A',
    REMOVE    = 'X',
    MODIFY    = 'M',
    TICK      = 'T',
    UNDEFINED = 'U',
  };

 public:
  FeedHandler(std::ostream* cout);

  void processMessage(const std::string& line);
  void printCurrentOrderBook(std::ostream& os) const;
  void printTickInfo(std::ostream& os) const;

 private:
  void processAdd(const MessageDetails& msg);
  void processModify(const MessageDetails& msg);
  void processRemove(const MessageDetails& msg);

  void processTick(const TickMessage& msg);

  void printBookMid() const;

  Book          m_book;
  std::ostream* m_cout;
};
