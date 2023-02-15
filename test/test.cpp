#include <cmath>
#include <testa.hpp>
#include <ProblemSolver.hpp>
#include "Graph.hpp"
#include "SimpleSolver.hpp"
#include "generator.hpp"

namespace AHO_MS2003 {

class Test {
public:
	// Complexity: O(U^n * m)
	static std::optional<Data> bf_solve(const Graph& g) {
		auto& [n, _m, μs, _ωs, _info] = g;
		auto ωs = _ωs;
		Data ret = INFINITY;
		
		for(auto& it : ωs) it.pre_processing();
		
		std::vector<int> v(n + 1);
		std::function<void(size_t, Data)> dfs = [&](size_t p, Data now_sum) {
			if (p == n) {
				for (auto& it : ωs) {
					int x = v[it.i] - v[it.j];
					if (x > it.u)
						return;
					now_sum += it.fn(x);
				}
				ret = std::min(ret, now_sum);
				return;
			}
			p++;
			for (auto &i = v[p] = μs[p].l; i <= μs[p].u; i++)
				dfs(p, now_sum + μs[p].fn(i));
		};
		dfs(0, 0);
		
		if (ret == INFINITY)
			return std::nullopt;
		return {ret};
	}
	// Complexity: O(n * m * U * log^2)
	static std::optional<Data> simple_solve(const Graph& g) {
		SimpleSolver ms(g);
		auto ret = ms.Solve();
		
		if (std::isnan(ret))
			return std::nullopt;
		return {ret};
	}
	static std::optional<Data> fast_solve(const Graph& g) {
		auto& [n, _m, μs, ωs, _info] = g;
		AHO_MS2003::ProblemSolver ps(n, μs, ωs);
		return ps.solve();
	}
};

}

using AHO_MS2003::Test;

TESTA_DEF_EQ_1(
	empty_graph,
	Test::bf_solve,
	Test::fast_solve,
	(Graph{
		0,
		0,
		{{}},
		{}
	})
);

TESTA_DEF_EQ_1(
	single_node_graph_1,
	Test::bf_solve,
	Test::fast_solve,
	(Graph{
		1,
		0,
		{
			{},
			{-9, 9, [](int) { return 1; }},
		},
		{}
	})
);

TESTA_DEF_EQ_1(
	single_node_graph_2,
	Test::bf_solve,
	Test::fast_solve,
	(Graph{
		1,
		0,
		{
			{},
			{-9, 9, [](int x) { return (x - 1.2) * (x - 1.2) + 3.4; }},
		},
		{}
	})
);

TESTA_DEF_EQ_1(
	mini_graph_1,
	Test::bf_solve,
	Test::fast_solve,
	(Graph{
		2,
		1,
		{
			{},
			{-2, 2, [](int x) { return -x; }},
			{-2, 2, [](int x) { return +x; }},
		},
		{
			{{-4, 4, [](int x) { return (x - 3) * (x - 3); }}, 1, 2},
		}
	})
);

TESTA_DEF_EQ_1(
	mini_graph_2,
	Test::bf_solve,
	Test::fast_solve,
	(Graph{
		2,
		1,
		{
			{},
			{-2, 2, [](int x) { return -x; }},
			{-2, 2, [](int x) { return +x; }},
		},
		{
			{{-4, 4, [](int x) { return (x + 3) * (x + 3); }}, 1, 2},
		}
	})
);

TESTA_DEF_EQ_1(
	mini_graph_3,
	Test::bf_solve,
	Test::fast_solve,
	(Graph{
		3,
		3,
		{
			{},
			{ 0, 0, [](int x) { return 5 * x * x + -9 * x + -6; }},
			{ 0, 1, [](int x) { return 8 * x * x +  8 * x + -6; }},
			{-2, 1, [](int x) { return 4 * x * x +  6 * x + 10; }},
		},
		{
			{{-2, 3, [](int x) { return 1 * x * x + -4 * x +  9; }}, 2, 1},
			{{-1, 3, [](int x) { return 2 * x * x +  7 * x +  2; }}, 3, 2},
			{{ 1, 2, [](int x) { return 2 * x * x +  8 * x + -2; }}, 3, 1},
		}
	})
);

