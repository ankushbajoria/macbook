#ifndef ORDER_H_
#define ORDER_H_

struct Order {
  int id;
  char side;
  double price;
  int quantity;

  Order(int id, int side, int price, int quantity)
    : id(id),
      side(side),
      price(price),
      quantity(quantity)
  { }
};

#endif  // ORDER_H_
