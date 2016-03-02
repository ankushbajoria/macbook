#pragma once

#include "StringHelper.h"

struct MessageDetails {
  char  action;
  int   orderid;
  char  side;
  size_t qty;
  double px;

  explicit MessageDetails(const std::string& input) {
    auto token = StringHelper::splitString(input, ',');

    if (token.size() != 5) {
      // LOG ERROR
      return;
    }

    try {
      action  = token[0][0];
      orderid = std::stoi(token[1]);
      side    = token[2][0];
      qty     = std::stoi(token[3]);
      px      = std::stod(token[4]);
    } catch (const std::exception&) {
      // LOG ERROR
      action = 'U'; // indicate to FeedHandler of error
    }

    // sanity check
    if (orderid < 0 ||
        (side != 'B' && side != 'S') ||
        (px < 0) ||
        std::stoi(token[3]) < 0) {
      action = 'U';
    }
  }
};

struct TickMessage {
  size_t qty;
  double px;

  explicit TickMessage(const std::string& input) {
    auto token = StringHelper::splitString(input, ',');

    if (token.size() != 3) {
      // LOG ERROR
      return;
    }

    try {
      qty = std::stoi(token[1]);
      px  = std::stod(token[2]);
    } catch (const std::exception&) {
      // Indicate error to FeedHandler
      qty = 0;
      px = -1.0;
    }
  }
};
