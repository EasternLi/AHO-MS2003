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
