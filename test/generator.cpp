#include "generator.hpp"

#include <cstdio>
#include <cmath>
#include <queue>
#include "testlib.h"
#include "gen.h"

thread_local static std::queue<std::tuple<double, double, double>> fun;

static std::function<Data(int)> RandF(std::pair<int,int> fun_limit, bool gen_info) {
	auto [l, r] = fun_limit;
	double a = rnd.next(4 * std::max(0, l), 4 * std::max(0, r)) / 4.0;
	double b = rnd.next(4 * l, 4 * r) / 4.0;
	double c = rnd.next(4 * l, 4 * r) / 4.0;
	if (gen_info)
		fun.push({a, b, c});
	return [a, b, c](int x) { return a * x * x + b * x + c; };
}

static std::string PrintFunction() {
	static char buf[90];
	auto [a, b, c] = fun.front(); fun.pop();
	sprintf(buf, "[](int x) { return %.2lf * x * x + %.2lf * x + %.2lf; }", a, b, c);
	return buf;
}

Graph TinyGen(
	int n,
	int mlimit,
	bool multi_edge,
	std::pair<int, int> node_limit,
	std::pair<int, int> edge_limit,
	std::pair<int, int> fun_limit,
	int seed,
	bool gen_info
) {
	if (!multi_edge) {
		mlimit = std::min(mlimit, n*(n-1)/2);
	}
	int m = rnd.next(n - 1, mlimit);
	
	Generator::Graph graph_gen;
	rnd.setSeed(seed);
	graph_gen.SetNode(n);
	graph_gen.SetSide(m);
	graph_gen.SetMultiplyEdge(multi_edge);
	graph_gen.GenGraph();
	
	std::vector<Node> nodes(1);
	nodes.reserve(n + 1);
	for (int i = 1; i <= n; i++) {
		auto l = rnd.next(node_limit.first, node_limit.second);
		auto u = rnd.next(node_limit.first, node_limit.second);
		if (l > u) std::swap(l, u);
		nodes.push_back({l, u, RandF(fun_limit, gen_info)});
	}
	
	std::vector<Edge> edges;
	edges.reserve(m);
	for (auto [i, j] : graph_gen.GetEdge()) {
		auto l = rnd.next(edge_limit.first, edge_limit.second);
		auto u = rnd.next(edge_limit.first, edge_limit.second);
		if (l > u) std::swap(l, u);
		edges.push_back({{l, u, RandF(fun_limit, gen_info)}, size_t(i + 1), size_t(j + 1)});
	}
	
	if (not gen_info)
		return Graph(n, m, std::move(nodes), std::move(edges));
	
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
	
	Graph g(n, m, std::move(nodes), std::move(edges));
	g.info = s;
	return g;
}
