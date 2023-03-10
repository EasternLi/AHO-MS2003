#pragma once

#include "ProblemDefine.hpp"
#include "LCT.hpp"
#include "UniqueQueue.hpp"

// 函数命名和符号优先采用 LCT 优化论文中的，其在接下来的注释中称为论文二。

namespace AHO_MS2003 {

class Flow {
public:
	Flow(size_t n_, Data M_, int U_, std::vector<OmegaLimit> limits_);
	
	// 返回值即所求的环流最小费用。
	// 在不存在合法解时，会求得超过 原问题在忽略μω关系下最大解 的解。（归功于 M 的存在）
	// NOTE: 一个实例仅可调用一次。
	Data min_cost();
private:
	// 初始化迭代时使用的成员变量。
	void initialization();
	
	// 将所有边推流至不再 admissible。论文二中该部分在`refine`的开头部分。
	void push_all_admissible_edge();
	
	// 探索更优解。
	void refine();
	
	// 即论文二中的 tree-push/update，为点 p 选择一可用的基础操作。
	// 如论文二 p15 第二段。
	void choice_operator(size_t p);
	
	// 一路向根节点推流，直至点 p 流量守恒或无边可推流。
	// 如论文二 p14 最后一段。
	void send(size_t p);
	
	// 如论文二 p15 第二段 case 2。
	void update_scaling(size_t p);
	
	// 使该边不再 admissible 所需的最少推流。
	// 如论文 (12a)。
	[[nodiscard]] Data q(size_t e_id) const;
	
	// 在该边推`flow_add`单位流。
	void add_flow_of_edge(size_t e_id, Data flow_add);
	
	// 将父节点设为当前边指向的节点。会将 p 的当前边的可发送流量设为 LCT 中的节点值。
	void link(size_t p);
	
	// 断开与父节点的边。会通过 LCT 中的节点值修正 p 的当前边的已发送流量。
	void cut(size_t p);
	
	size_t n;
	Data   M;
	int    epsilon; // 实际上 <_>/2/(n+1) 才是真正的值。一直到算法结束，其均为 2 的幂。这使其避免了非整数运算。
	
	// 下面五个成员变量在迭代时使用，每次均由`initialization`函数初始化。
	std::vector<LCT::Node> nodes; // 各节点对应的 LCT 中的节点。
	std::vector<Data> imbalances; // 即 $b_f$。
	// 各节点的当前边的 e_id。若某节点在 LCT 中非根，该边即为连向父节点的边。
	std::vector<size_t> current_edge;
	// 活跃节点的标号的集合。由于算法逻辑，活跃节点在 LCT 中为根。
	UniqueQueue uq;
	// 在 LCT 中的父节点。-1 表该点为根。
	std::vector<size_t> fa;
	
	std::vector<OmegaLimit> edges; // 边集。
	std::vector<Data> flows;       // 在`edges`中相同下标的边的已流流量。
	std::vector<std::vector<size_t>> G; // 邻接表存图。
	std::vector<int> scaling;           // 即 $p$。同 epsilon，<_>/2/(n+1) 才是真正的值。
};

}
