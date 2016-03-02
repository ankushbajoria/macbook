#include <iostream>

#include <time.h>

int main(int argc, char** argv) {
  if (argc == 1) { std::cerr << "Error! must input n!" << std::endl; exit(-1); }

  size_t n = atoi(argv[1]);

  srand(time(NULL));

  uint32_t distanceMatrix[n][n];
  // fill the diagonal entries for the matrix
  for (size_t i = 0; i < n; i++) {
    distanceMatrix[i][i] = 0;
  }

  for (size_t i = 0; i < n; i++) {
    for (size_t j = i + 1; j < n ; j++) {
      distanceMatrix[i][j] = (rand() % 100) + 1;
      distanceMatrix[j][i] = distanceMatrix[i][j];
    }
  }

  std::cout << n << std::endl;
  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < n; j++) {
      std::cout << distanceMatrix[i][j] << " ";
    }

    std::cout << "\n";
  }

  // gemerate the load vector
  static const int NUM_MACHINES_AT_EACH_COLO = 1;
  for (size_t i = 0; i < n; i++) {
    uint32_t load = 0;
    for (size_t j = 0; j < NUM_MACHINES_AT_EACH_COLO; j++) {
      load += rand() % 11;
    }
    std::cout << load << std::endl;
  }

  std::cout << rand() % (n*5) << std::endl;

  return 0;
}
