#include "MediumSolver.hpp"
#include "CostScaling.hpp"
#include <cmath>

#define l_ l
#define r_ u
#define u_ i
#define v_ j
#define F_ fn

MediumSolver::MediumSolver(Graph g){
  G=g;
}

double MediumSolver::bij(std::function<double(int)> f, int l, int r, int x) {
  if (x < l) {
    return -M;
  }
  if (x >= r) {
    return M;
  }
  return f(x + 1) - f(x);
}

double MediumSolver::Solve() {
  std::vector<std::function<double(int)>> functions;
  std::vector<std::pair<int, int>> limi;
  std::vector<InputEdge> edge;
  double mx=0;
  for (int i = 1; i <= G.n_; i++) {
    auto e = G.node_[i];
    if(e.l_>e.r_){
      return nan("");
    }
    mx+=std::max(e.F_(e.l_),e.F_(e.r_));
    for (int j = e.l_; j <= e.r_; j++) {
      double lower=bij(e.F_,e.l_,e.r_,j-1);
      double upper=bij(e.F_,e.l_,e.r_,j);
      if(lower>=0){
        edge.push_back({i,0,j,upper-lower,0,upper-lower,0});
      }
      else if(upper<=0){
        edge.push_back({0,i,-j,upper-lower,0,upper-lower,0});
      }
      else{
        edge.push_back({i,0,j,upper,0,upper,0});
        edge.push_back({0,i,-j,-lower,0,-lower,0});
      }
    }
    functions.push_back(e.F_);
    limi.push_back({e.l_, e.r_});
  }
  for (auto e : G.edge_) {  
    if(e.l_>e.r_){
      return nan("");
    }
    mx+=std::max(e.F_(e.l_),e.F_(e.r_));  
    std::function<double(int)> E;
    E = [e](int x) {
      int l = std::max(e.l_,x), r = e.r_;
      while (l < r) {
        int lmid = l + (r - l) / 3;
        int rmid = r - (r - l) / 3;
        if (e.F_(lmid) <= e.F_(rmid)) {
          r = rmid - 1;
        } else {
          l = lmid + 1;
        }
      }
      double minval = std::min(e.F_(l), e.F_(r));
      return minval;
    };
    e.F_=E;
    e.l_=std::min(e.l_,G.node_[e.u_].l_-G.node_[e.v_].r_);
    for(int i=e.l_;i<=e.r_;i++){
      double lower=bij(e.F_,e.l_,e.r_,i-1);
      double upper=bij(e.F_,e.l_,e.r_,i);
      if(lower>=0){
        edge.push_back({int(e.u_),int(e.v_),i,upper-lower,0,upper-lower,0});
      }
      else if(upper<=0){
        edge.push_back({int(e.v_),int(e.u_),-i,upper-lower,0,upper-lower,0});
      }
      else{
        edge.push_back({int(e.u_),int(e.v_),i,upper,0,upper,0});
        edge.push_back({int(e.v_),int(e.u_),-i,-lower,0,-lower,0});
      } 
    }
    functions.push_back(E);
    limi.push_back({e.l_, e.r_});
  }
  CostScaling cs;
  auto [success,res] = cs.MinCost(G.n_ + 1, edge);
  for(int i=0;i<functions.size();i++){
    auto FunctionMin = [](std::function<double(int)>f,int l,int r){
      while (l < r) {
        int lmid = l + (r - l) / 3;
        int rmid = r - (r - l) / 3;
        if (f(lmid) <= f(rmid)) {
          r = rmid - 1;
        } else {
          l = lmid + 1;
        }
      }
      double minval = std::min(f(l), f(r));
      return minval;
    };
    res+=FunctionMin(functions[i],limi[i].first,limi[i].second);
  }  
  if(res>mx){
    return nan("");
  }
  return res;
}
