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
  Book(std::ostream* os);

  // modifiers
  bool addOrder(Order *order);
  bool removeOrder(int id);
  bool modifyOrder(int id, int qty, double price);

  bool processTick(double px, int qty);

  // LevelBook accessors
  double              getPriceAtLevel(char side, int levelno) const;
  int                 getQtyAtLevel(char side, int levelno) const;
  size_t              numOrdersAtLevel(char side, int levelno) const;
  std::vector<Order*> getOrders(char side) const;

  int     runningTickVolume() const { return m_lastPxVolume; }
  double  lastTickPx()        const { return m_lastPx; }

 private:
  // Helper Functions
  int FromSideCharToEnum(char side) const;
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

  int m_lastPxVolume = 0;
  double m_lastPx = 0.0;
};

inline
Book::Book(std::ostream* os) 
  : m_orderMap()
{ }

inline
int Book::FromSideCharToEnum(char side) const {
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
double Book::getPriceAtLevel(char side, int levelno) const {
  int sideInt = FromSideCharToEnum(side);

  if (levelno >= m_levelBook[sideInt].size()) { return 0; /* error */}

  return IntPriceUtil::IntToDoublePrice(m_levelBook[sideInt][levelno]->intPrice);
}

inline
int Book::getQtyAtLevel(char side, int levelno) const {
  int sideInt = FromSideCharToEnum(side);

  if (levelno >= m_levelBook[sideInt].size()) { return 0; /* error */}

  return m_levelBook[sideInt][levelno]->totalQty;
}

inline
std::vector<Order*> Book::getOrders(char side) const {
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
size_t Book::numOrdersAtLevel(char side, int levelno) const {
  int sideInt = FromSideCharToEnum(side);

  if (levelno >= m_levelBook[sideInt].size()) { return 0; /* error */ }

  return m_levelBook[sideInt][levelno]->orderList.size();
}

inline
bool Book::processTick(double px, int qty) {
  // tick hitting the best ask
  if (m_lastPx != px) {
    m_lastPx = px;
    m_lastPxVolume = qty;
  } else {
    m_lastPxVolume += qty;
  }

  Level* level = nullptr;
  int idx = 0;
  int side = 0;

  if (m_levelBook[SELL].size() && 
      IntPriceUtil::DoubleToIntPrice(px) == m_levelBook[SELL][0]->intPrice) {
    level = m_levelBook[SELL][0];
    idx = 0;
    side = SELL;
  } else if (m_levelBook[BUY].size() &&
             IntPriceUtil::DoubleToIntPrice(px) == m_levelBook[BUY][0]->intPrice) {
    level = m_levelBook[BUY][0];
    idx = 0;
    side = BUY;
  } else {
    return false; // error
  }

  if (level->totalQty < qty) {
    return false;
  }

  auto iter = level->orderList.begin();
  while (qty > 0) {
    auto order = (*iter).second;

    if (order->quantity <= qty) {
      level->totalQty -= order->quantity;
      qty -= order->quantity;
      iter = level->orderList.erase(iter);

      m_orderMap.erase(order->id);
      delete order;
    } else {
      order->quantity -= qty;
      level->totalQty -= qty;
      qty = 0;
    }
  }

  // remove from book if level size is 0
  if (level->totalQty == 0) {
    m_levelBook[side].erase(m_levelBook[side].begin());
    delete level;
  }

  return true;
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
bool Book::modifyOrder(int id, int qty, double price) {
  Order *order = getOrder(id);

  // We only support modify to alter qty;
  if (order == NULL || order->price != price) {
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
