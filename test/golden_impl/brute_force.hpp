#pragma once

#include "graph.hpp"

class  BruteForce
{
  public:
    Graph G_;
    BruteForce(){};
    BruteForce(Graph G);
    void ChangeGraph(Graph G);
    double Solve(std::vector<int> &v);
};