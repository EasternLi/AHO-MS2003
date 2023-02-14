#pragma once

#include <vector>
#include <string>
#include "ProblemDefine.hpp"

using Node = AHO_MS2003::μLimit;
using Edge = AHO_MS2003::ωLimit;

class Graph{
public:
	int n_,m_;
	std::vector<Node> node_;
	std::vector<Edge> edge_;
	std::string info;
	
	Graph() = default;
	Graph(int n, int m, std::vector<Node>node, std::vector<Edge> edge);
	
	void prettyPrint(std::string &out) const;
};