TESTA_DEF_EQ_1(
	mini_graph_4,
	Test::bf_solve,
	Test::fast_solve,
	(Graph{
		4,
		4,
		{
			{},
			{-2, 0, [](int x) { return 2 * x * x +  9 * x + 4; }},
			{ 1, 2, [](int x) { return 5 * x * x + -1 * x + 6; }},
			{-1, 0, [](int x) { return 0 * x * x + 10 * x + 5; }},
			{ 0, 0, [](int x) { return 0 * x * x + 10 * x + 5; }},
		},
		{
			{{ 0, 7, [](int x) { return  4 * x * x + 9 * x + -1; }}, 2, 4},
			{{ 2, 9, [](int x) { return  8 * x * x + 5 * x + -7; }}, 1, 4},
			{{ 5, 9, [](int x) { return 10 * x * x + 9 * x +  4; }}, 3, 4},
			{{-2, 1, [](int x) { return  8 * x * x + 5 * x +  8; }}, 1, 2},
		}
	})
);

TESTA_DEF_EQ_1(
	mini_graph_5,
	Test::bf_solve,
	Test::fast_solve,
	(Graph{
		5,
		6,
		{
			{},
			{ -8,   0, [](int x) { return 5 * x * x +  2 * x +  1; }},
			{ -6,   3, [](int x) { return 4 * x * x + -8 * x +  4; }},
			{-10, -10, [](int x) { return 5 * x * x +  9 * x + -4; }},
			{  4,   5, [](int x) { return 8 * x * x +  1 * x +  0; }},
			{ -3,   0, [](int x) { return 1 * x * x +  6 * x +  5; }},
		},
		{
			{{10, 12, [](int x) { return 5 * x * x +  4 * x + -2; }}, 4, 3},
			{{10, 15, [](int x) { return 7 * x * x +  0 * x + -5; }}, 3, 2},
			{{17, 20, [](int x) { return 1 * x * x + -3 * x +  0; }}, 3, 5},
			{{12, 18, [](int x) { return 0 * x * x + 10 * x + 10; }}, 5, 1},
			{{17, 19, [](int x) { return 1 * x * x + -7 * x +  6; }}, 3, 1},
			{{13, 20, [](int x) { return 3 * x * x + -3 * x + -8; }}, 4, 5},
		}
	})
);

TESTA_DEF_EQ_1(
	no_solution_1,
	Test::bf_solve,
	Test::fast_solve,
	(Graph{
		2,
		1,
		{
			{},
			{0, 0, [](int) { return 0; }},
			{0, 0, [](int) { return 0; }},
		},
		{
			{{-1, -1, [](int) { return 0; }}, 1, 2},
		}
	})
);

TESTA_DEF_EQ_1(
	no_solution_2,
	Test::bf_solve,
	Test::fast_solve,
	(Graph{
		2,
		2,
		{
			{},
			{0, 3, [](int) { return 0; }},
			{0, 3, [](int) { return 0; }},
		},
		{
			{{ 0,  1, [](int) { return 0; }}, 1, 2},
			{{-3, -2, [](int) { return 0; }}, 2, 1},
		}
	})
);

TESTA_DEF_EQ_1(
	no_solution_3,
	Test::bf_solve,
	Test::fast_solve,
	(Graph{
		3,
		2,
		{
			{},
			{0, 0, [](int) { return 0; }},
			{0, 1, [](int) { return 0; }},
			{0, 0, [](int) { return 0; }},
		},
		{
			{{-1, -1, [](int) { return 0; }}, 1, 2},
			{{ 0,  0, [](int) { return 0; }}, 2, 3},
		}
	})
);

using std::string;
using std::function;
using std::tuple;
using std::optional;

void SmallGen1(const string &name, function<void(const Graph&)> cs) {
	int n = (name.substr(0, 7) == "Compare" ? 10000 : 100);
	for(int i = 1; i <= n; i++) {
		cs(TinyGen(5, 10, 0, { -2, 2 }, { -2, 6 }, { -10, 10 }, i));
	}
}

