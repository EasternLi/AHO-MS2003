#pragma once

#include "ProblemDefine.hpp"
#include <cstddef>
#include <vector>

class ProblemSolver {
public:
	// μs[0] 会被忽略。
	ProblemSolver(
			size_t n,
			std::vector<μLimit> μs,
			std::vector<ωLimit> ωs
	);
	
	// 返回值即所求的最小值。
	// NOTE: 一个实例仅可调用一次。
	Data solve();
private:
	// 根据论文第二章 (6)(7) 中间段，构建网络并返回。
	std::vector<ωLimit> merge_limits();
	
	size_t n;
	std::vector<μLimit> μs;
	std::vector<ωLimit> ωs;
	
	// 论文第一章假设二中引入的值。
	// 在`merge_limits`中合并边后计算，这可能降低一定大小。
	Data M;
	// 论文摘要中引入的值。
	// 在`merge_limits`中合并边后计算，这可能降低一定大小。
	int U;
};
