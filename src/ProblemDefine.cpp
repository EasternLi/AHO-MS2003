#include "ProblemDefine.hpp"
#include <algorithm>
#include <cassert>

// like std::partition_point
template<typename Fn>
static int partition_point(int l, int r, const Fn& pred) {
	while (l != r) {
		int mid = l + (r - l) / 2;
		if (pred(mid))
			l = mid + 1;
		else
			r = mid;
	}
	return l;
}

void AHO_MS2003::OmegaLimit::pre_processing() {
	// 在参数取该值时，函数值达到最小值。
	auto min_point = partition_point(l, u, [this](int x) {
		return fn(x) > fn(x + 1);
	});
	fn = [min_point, fn(fn)](int x) {
		return fn(std::max(min_point, x));
	};
}

Data AHO_MS2003::OmegaLimit::min() const {
	// 在参数取该值时，函数值达到最小值。
	auto min_point = partition_point(l, u, [this](int x) {
		return fn(x) > fn(x + 1);
	});
	return fn(min_point);
}

void AHO_MS2003::OmegaLimit::reverse() {
	std::tie(l, u) = std::tuple(-u, -l);
	fn             = [fn(fn)](int x) { return fn(-x); };
	std::swap(i, j);
}

bool AHO_MS2003::OmegaLimit::merge(const AHO_MS2003::OmegaLimit &other) {
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
