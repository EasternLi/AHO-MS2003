#pragma once

#include "ProblemDefine.hpp"
#include "LCT.hpp"
#include "UniqueQueue.hpp"

// 函数命名和符号优先采用 LCT 优化论文中的，其在接下来的注释中称为论文二。

class Flow {
public:
	Flow(size_t n, Data M, int U, std::vector<ωLimit> limits);
	
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
	
	// 推流这些单位后，该边与其反向边均不再 admissible。
	// 如论文 (12a)。
	Data q(size_t e_id);
	
	// 推无穷小的流时的单位费用。
	Data scaled_cost(size_t e_id);
	
	// 在该边推`flow_add`单位流。
	void add_flow_of_edge(size_t e_id, Data flow_add);
	
	// 将父节点设为当前边指向的节点。会将 p 的当前边的可发送流量设为 LCT 中的节点值。
	void link(size_t p);
	
	// 断开与父节点的边。会通过 LCT 中的节点值修正 p 的当前边的已发送流量。
	void cut(size_t p);
	
	size_t n;
	Data M;
	Data ε;
	int U;
	
	// 下面六个成员变量在迭代时使用，每次均由`initialization`函数初始化。
	std::vector<LCT::Node> nodes; // 各节点对应的 LCT 中的节点。
	std::vector<Data> imbalances; // 即 $b_f$。
	// 各节点的当前边的 e_id。若某节点在 LCT 中非根，该边即为连向父节点的边。
	std::vector<size_t> current_edge;
	// 活跃节点的标号的集合。由于算法逻辑，活跃节点在 LCT 中为根。
	UniqueQueue uq;
	// 在 LCT 中的子节点集合。`tree_push, link`中剪边不会维护，故可能过多。
	std::vector<std::vector<size_t>> sons;
	// 在 LCT 中的父节点。-1 表该点为根。
	std::vector<size_t> fa;
	
	std::vector<ωLimit> edges; // 边集。
	std::vector<Data> flows;   // 在`edges`中相同下标的边的已流流量。
	std::vector<std::vector<size_t>> G; // 邻接表存图。
	std::vector<Data> scaling;          // 即 $p$。
};
