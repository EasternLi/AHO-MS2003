#pragma once

#include "Problem.hpp"
#include "LCT.hpp"
#include "UniqueQueue.hpp"
#include <limits>
#include <numeric>
#include <cmath>

using Data = double;

class Flow {
public:
	Flow(size_t n, Data M, int U,
	     std::map<std::pair<size_t, size_t>, std::forward_list<Problem::Edge>> mp);
	
	Data min_cost();

private:
	struct Edge {
		size_t from, to;
		int l, u;
		Data flow;
		Fn fn;
		
		Data b(int x) { return fn(x + 1) - fn(x); }
	};
	
	void initialization();
	
	void push_all_admissible_edge();
	
	void choice_operator(size_t p);
	
	void tree_push(size_t p);
	
	void update_scaling(size_t p);
	
	// 将父节点设为当前边指向的节点
	void link(size_t p);
	
	// 断开与父节点的边
	void cut(size_t p);
	
	// maximum flow that can be sent
	Data q(size_t e_id);
	
	size_t n;
	Data M;
	Data ε;
	int U;
	
	std::vector<LCT::Node> nodes;
	std::vector<Data> imbalances;
	std::vector<size_t> current_edge;
	UniqueQueue uq;
	std::vector<std::vector<size_t>> sons; // tree_push 时剪边不会维护 sons，可能过多
	std::vector<size_t> fa; // -1 表该点为根
	
	std::vector<Flow::Edge> edges;
	std::vector<std::vector<size_t>> G;
	std::vector<Data> scaling;
};
