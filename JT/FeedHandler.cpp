#include "FeedHandler.h"

FeedHandler::FeedHandler(std::ostream* cout)
  : m_book(cout)
  , m_cout(cout) {
}

void
FeedHandler::processMessage(const std::string& line) {
  // handle tick messages
  if (line[0] == TICK) {
    TickMessage msg(line);

    if (msg.px < 0) {
      *m_cout << "error parsing \"" << line << "\"" << std::endl;
      return;
    }

    processTick(msg);

    return;
  }

  MessageDetails msg(line);

  switch (msg.action) {
    case ADD:
    {
      processAdd(msg);
    } break;

    case MODIFY:
    {
      processModify(msg);
    } break;

    case REMOVE:
    {
      processRemove(msg);
    } break;

    case UNDEFINED:
    default:
    {
      *m_cout << "error parsing \"" << line << "\"" << std::endl;
    } break;
  }

  return;
}

void FeedHandler::printBookMid() const {
  auto topBidSz = m_book.getQtyAtLevel('B', 0);
  auto topAskSz = m_book.getQtyAtLevel('S', 0);

  if (topBidSz != 0 && topAskSz != 0) {
    // auto mid = (m_book.getPriceAtLevel('B', 0) + m_book.getPriceAtLevel('S', 0)) / 2.0;

    // *m_cout << mid << std::endl;
  } else {
    // *m_cout << "NAN" << std::endl;
  }
}

void FeedHandler::processAdd(const MessageDetails& msg) {
  auto order = new Order(msg.orderid, msg.side, msg.px, msg.qty);
  if (not m_book.addOrder(order)) {
    *m_cout << "error adding order " << msg.orderid << std::endl;
  } else {
    printBookMid();
  }

  return;
}

void FeedHandler::processRemove(const MessageDetails& msg) {
  if (not m_book.removeOrder(msg.orderid)) {
    *m_cout << "error removing order " << msg.orderid << std::endl;
  } else {
    printBookMid();
  }

  return;
}

// TODO: assumption, px is not modified, only qty;
void FeedHandler::processModify(const MessageDetails& msg) {
  if (not m_book.modifyOrder(msg.orderid, msg.qty, msg.px)) {
    *m_cout << "error modifying order " << msg.orderid << std::endl;
  } else {
    printBookMid();
  }

  return;
}

void FeedHandler::processTick (const TickMessage& tick) {
  if (not m_book.processTick(tick.px, tick.qty)) {
    *m_cout << "error adding tick " << tick.qty << "@" << tick.px << std::endl;
    return;
  }
  
  // *m_cout << m_book.runningTickVolume()
  //        << "@" << m_book.lastTickPx() << std::endl;

  return;
}

void
FeedHandler::printCurrentOrderBook(std::ostream& os) const {
  os << "\nBook:" << std::endl;
  for (int i = 0; i < MAX_PRINT_LEVELS; i++) {
    auto qtyBuy   = m_book.getQtyAtLevel('B', i);
    auto qtySell  = m_book.getQtyAtLevel('S', i);

    if (qtyBuy != 0 || qtySell != 0) {
      os << "\t" << "(" << m_book.numOrdersAtLevel('B', i) << ")\t"
                << m_book.getQtyAtLevel('B', i)
                << "\t" << m_book.getPriceAtLevel('B', i) 
                << "\t|\t"
                << m_book.getPriceAtLevel('S', i)
                << "\t" << m_book.getQtyAtLevel('S', i)
                << "\t(" << m_book.numOrdersAtLevel('S', i) << ")"
                << std::endl;
    } else {
      break;
    }
  }

  os << std::endl;
};

