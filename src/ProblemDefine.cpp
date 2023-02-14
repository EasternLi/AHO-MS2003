#include "ProblemDefine.hpp"
#include <algorithm>
#include <cassert>
#include <ranges>

void AHO_MS2003::ωLimit::pre_processing() {
	// 在参数取该值时，函数值达到最小值。
	auto min_point = *std::ranges::partition_point(
		std::views::iota(l, u),
		[this](int x) { return fn(x) > fn(x + 1); }
	);
	fn = [min_point, fn(fn)](int x) {
		return fn(std::max(min_point, x));
	};
}

Data AHO_MS2003::ωLimit::min() const {
	// 在参数取该值时，函数值达到最小值。
	auto min_point = *std::ranges::partition_point(
		std::views::iota(l, u),
		[this](int x) { return fn(x) > fn(x + 1); }
	);
	return fn(min_point);
}

void AHO_MS2003::ωLimit::reverse() {
	std::tie(l, u) = std::tuple(-u, -l);
	fn             = [fn(fn)](int x) { return fn(-x); };
	std::swap(i, j);
}

bool AHO_MS2003::ωLimit::merge(const AHO_MS2003::ωLimit &other) {
	// [l, u] 取交集，fn 求和。
	l  = std::max(l, other.l);
	u  = std::min(u, other.u);
	fn = [fn1(fn), fn2(other.fn)](int x) {
		return fn1(x) + fn2(x);
	};
	assert(i == other.i);
	assert(j == other.j);
	return not(l <= u);
}