void SmallGen2(const string &name, function<void(const Graph&)> cs) {
	int n = (name.substr(0, 7) == "Compare" ? 10000 : 100);
	for(int i = 1; i <= n; i++) {
		cs(TinyGen(4, 10, 1, { -2, 2 }, { -2, 6 }, { -10, 10 }, i));
	}
}

void HugeGen1(const string &name, function<void(const Graph&)> cs) {
	int n = (name.substr(0, 7) == "Compare" ? 100 : 1);
	for(int i = 1; i <= n; i++) {
		cs(TinyGen(50, 100, 0, { -20, 20 }, { -20, 60 }, { -100, 100 }, i));
	}
}

void HugeGen2(const string &name, function<void(const Graph&)> cs) {
	int n = (name.substr(0, 7) == "Compare" ? 100 : 1);
	for(int i = 1; i <= n; i++) {
		cs(TinyGen(50, 200, 1, { -20, 20 }, { -20, 60 }, { -100, 100 }, i));
	}
}

void HugeGen3(const string &name, function<void(const Graph&)> cs) {
	int n = (name.substr(0, 7) == "Compare" ? 10 : 1);
	for(int i = 1; i <= n; i++) {
		cs(TinyGen(100, 300, 1, { -40, 40 }, { -40, 40 }, { -100, 100 }, i));
	}
}

void BFVerifier(const optional<Data> &res, const Graph &in) {
	auto ans = Test::bf_solve(in);
	TESTA_ASSERT(res == ans)
		(res)
		(ans)
		(in.info)
		.issue();
}

void SimpleVerifier(const optional<Data> &res, const Graph &in) {
	auto ans = Test::simple_solve(in);
	TESTA_ASSERT(res == ans)
		(res)
		(ans)
		(in.info)
		.issue();
}

void ImproveVerifier(const optional<Data> &res, const Graph &in) {
	auto ans = Test::fast_solve(in);
	TESTA_ASSERT(res == ans)
		(res)
		(ans)
		(in.info)
		.issue();
}


TESTA_DEF_VERIFY_WITH_TB(SimpleCorrectTest1, SmallGen1, BFVerifier, Test::simple_solve);
TESTA_DEF_VERIFY_WITH_TB(ImproveCorrectTest1, SmallGen1, BFVerifier, Test::fast_solve);
TESTA_DEF_VERIFY_WITH_TB(SimpleCorrectTest2, SmallGen2, BFVerifier, Test::simple_solve);
TESTA_DEF_VERIFY_WITH_TB(ImproveCorrectTest2, SmallGen2, BFVerifier, Test::fast_solve);
TESTA_DEF_VERIFY_WITH_TB(CompareTest1, SmallGen1, SimpleVerifier, Test::fast_solve);
TESTA_DEF_VERIFY_WITH_TB(CompareTest2, SmallGen2, SimpleVerifier, Test::fast_solve);
TESTA_DEF_VERIFY_WITH_TB(CompareTest3, HugeGen1, SimpleVerifier, Test::fast_solve);
TESTA_DEF_VERIFY_WITH_TB(CompareTest4, HugeGen2, SimpleVerifier, Test::fast_solve);
TESTA_DEF_VERIFY_WITH_TB(CompareTest5, HugeGen3, SimpleVerifier, Test::fast_solve);
TESTA_DEF_VERIFY_WITH_TB(Simple_TimeTest1, HugeGen1, SimpleVerifier, Test::simple_solve);
TESTA_DEF_VERIFY_WITH_TB(Improve_TimeTest1, HugeGen1, ImproveVerifier, Test::fast_solve);
TESTA_DEF_VERIFY_WITH_TB(Simple_TimeTest2, HugeGen2, SimpleVerifier, Test::simple_solve);
TESTA_DEF_VERIFY_WITH_TB(Improve_TimeTest2, HugeGen2, ImproveVerifier, Test::fast_solve);
TESTA_DEF_VERIFY_WITH_TB(Simple_TimeTest3, HugeGen3, SimpleVerifier, Test::simple_solve);
TESTA_DEF_VERIFY_WITH_TB(Improve_TimeTest3, HugeGen3, ImproveVerifier, Test::fast_solve);
