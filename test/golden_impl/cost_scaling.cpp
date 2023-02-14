#include "cost_scaling.hpp"

double CostScaling::CostPi(int u, Arc e) {
  return e.cost - p[u] + p[e.v];
};

bool CostScaling::Refine() {
  std::vector<double> excess(N);
  std::vector<double> incp(N, 0);
  for (int u = 0; u < N; u++) {
    for (int i = head[u]; i != -1; i = edge[i].u) {
      if (CostPi(u, edge[i]) < 0) {
        excess[u] -= edge[i].capacity;
        excess[edge[i].v] += edge[i].capacity;
        edge[i ^ 1].capacity += edge[i].capacity;
        edge[i].capacity = 0;
      }
    }
  }
  std::vector<bool> vis(N, 0);
  std::stack<int> st;
  for (int u = 0; u < N; u++) {
    if (excess[u] > 0) {
      st.push(u);
      vis[u] = 1;
    }
  }

  auto Push = [&](int u, int x, int flow) {
    edge[x].capacity -= flow;
    edge[x ^ 1].capacity += flow;
    excess[u] -= flow;
    excess[edge[x].v] += flow;
    if (excess[edge[x].v] > 0 && vis[edge[x].v] == 0) {
      st.push(edge[x].v);
      vis[edge[x].v] = 1;
    }
    for (int i = 0; i < edge.size(); i++) {
      assert(edge[i].capacity + edge[i ^ 1].capacity == edge[i].total);
    }
  };

  auto Relabel = [&](int u) {
    p[u] += epsilon / 2;
    incp[u] += epsilon / 2;
  };

  auto Modify = [&](int u) {
    assert(excess[u] > 0);
    for (int i = head[u]; i != -1; i = edge[i].u) {
      double w = CostPi(u, edge[i]);
      if (w < 0) {
        Push(u, i, std::min(edge[i].capacity, excess[u]));
      }
      if (excess[u] == 0) {
        return;
      }
    }
    assert(excess[u] > 0);
    Relabel(u);
    st.push(u);
    vis[u] = 1;
  };
  while (!st.empty()) {
    int u = st.top();
    st.pop();
    vis[u] = 0;
    if (incp[u] > 3 * N * epsilon) {
      return false;
    }
    Modify(u);
  }
  return true;
};

void CostScaling::AddEdge(int u, int v, int c, double p, double tot, int id) {
  edge.push_back({head[u], v, c, p, tot, id});
  len++;
  head[u] = len;
};

void CostScaling::Init(std::vector<InputEdge> arc) {
  edge.clear();
  head.clear();
  p.clear();
  total_cost = 0;
  epsilon = 1;
  flow = 0;
  head.resize(N, -1);
  len = -1;
  for (auto e : arc) {
    epsilon = std::max(epsilon, e.cost * 1.0);
    total_cost += e.capacity * e.cost;
    AddEdge(e.u, e.v, e.cost, e.capacity, e.capacity, -1);
    AddEdge(e.v, e.u, -e.cost, 0, e.capacity, e.id);
  }
  p.resize(N, 0);
};

std::pair<bool, double> CostScaling::MinCost(int n,std::vector<InputEdge> arc) {
  N = n;
  side = arc.size();
  Init(arc);
  while (epsilon * N >= 1.0) {
    if (Refine() == false) {
      return {false, 0};
    }
    epsilon = epsilon / 2;
  }
  for (auto e : edge) {
    total_cost -= e.capacity * e.cost;
  }
  return {true, -total_cost / 2};
};