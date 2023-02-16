#include "generator.hpp"

#include <cstdio>
#include <cmath>
#include <queue>
#include "testlib.h"
#include "gen.h"

thread_local static std::queue<std::tuple<int, int, int>> fun;

static std::function<double(int)> RandF(std::pair<int,int> fun_limit) {
	auto [l, r] = fun_limit;
	int a = rnd.next(std::max(0, l), std::max(0, r));
	int b = rnd.next(l, r);
	int c = rnd.next(l, r);
	fun.push({a, b, c});
	return [a, b, c](int x) { return a * x * x + b * x + c; };
}

static std::string PrintFunction() {
	static char buf[80];
	auto [a, b, c] = fun.front(); fun.pop();
	sprintf(buf, "[](int x) { return %3d * x * x + %3d * x + %3d; }", a, b, c);
	return buf;
}
		
Graph TinyGen(
	int n,
	std::pair<int, int> mlimit,
	bool multi_edge,
	std::pair<int, int> node_limit,
	std::pair<int, int> edge_limit,
	std::pair<int, int> fun_limit,
	int seed
) {
	mlimit.first=std::max(1,mlimit.first);
	if (!multi_edge) {
		mlimit.second = std::min(mlimit.second, n*(n-1)/2);
	}
	int m = rnd.next(mlimit.first, mlimit.second);
	
	Generator::Graph graph_gen;
	rnd.setSeed(seed);
	graph_gen.SetNode(n);
	graph_gen.SetSide(m);
	graph_gen.SetMultiplyEdge(multi_edge);
	graph_gen.GenGraph();
	
	std::vector<Node> nodes(1);
	for (int i = 1; i <= n; i++) {
		auto l = rnd.next(node_limit.first, node_limit.second);
		auto u = rnd.next(node_limit.first, node_limit.second);
		if (l > u) std::swap(l, u);
		nodes.push_back({l, u, RandF(fun_limit)});
	}
	
	std::vector<Edge> edges;
	for (auto [i, j] : graph_gen.GetEdge()) {
		auto l = rnd.next(edge_limit.first, edge_limit.second);
		auto u = rnd.next(edge_limit.first, edge_limit.second);
		if (l > u) std::swap(l, u);
		edges.push_back({{l, u, RandF(fun_limit)}, size_t(i + 1), size_t(j + 1)});
	}
	
	std::string s;
	s += "{\n"; {
		s += "\t"; s += std::to_string(n); s += "\n";
		
		s += "\t{\n\t\t{}\n"; for (int i = 1; i <= n; i++) {
			s += "\t\t{";
			s += std::to_string(nodes[i].l);
			s += ", ";
			s += std::to_string(nodes[i].u);
			s += ", ";
			s += PrintFunction();
			s += "}\n";
		} s += "\t}\n";
		
		s += "\t{\n"; for (int i = 0; i < m; i++) {
			s += "\t\t{{";
			s += std::to_string(edges[i].l);
			s += ", ";
			s += std::to_string(edges[i].u);
			s += ", ";
			s += PrintFunction();
			s += "}";
			s += ", ";
			s += std::to_string(edges[i].i);
			s += ", ";
			s += std::to_string(edges[i].j);
			s += "}\n";
		} s += "\t}\n";
	} s += "}\n";
	
	Graph g(n, m, nodes, edges);
	g.info = s;
	return g;
}
		
Graph TinyGen(
	int n,
	std::pair<int, int> mlimit,
	bool multi_edge,
	std::pair<int, int> node_limit,
	std::pair<int, int> edge_limit,
	std::pair<int, int> fun_limit
) {
	return TinyGen(n, mlimit, multi_edge, node_limit, edge_limit, fun_limit, 0);
}
