#pragma once
#include "Graph.hpp"
#include <functional>

class SimpleSolver{
  private:
    const double M = 1e9;
    Graph G;
    double bij(std::function<double(int)> f, int l, int r, int x);
  public:
    SimpleSolver(Graph g);
    double Solve();
};
