#include "FeedHandler.h"

#include <fstream>
#include <string>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Usage: ./feedhandler input.txt" << std::endl;
    exit(-1);
  }

  FeedHandler feed(&std::cout);
  std::string line;

  const std::string filename(argv[1]);
  std::ifstream infile(filename.c_str(), std::ios::in);

  int counter = 0;

  while (std::getline(infile, line)) {
    feed.processMessage(line);
    if (++counter % 10 == 0) {
      // feed.printCurrentOrderBook(std::cout);
    }
  }

  // feed.printCurrentOrderBook(std::cout);

  return 0;
}


