#pragma once

#include <utility>
#include "Graph.hpp"

Graph TinyGen(
	int  n,
	int  mlimit,
	bool multi_edge,
	std::pair<int, int> node_limit,
	std::pair<int, int> edge_limit,
	std::pair<int, int> fun_limit,
	int seed = 0,
	bool gen_info = true
);
