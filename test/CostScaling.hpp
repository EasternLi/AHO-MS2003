#include <vector>

struct InputEdge {
  int u, v, cost;
  double capacity, lower, upper;
  int id;
};
class CostScaling {
 private:
  struct Arc {
    int u, v, cost;
    double capacity, total;
    int id;
  };
  double epsilon, flow;
  std::vector<double> p;  // potential
  std::vector<int> head;
  std::vector<Arc> edge;
  int N, len, side;
  double total_cost = 0;

  double CostPi(int u, Arc e);

  bool Refine();
  void AddEdge(int u, int v, int c, double p, double tot, int id);

  void Init(std::vector<InputEdge> arc);

 public:
  std::pair<bool, double> MinCost(int n, std::vector<InputEdge> arc);
};
