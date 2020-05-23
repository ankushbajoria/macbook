#include "FeedHandler.h"

namespace impl {

//void FeedHandler::printBookMid() const {
//    auto topBidSz = m_book.getQtyAtLevel('B', 0);
//    auto topAskSz = m_book.getQtyAtLevel('S', 0);
//
//    if (topBidSz != 0 && topAskSz != 0) {
//        // auto mid = (m_book.getPriceAtLevel('B', 0) + m_book.getPriceAtLevel('S', 0)) / 2.0;
//        // *m_cout << mid << std::endl;
//    } else {
//        // *m_cout << "NAN" << std::endl;
//    }
//}

//void FeedHandler::processAdd(const RawMsg & msg) {
//  auto order = new Order(msg.orderid, msg.side, msg.px, msg.qty);
//  if (not m_book.addOrder(order)) {
//    *m_cout << "error adding order " << msg.orderid << std::endl;
//  } else {
//    printBookMid();
//  }
//
//  return;
//}
//
//void FeedHandler::processRemove(const RawMsg & msg) {
//  if (not m_book.removeOrder(msg.orderid)) {
//    *m_cout << "error removing order " << msg.orderid << std::endl;
//  } else {
//    printBookMid();
//  }
//
//  return;
//}
//
//// TODO: assumption, px is not modified, only qty;
//void FeedHandler::processModify(const RawMsg & msg) {
//  if (not m_book.modifyOrder(msg.orderid, msg.qty, msg.px)) {
//    *m_cout << "error modifying order " << msg.orderid << std::endl;
//  } else {
//    printBookMid();
//  }
//
//  return;
//}
//
//void FeedHandler::processTick (const TickMessage& tick) {
//  if (not m_book.processTick(tick.px, tick.qty)) {
//    *m_cout << "error adding tick " << tick.qty << "@" << tick.px << std::endl;
//    return;
//  }
//
//  // *m_cout << m_book.runningTickVolume()
//  //        << "@" << m_book.lastTickPx() << std::endl;
//
//  return;
//}

}