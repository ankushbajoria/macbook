#include <stdint.h>

/*
 * author: abajoria
 * date: 20140622
 * */

struct Edge {
  uint32_t from;
  uint32_t to;
  uint32_t length;
  double weight;
};

class CompareEdges {
  public:
    bool operator() (const Edge& e1, const Edge& e2) {
      return e1.weight < e2.weight;
    }
};
