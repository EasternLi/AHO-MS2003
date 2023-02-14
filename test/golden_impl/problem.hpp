#pragma once
#include "graph.hpp"
#include <functional>

class Problem{
  private:
    const double M = 1e9;
    Graph G;
    double bij(std::function<double(int)> f, int l, int r, int x);
  public:
    Problem(Graph g);
    double Solve();
};