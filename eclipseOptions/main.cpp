#include "Book.h"

#include <cassert>
#include <iostream>

int main() {
  Book book;

  // lambda to print orders
  auto printAllOrders = [&] {
    std::cout << "Level 1:" << std::endl;
    std::cout << "\t" << book.getQtyAtLevel('B', 0)
              << " " << book.getPriceAtLevel('B', 0) 
              << " | "
              << book.getPriceAtLevel('S', 0)
              << " " << book.getQtyAtLevel('S', 0)
              << std::endl
              << std::endl;

    std::cout << "SELL:" << std::endl;
    std::cout << "---------------" << std::endl;
    for (auto o : book.getOrders('S')) {
      std::cout
        << " id:" << o->id
        << " prc:" << o->price
        << " qty:" << o->quantity 
        << std::endl;
    }

    std::cout << std::endl;

    std::cout << "BUY:" << std::endl;
    std::cout << "---------------" << std::endl;
    for (auto o : book.getOrders('B')) {
      std::cout
        << " id:" << o->id
        << " prc:" << o->price
        << " qty:" << o->quantity 
        << std::endl;
    }

    std::cout << std::endl;
  };

  printAllOrders();

  // Insert Sell Orders
  Order *order = new Order(1, 'S', 10.0, 2);
  book.addOrder(order);
  
  order = new Order(2, 'S', 11.0, 2);
  book.addOrder(order);

  order = new Order(3, 'S', 9.0, 2);
  book.addOrder(order);

  // Insert BUY orders
  order = new Order(4, 'B', 11.0, 2);
  book.addOrder(order);

  order = new Order(5, 'B', 11.0, 2);
  book.addOrder(order);

  order = new Order(6, 'B', 10.0, 2);
  book.addOrder(order);

  order = new Order(7, 'B', 9.0, 2);
  book.addOrder(order);

  book.modifyOrder(7, 5);
  printAllOrders();

  book.removeOrder(4);
  book.removeOrder(5);

  printAllOrders();

  book.modifyOrder(6, 10);

  printAllOrders();

  return 0;
}
