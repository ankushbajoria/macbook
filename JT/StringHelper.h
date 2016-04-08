#pragma once

#include <string>
#include <vector>

struct StringHelper {
  static std::vector<std::string>
  splitString(std::string input, char delimiter) {
    // remove everything after space
    auto spacePos = input.find(" ");
    if (spacePos != std::string::npos) {
      input.erase(spacePos);
    }
  
    std::vector<std::string> ret;
    int prevPos = 0;
    int pos = input.find(delimiter);
  
    while (pos != std::string::npos) {
      ret.push_back(input.substr(prevPos, pos - prevPos));
      prevPos = pos + 1;
      pos = input.find(delimiter, prevPos);
    }
  
    ret.push_back(input.substr(prevPos));
  
    return ret;
  }
};
