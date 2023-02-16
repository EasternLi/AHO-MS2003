#include "ProblemSolver.hpp"
#include "Flow.hpp"
#include <map>
#include <ranges>
#include <utility>

AHO_MS2003::ProblemSolver::ProblemSolver(
		size_t n_,
		std::vector<AHO_MS2003::MuLimit> Mus_,
		std::vector<AHO_MS2003::OmegaLimit> Omegas_
) : n(n_), Mus(std::move(Mus_)), Omegas(std::move(Omegas_)) {
	// 不再使整个程序崩溃，而是将数据置非法。
#define assert(condition)      \
	do {                       \
		if (not (condition)) { \
			n = -1;            \
			Mus.clear();       \
			Omegas.clear();    \
			return;            \
		}                      \
	} while (false)
	
	assert(n >= 0);
	assert(Mus.size() == n + 1);
	for (auto &it: Mus | std::views::drop(1)) {
		assert(it.l <= it.u);
	}
	
	for (auto &it: Omegas) {
		assert(1 <= it.i and it.i <= n);
		assert(1 <= it.j and it.j <= n);
		assert(it.i != it.j);
		
		assert(it.l <= it.u);
		it.pre_processing();
		// 该点论文中未提及，但应为论文第一章假设三的一部分。
		it.l = Mus[it.i].l - Mus[it.j].u;
		assert(it.l <= it.u);
	}
#undef assert
}

std::optional<Data> AHO_MS2003::ProblemSolver::solve() const {
	if (n == size_t(-1))
		return std::nullopt;
	
	auto _limits = merge_limits();
	if (not _limits.has_value())
		return std::nullopt;
	
	auto& limits = *_limits;
	auto [valid_cost_max, M, U] = calc(limits);
	
	AHO_MS2003::Flow flow(n, M, U, limits);
	auto min_cost = flow.min_cost();
	
	if (min_cost > valid_cost_max)
		return std::nullopt;
	return {min_cost};
}

std::optional<std::vector<AHO_MS2003::OmegaLimit>> AHO_MS2003::ProblemSolver::merge_limits() const {
	// 应论文 p956 右侧首段中的要求，合并无序对 (i,j) 相同的限制。
	// 论文中该步骤在网络流部分，但该实现提前至这里。
	// 此次 map 可改为 unorder_* 变种，也有办法避免键在值中重复。但不是热代码，不进行优化。
	std::map<std::pair<size_t, size_t>, AHO_MS2003::OmegaLimit> mp;
	for (auto it: Omegas) {
		// 规定 (i,j) 顺序。
		if (it.i < it.j)
			it.reverse();
		// 去重。
		if (auto p = mp.find({it.i, it.j}); p != mp.end()) {
			bool err = p->second.merge(it);
			if (err)
				return std::nullopt;
		} else
			mp[{it.i, it.j}] = it;
	}
	
	std::vector<AHO_MS2003::OmegaLimit> ret;
	ret.reserve(mp.size() + n);
	
	for (auto &[k, v]: mp)
		ret.push_back(v);
	for (size_t i = 1; i <= n; ++i)
		ret.push_back({Mus[i], i, 0});
	return {ret};
}

std::tuple<Data, Data, int> AHO_MS2003::ProblemSolver::calc(const std::vector<AHO_MS2003::OmegaLimit> &limits) {
	Data valid_cost_max = 0;
	Data M              = 1;
	int  U              = 0;
	for(auto &it: limits) {
		valid_cost_max += std::max(it.fn(it.l), it.fn(it.u));
		M              += std::max(it.fn(it.l), it.fn(it.u)) - it.min();
		U               = std::max(U, it.u - it.l);
	}
	return { valid_cost_max, M, U };
}
