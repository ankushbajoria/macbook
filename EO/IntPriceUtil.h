#ifndef INTPRICEUTIL_H_
#define INTPRICEUTIL_H_

struct IntPriceUtil {
  static const int PRICE_SCALE = 1000000;

  static int DoubleToIntPrice(double price) {
    return price * PRICE_SCALE;
  }

  static double IntToDoublePrice(int intPrice) {
    return ((double)intPrice) / PRICE_SCALE;
  }
};

#endif  // INTPRICEUTIL_H_
