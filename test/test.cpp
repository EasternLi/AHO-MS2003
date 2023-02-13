#include <testa.hpp>
#include <ProblemSolver.hpp>
#include <sstream>

struct G {
	size_t n;
	std::vector<μLimit> μs;
	std::vector<ωLimit> ωs;
	
	void prettyPrint(std::string& out) const {
		std::stringstream ss;
		ss << "{"; {
			ss << "n: " << n <<"; ";
			ss << "{"; for (auto& limit : μs) {
				ss << "{";
				ss << "l:" << limit.l << ",";
				ss << "u:" << limit.u << ",";
				ss << "F:#";
				ss << "},";
			} ss << "}; ";
			ss << "m: " << ωs.size() <<"; ";
			ss << "{"; for (auto& limit : ωs) {
				ss << "{";
				ss << "i:" << limit.i << ",";
				ss << "j:" << limit.j << ",";
				ss << "l:" << limit.l << ",";
				ss << "u:" << limit.u << ",";
				ss << "B:#";
				ss << "},";
			} ss << "}; ";
		} ss << "}";
		out += ss.str();
	}
};

class Test {
public:
	// Complexity: O(U^n * m)
	static std::optional<Data> bf_solve(const G& g) {
		auto& [n, μs, _ωs] = g;
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
	static std::optional<Data> fast_solve(const G& g) {
		auto& [n, μs, ωs] = g;
		ProblemSolver ps(n, μs, ωs);
		return ps.solve();
	}
};

TESTA_DEF_EQ_1(
	empty_graph,
	Test::bf_solve,
	Test::fast_solve,
	(G{
		0,
		{{}},
		{}
	})
);

TESTA_DEF_EQ_1(
	single_node_graph_1,
	Test::bf_solve,
	Test::fast_solve,
	(G{
		1,
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
	(G{
		1,
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
	(G{
		2,
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
	(G{
		2,
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
	(G{
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
	(G{
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
	no_solution_1,
	Test::bf_solve,
	Test::fast_solve,
	(G{
		2,
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
	(G{
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
	(G{
		3,
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
