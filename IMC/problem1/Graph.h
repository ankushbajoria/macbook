#pragma once

#include "Edge.h"

#include <algorithm>
#include <iostream>
#include <list>
#include <queue>
#include <unordered_set>
#include <vector>

/*
 * author: abajoria
 * date: 20140622
 * */

class Graph {
  static const uint32_t NUM_CPUS = 10;
  static const uint32_t INFINITE = -1;

  private:
    std::vector<std::vector<uint32_t> > m_matrix; 
    size_t m_numNodes;
    std::vector<uint32_t> m_freeVector; // vector of num of free cpu at each machine
    uint32_t            m_masterNode; // node with maximum score value
    std::vector<uint32_t> m_minDistanceFromMaster;

    void setupMasterNode();

  public:
    Graph(size_t n);

    void read();
    uint32_t distance(int from, int to);
    std::list<uint32_t> allocateMachines(int n /* numCoresRequired */);
    uint32_t freeCpu(uint32_t nodeId) { return m_freeVector[nodeId]; }
};

Graph::Graph(size_t n)
  : m_numNodes(n) {
  m_matrix.resize(n, std::vector<uint32_t>(n, 0));
  m_freeVector.resize(n, 0);

  // initialize the matrix
  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < n; j++) {
      m_matrix[i][j] = 0;
    }
  } // Done.
}

void Graph::read() {
  for (uint32_t i = 0ul; i < m_numNodes; i++) {
    for (uint32_t j = 0ul; j < m_numNodes; j++) {
      std::cin >> m_matrix[i][j];
    }
  }

  for (int i = 0; i < m_numNodes; i++) {
    uint32_t load;
    std::cin >> load;
    m_freeVector[i] = (NUM_CPUS - load);
  }

  setupMasterNode();
  std::cout << "\nreading of graph completed!" << std::endl;
}

uint32_t Graph::distance(int from, int to) {
  if (from >= m_numNodes || to >= m_numNodes) {
    return 0;
  }

  return m_matrix[from][to];
}

// This decides the node always present in the 
// solution. This is the node with the strongest connectivity
// in terms of free_cpu/distance from nodes
void Graph::setupMasterNode() {
  double max = 0;

  for (size_t i = 0; i < m_numNodes; i++) {
    double score = m_freeVector[i] * 2; // Self free cpu number is added a weight of 10

    for (size_t j = 0; j < m_numNodes; j++) {
      if (i == j) { continue; }
      score += ( (double) m_freeVector[j]) / distance(i,j);
    }

    if (max < score) {
      max = score;
      m_masterNode = i;
    }
  }
}

std::list<uint32_t> Graph::allocateMachines(int n) {
  typedef std::priority_queue<Edge, std::vector<Edge>, CompareEdges> PriorityQueueType;
  PriorityQueueType queueForEdges;

  std::list<uint32_t> allocatedMachines;
  bool added[m_numNodes];

  // initialize added array
  for (uint32_t i = 0; i < m_numNodes; i++) {
    added[i] = false;
  }
  added[m_masterNode] = true;

  allocatedMachines.push_back(m_masterNode);
  n -= m_freeVector[m_masterNode];

  for (uint32_t i = 0; i < m_numNodes; i++) {
     Edge e;
     if (i != m_masterNode && distance(m_masterNode,i) != INFINITE) {
      e.from = m_masterNode;
      e.to = i;
      e.length = distance(m_masterNode, i);
      e.weight = ((double) m_freeVector[i]) / e.length;

      queueForEdges.push(e);
    }
  }
  
  while (n > 0 && !queueForEdges.empty()) {
    Edge e = queueForEdges.top();
    queueForEdges.pop();

    if (!added[e.to]) {
      allocatedMachines.push_back(e.to);
      n -= m_freeVector[e.to];
      added[e.to] = true;

      for (uint32_t i = 0; i < m_numNodes; i++) {
        if (i != e.to && distance(e.to, i) != INFINITE && !added[i] ) {
          Edge edgeToAdd;
          edgeToAdd.from = e.to;
          edgeToAdd.to = i;
          edgeToAdd.length = distance(e.to, i);
          edgeToAdd.weight = ((double) m_freeVector[i])/edgeToAdd.length;

          queueForEdges.push(edgeToAdd);
        }
      }
    }
  }
   
  if (n > 0) {
    std::cerr << "Unable to allocate machines!" << std::endl;
    return std::list<uint32_t>();
  }

  return allocatedMachines;
}


