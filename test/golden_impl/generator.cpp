#include"generator.hpp"
#include <iostream>
#include <map>
#include <cmath>
#include "gen.h"
#include "testlib.h"
struct Fun {
  int a, b, c;
};
std::vector<Fun> fun;

std::function<double(int)> RandF(std::pair<int,int> fun_limit) {
  auto [l,r]=fun_limit;
  int a = rnd.next(std::max(0,l),std::max(0,r));
  int b = rnd.next(l,r);
  int c = rnd.next(l,r);
  fun.push_back({a, b, c});
  return [a, b, c](int x) { return a * x * x + b * x + c; };
}

void PrintFunction(char * buf,int x) {
  sprintf(buf," ( %d ) * x * x + ( %d ) * x + ( %d )", fun[x].a, fun[x].b, fun[x].c);
}

std::string PrintCase(Graph& G) {
  char buf[50000];
  sprintf(buf,"%d ,\n{\n", G.n_);
  sprintf(buf,"  {0,0,[](int){ return 0;}},\n");
  for (int i = 1; i <= G.n_; i++) {
    sprintf(buf,"  {%d,%d,[](int x){ return", G.node_[i].l_, G.node_[i].r_);
    PrintFunction(buf,i - 1);
    sprintf(buf,";}},\n");
  }
  sprintf(buf,"},\n{\n");
  for (int i = 0; i < G.m_; i++) {
    sprintf(buf,"  {{%d,%d,[](int x){ return", G.edge_[i].l_, G.edge_[i].r_);
    PrintFunction(buf,i + G.n_);
    sprintf(buf,";}},%d,%d},\n", G.edge_[i].u_, G.edge_[i].v_);
  }
  sprintf(buf,"}\n");
  return std::string(buf);
}

Graph TinyGen(int n, int mlimit, bool multi_edge,std::pair<int,int> node_limit,std::pair<int,int> edge_limit,std::pair<int,int> fun_limit,int seed) {
  fun.clear();
  if(!multi_edge){
    mlimit=std::min(mlimit,n*(n-1)/2);
  }
  int m = rnd.next(n - 1, mlimit);
  Generator::Graph graph;
  rnd.setSeed(seed);
  graph.SetNode(n);
  graph.SetSide(m);
  graph.SetMultiplyEdge(multi_edge);
  graph.GenGraph();
  std::vector<std::pair<int, int>> e = graph.GetEdge();
  Graph nxt;
  nxt.n_ = n;
  nxt.m_ = m;
  nxt.node_.resize(n + 1);
  nxt.edge_.resize(m);
  for (int i = 1; i <= n; i++) {
    auto [l,r]=node_limit;
    nxt.node_[i].l_ = rnd.next(l,r);
    nxt.node_[i].r_ = rnd.next(l,r);
    if (nxt.node_[i].l_ > nxt.node_[i].r_) {
      std::swap(nxt.node_[i].l_, nxt.node_[i].r_);
    }
    nxt.node_[i].F_ = RandF(fun_limit);
  }
  std::map<std::pair<int, int>, int> mp;
  for (int i = 0; i < m; i++) {
    auto [l,r]=edge_limit;
    nxt.edge_[i].l_ = rnd.next(l,r);
    nxt.edge_[i].r_ = rnd.next(l,r);
    if (nxt.edge_[i].l_ > nxt.edge_[i].r_) {
      std::swap(nxt.edge_[i].l_, nxt.edge_[i].r_);
    }
    nxt.edge_[i].F_ = RandF(fun_limit);
    if (mp[e[i]]) {
      e[i] = e[mp[e[i]] - 1];
    } else {
      mp[e[i]] = i + 1;
      mp[std::make_pair(e[i].second, e[i].first)] = i + 1;
    }
    nxt.edge_[i].u_ = e[i].first + 1;
    nxt.edge_[i].v_ = e[i].second + 1;
  }
  nxt.info=PrintCase(nxt);
  return nxt;
}

Graph TinyGen(int n, int mlimit, bool multi_edge,std::pair<int,int> node_limit,std::pair<int,int> edge_limit,std::pair<int,int> fun_limit){
  return TinyGen(n,mlimit,multi_edge,node_limit,edge_limit,fun_limit,0);
}