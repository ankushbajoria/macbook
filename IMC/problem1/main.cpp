#include "Graph.h"

int main() {
  size_t n;
  std::cin >> n ;

  Graph graph(n);
  graph.read();

  int numCores;
  std::cin >> numCores ;
  std::cout << "\nrequired number of cores : " << numCores << std::endl << std::endl;

  std::list<uint32_t> allocatedMachines = graph.allocateMachines(numCores);
  typedef std::list<uint32_t>::iterator Iter;

  if (allocatedMachines.size() > 0) { 
    std::cout << "Following is a list of allocated machines:" << std::endl;
  }

  for (Iter itr = allocatedMachines.begin(); itr != allocatedMachines.end(); itr++) {
    std::cout << "Machine no. " << *itr << " with " << graph.freeCpu(*itr)  << " free cores." << std::endl;
  }

  std::cout << std::endl;
}

