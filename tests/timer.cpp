#include <boost/timer.hpp>
#include <cstdlib>
#include <iostream>

int main() {
  boost::timer timerClock;

  while (1) {
    timerClock.restart();
    ::sleep(1);
    std::cout << timerClock.elapsed() << std::endl;
  }

  return 1;
}
