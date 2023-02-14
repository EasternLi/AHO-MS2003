#pragma once

#include <cstddef>
#include <functional>

// 该文件定义了问题中的值的类型。
// 实数类型即 Data 的值。用于 F, B 的返回值。
// 整数类型直接使用了 int。用于 l, u。
// 标号类型直接使用了 size_t。用于 i, j。
// 以及用于描述 μ, ω 各自限制的两个结构体。

using Data = double;

using Fn = std::function<Data(int)>;

namespace AHO_MS2003 {

struct μLimit {
	int l, u;
	Fn fn; // NOTE: 需为凸函数。
};

class ProblemSolver;

class Flow;

class Test;

struct ωLimit : μLimit {
	friend ProblemSolver;
	friend Flow;
	friend Test;
	
	size_t i, j;
private:
	// 修改 `fn`，以满足论文第一章假设三。
	void pre_processing();
	
	// min(fn(l..=u))
	[[nodiscard]] Data min() const;
	
	// 用于反转 i,j。
	void reverse();
	
	// 用于合并两个限制，其中 i,j 分别相同。
	// 返回值为真表示有错误发生，即两边共存时无解。
	bool merge(const ωLimit &other);
};

}
