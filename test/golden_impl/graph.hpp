#pragma once

#include <queue>
#include <functional>
#include <string>
class Node{
  public:
    int l_,r_;
    std::function<double(int)> F_;
    Node(){};
    Node(int l,int r,std::function<double(int)> F);
};

class Edge{
  public:
    int l_,r_;
    std::function<double(int)> F_;
    int u_,v_;
    Edge(){};
    Edge(int l,int r,std::function<double(int)> F,int u,int v);
    std::function<double(int)> Reverse();
};

class Graph{
  public:
    int n_,m_;
    std::deque<Node> node_;
    std::deque<Edge> edge_;
    int U_;
    std::string info;
    Graph(){};
    Graph(int n,int m,std::deque<Node>node,std::deque<Edge> edge);
};