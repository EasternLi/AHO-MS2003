#include "ProblemSolver.hpp"
#include "Flow.hpp"
#include <cassert>
#include <map>
#include <ranges>
#include <utility>

ProblemSolver::ProblemSolver(
		size_t n,
		std::vector<μLimit> μs,
		std::vector<ωLimit> ωs
) : n(n), μs(μs), ωs(ωs) {
	assert(n > 0);
	assert(μs.size() == n + 1);
	for (auto &it: μs | std::views::drop(1)) {
		assert(it.l <= it.u);
	}
	
	for (auto &it: this->ωs) {
		assert(1 <= it.i and it.i <= n);
		assert(1 <= it.j and it.j <= n);
		assert(it.i != it.j);
		
		assert(it.l <= it.u);
		it.pre_processing();
		// 该点论文中未提及，但应为论文第一章假设三的一部分。
		it.l = μs[it.i].l - μs[it.j].u;
		assert(it.l <= it.u);
	}
}

std::optional<Data> ProblemSolver::solve() {
	auto limits = merge_limits();
	auto [valid_cost_max, M, U] = calc(limits);
	
	Flow flow(n, M, U, limits);
	auto min_cost = flow.min_cost();
	
	if (min_cost > valid_cost_max)
		return std::nullopt;
	return {min_cost};
}

std::vector<ωLimit> ProblemSolver::merge_limits() {
	// 应论文 p956 右侧首段中的要求，合并无序对 (i,j) 相同的限制。
	// 论文中该步骤在网络流部分，但该实现提前至这里。
	// 此次 map 可改为 unorder_* 变种，也有办法避免键在值中重复。但不是热代码，不进行优化。
	std::map<std::pair<size_t, size_t>, ωLimit> mp;
	for (auto &it: ωs) {
		// 规定 (i,j) 顺序。
		if (it.i < it.j)
			it.reverse();
		// 去重。
		if (auto p = mp.find({it.i, it.j}); p != mp.end())
			p->second.merge(it);
		else
			mp[{it.i, it.j}] = it;
	}
	
	std::vector<ωLimit> ret;
	ret.reserve(mp.size() + n);
	
	for (auto &[k, v]: mp)
		ret.push_back(v);
	for (size_t i = 1; i <= n; ++i)
		ret.push_back({μs[i], i, 0});
	return ret;
}

std::tuple<Data, Data, int> ProblemSolver::calc(const std::vector<ωLimit> &limits) {
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
