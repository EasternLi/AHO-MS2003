#pragma once

#include "ProblemDefine.hpp"
#include <cstddef>
#include <optional>
#include <vector>

namespace AHO_MS2003 {

class ProblemSolver {
public:
	// μs_[0] 会被忽略。
	ProblemSolver(
			size_t n_,
			std::vector<μLimit> μs_,
			std::vector<ωLimit> ωs_
	);
	
	// 返回值即所求的最小值。
	// 在无合法解（因l,u的限制）时返回值为空。
	[[nodiscard]] std::optional<Data> solve() const;
private:
	// 根据论文第二章 (6)(7) 中间段，构建网络并返回。
	// 可能会发现部分无解的情况，此时返回值为空。
	[[nodiscard]] std::optional<std::vector<ωLimit>> merge_limits() const;
	
	// 对于边集计算三个值，分别为
	// 1. valid_cost_max: 在忽略μ,ω关系下的最大解。用于检查最终答案是否合法。
	// 2. M: 论文第一章假设二中引入的值。
	// 3. U: 论文摘要中引入的值。
	// 在`merge_limits`中合并边后计算，这可能降低一定大小。
	static std::tuple<Data, Data, int> calc(const std::vector<ωLimit> &limits);

	size_t n;
	std::vector<μLimit> μs;
	std::vector<ωLimit> ωs;
};

}
