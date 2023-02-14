#include "graph.hpp"

Node::Node(int l,int r,std::function<double(int)> F)
  :l_(l),r_(r),F_(F){};

Edge::Edge(int l,int r,std::function<double(int)> F,int u,int v)
  :u_(u),v_(v),l_(l),r_(r),F_(F){};

std::function<double(int)> Edge::Reverse(){
  F_=[f(F_)](int x){
    return f(-x);
  };
  return F_;
};

Graph::Graph(int n,int m,std::deque<Node>node,std::deque<Edge> edge)
  :n_(n),m_(m),node_(node),edge_(edge){};
