#ifndef BOOK_H_
#define BOOK_H_

#include "IntPriceUtil.h"
#include "Order.h"

#include <iostream>
#include <unordered_map>
#include <vector>

class Book {
  enum Side {
    BUY,
    SELL,
    NUM_SIDES
  };

 public:
  Book();

  // modifiers
  bool addOrder(Order *order);
  bool removeOrder(int id);
  bool modifyOrder(int id, int qty);

  // LevelBook accessors
  double              getPriceAtLevel(char side, int levelno);
  int                 getQtyAtLevel(char side, int levelno);
  std::vector<Order*> getOrders(char side);

 private:
  // Helper Functions
  int FromSideCharToEnum(char side);
  Order *getOrder(int id);

  struct Level {
    int intPrice;
    int totalQty;
    std::unordered_map<size_t, Order*> orderList;

    Level(Order *order, int intPrice)
      : intPrice(intPrice),
        totalQty(order->quantity),
        orderList()
      {
        orderList.insert(std::pair<size_t, Order*>(size_t(order), order));
      };
    
    void addOrder(Order *order) {
      totalQty += order->quantity;
      orderList.insert(std::pair<size_t, Order*>(size_t(order), order));
    }

    void removeOrder(Order *order) {
      totalQty -= order->quantity;
      orderList.erase(size_t(order));
    }
  };

  typedef typename std::vector<Level*> LevelBookHalf;
  typedef LevelBookHalf::iterator LevelBookHalfIter;
  LevelBookHalf m_levelBook[2];

  void insertToLevelBook(Order *order);
  void modifyLevelBook(Order *order, int oldQty);
  void removeFromLevelBook(Order *order);

  typedef std::unordered_map<int, Order*> OrderMap;
  typedef OrderMap::iterator OrderMapIterator;
  OrderMap m_orderMap;
};

inline
Book::Book() 
  : m_orderMap()
{ }

inline
int Book::FromSideCharToEnum(char side) {
  switch (side) {
    case 'B' : return BUY;
    case 'S': return SELL;
    default:
      return NUM_SIDES;
  }

  return NUM_SIDES;
}

inline
void Book::modifyLevelBook(Order *order, int oldQty) {
  int side = FromSideCharToEnum(order->side);
  int intPrice = IntPriceUtil::DoubleToIntPrice(order->price);

  for (auto elem : m_levelBook[side]) {
    if (elem->intPrice == intPrice) {
      elem->totalQty = elem->totalQty - oldQty + order->quantity;
      break;
    }
  }

  return;
}

inline
void Book::insertToLevelBook(Order *order) {
  int side = FromSideCharToEnum(order->side); // add sanity check;

  int intPrice = IntPriceUtil::DoubleToIntPrice(order->price);
  bool levelWithPx = false;

  LevelBookHalfIter iter;
  if (side == BUY) {
    iter = m_levelBook[BUY].begin();
    while (iter != m_levelBook[BUY].end()) {
      Level *curLevel = *iter;

      if (curLevel->intPrice == intPrice) {
        curLevel->addOrder(order);

        levelWithPx = true;
        break;
      } else if (curLevel->intPrice < intPrice) {
        break;
      } else {
        iter++;
      }
    }

    if (!levelWithPx) {
      Level *level = new Level(order, intPrice);
      m_levelBook[BUY].insert(iter, level);
    }

    return;
  } else {
    iter = m_levelBook[SELL].begin();
    while (iter != m_levelBook[SELL].end()) {
      Level *curLevel = *iter;

      if (curLevel->intPrice == intPrice) {
        curLevel->addOrder(order);

        levelWithPx = true;
        break;
      } else if (curLevel->intPrice > intPrice) {
          break;
      } else {
        iter++;
      }
    }

    if (!levelWithPx) {
      Level *level = new Level(order, intPrice);
      m_levelBook[SELL].insert(iter, level);
    }
  }
}

inline
void Book::removeFromLevelBook(Order *order) {
  int intPrice = IntPriceUtil::DoubleToIntPrice(order->price);
  LevelBookHalfIter iter = order->side == 'B' ? m_levelBook[BUY].begin() : m_levelBook[SELL].begin();
  
  LevelBookHalfIter endIter = order->side == 'B' ? m_levelBook[BUY].end() : m_levelBook[SELL].end();

  while (iter != endIter) {
    if ((*iter)->intPrice == intPrice) { break; }
    iter++;
  }

  if (iter == endIter) {
    // LOG ERROR
    return;
  }

  Level *level = *iter;
  level->removeOrder(order);

  if (level->totalQty <= 0) {
    if (order->side == 'B') {
      m_levelBook[BUY].erase(iter);
    } else {
      m_levelBook[SELL].erase(iter);
    }

    delete level;
  }
}

inline
double Book::getPriceAtLevel(char side, int levelno) {
  int sideInt = FromSideCharToEnum(side);

  if (levelno >= m_levelBook[sideInt].size()) { return 0; /* error */}

  return IntPriceUtil::IntToDoublePrice(m_levelBook[sideInt][levelno]->intPrice);
}

inline
int Book::getQtyAtLevel(char side, int levelno) {
  int sideInt = FromSideCharToEnum(side);

  if (levelno >= m_levelBook[sideInt].size()) { return 0; /* error */}

  return m_levelBook[sideInt][levelno]->totalQty;
}

inline
std::vector<Order*> Book::getOrders(char side) {
  int sideInt = FromSideCharToEnum(side);
  std::vector<Order*> order_list;

  for (int i = 0; i < m_levelBook[sideInt].size(); i++) {
    for (auto elem : m_levelBook[sideInt][i]->orderList) {
      order_list.push_back(elem.second);
    }
  }

  return order_list;
}

inline
bool Book::addOrder(Order *order) {
  int id = order->id;

  if (m_orderMap.count(id) != 0) {
    // LOG ERROR
    return false;
  }

  m_orderMap.insert(std::pair<int, Order*>(id, order));

  insertToLevelBook(order);

  return true;
}

inline
bool Book::removeOrder(int id) {
  OrderMapIterator iter = m_orderMap.find(id);

  if (iter == m_orderMap.end()) {
    // LOG ERROR
    return false;
  }

  Order *order = (*iter).second;

  m_orderMap.erase(iter);
  removeFromLevelBook(order);

  delete order;

  return true;
}

inline
bool Book::modifyOrder(int id, int qty) {
  Order *order = getOrder(id);

  if (order == NULL) {
    // LOG ERROR
    return false;
  }

  int oldQty = order->quantity;

  order->quantity = qty;
  modifyLevelBook(order, oldQty);

  return true;
}

inline
Order* Book::getOrder(int id) {
  if (m_orderMap.count(id) == 0) {
    return NULL;
  }

  return m_orderMap[id];
}

#endif  // BOOK_H_
