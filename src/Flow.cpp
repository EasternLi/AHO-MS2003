#include "Flow.hpp"
#include <cmath>
#include <limits>

Flow::Flow(size_t n, Data M, int U, std::vector<ωLimit> limits)
		: n(n), M(M), ε(U), U(U), nodes(n + 1), imbalances(n + 1), current_edge(n + 1), uq(n + 1), sons(n + 1),
		  fa(n + 1, -1) {
	edges.reserve(limits.size() * 2);
	flows.resize (limits.size() * 2);
	G.resize(n + 1);
	scaling.resize(n + 1);
	
	for (auto edge: limits) {
		G[edge.i].push_back(edges.size());
		edges.push_back(edge);
		
		edge.reverse();
		
		G[edge.i].push_back(edges.size());
		edges.push_back(edge);
	}
}

Data Flow::min_cost() {
	for (; ε * (n + 1) >= Data(1); ε /= 2) {
		initialization();
		push_all_admissible_edge();
		refine();
	}
	Data answer = 0;
	for (size_t e_id = 0; e_id < edges.size(); ++e_id) {
		auto &e = edges[e_id];
		// 根据论文 (9b) 下一段。
		e.fn = [x(flows[e_id]), fn(e.fn)](int w) { return fn(w) - x * w; };
		answer += e.min();
	}
	
	answer /= 2; // 每条边在`edges`中各出现两次。
	return answer;
}

void Flow::initialization() {
	// 部分变量在迭代前后不变。
	for (size_t i = 0; i <= n; ++i)
		nodes[i].clear({std::numeric_limits<Data>::infinity(), i});
	// std::fill(imbalances.begin(), imbalances.end(), 0);
	std::fill(current_edge.begin(), current_edge.end(), 0);
	// uq.clear();
	for (auto &item: sons)item.clear();
	// std::fill(fa.begin(), fa.end(), -1);
}

void Flow::push_all_admissible_edge() {
	for (size_t e_id = 0; e_id < edges.size(); ++e_id) {
		auto &e = edges[e_id];
		auto _q = q(e_id);
		if (greater_than_zero(_q)) {
			imbalances[e.i] -= _q;
			imbalances[e.j] += _q;
			add_flow_of_edge(e_id, _q);
		}
	}
	for (size_t i = 0; i <= n; ++i)
		if (greater_than_zero(imbalances[i]))
			uq.push(i);
}

void Flow::refine() {
	while (not uq.empty()) {
		choice_operator(uq.front());
		if (not greater_than_zero(imbalances[uq.front()]))
			uq.pop();
	}
	// 如论文二 p15 第三段最后一句话。
	for (size_t i = 0; i <= n; ++i)
		if (fa[i] != size_t(-1))
			cut(i);
}

void Flow::choice_operator(size_t p) {
	for (; current_edge[p] < G[p].size(); ++current_edge[p]) {
		auto e_id = G[p][current_edge[p]];
		if (greater_than_zero(q(e_id)))
			return link(p), send(p);
	}
	update_scaling(p);
}

void Flow::send(size_t p) {
	auto root = nodes[p].find_root();
	// 从 p 点推 x 单位流至根节点。
	auto push = [&](Data x) {
		auto root_id = root - nodes.data();
		LCT::add(&nodes[p], root, -x);
		imbalances[p] -= x;
		imbalances[root_id] += x;
		if (greater_than_zero(imbalances[root_id]))
			uq.push(root_id);
	};
	while (&nodes[p] != root) {
		auto min = LCT::query_min(&nodes[p], root);
		if (imbalances[p] <= min.first) {
			push(imbalances[p]);
			return;
		}
		push(min.first);
		cut(min.second);
		root = &nodes[min.second];
	}
}

void Flow::update_scaling(size_t p) {
	// 如论文二 p15 第二段最后一句话。
	for (auto cut_p: sons[p])
		if (fa[cut_p] == p)
			cut(cut_p);
	sons[p].clear();
	// 在`cut`之后修改。因其需要之前的`q`，其又需要之前的`scaling`。
	scaling[p] += ε / 2;
	current_edge[p] = 0;
}

Data Flow::q(size_t e_id) {
	auto &e = edges[e_id];
	// 论文中该值为  ⌊...⌋，但  ⌈...⌉-1 同样满足要求。
	// 且其在 ... 为 [l,u] 中的整数时避免了对拉伸后价格为零的部分流量的推流。
	int t = std::ceil(scaling[e.i] - scaling[e.j]) - 1;
	// 下面两个条件语句起到了论文第一章假设二的作用。
	if (t + 1 > e.u)return M - flows[e_id];
	if (t < e.l)return 0;
	return std::max(Data(0), e.fn(t+1) - e.fn(t) - flows[e_id]);
}

void Flow::add_flow_of_edge(size_t e_id, Data flow_add) {
	flows[e_id] += flow_add;
	flows[e_id ^ 1] = -flows[e_id];
}

void Flow::link(size_t p) {
	auto e_id = G[p][current_edge[p]];
	auto &e = edges[e_id];
	sons[e.j].push_back(p);
	fa[p] = e.j;
	
	// 论文二中证明了不会出现环，但要求我们及时剪掉值为零的节点与其父节点之间的边。
	// 论文二中的做法是每次未耗尽叶节点的推流后，剪掉路径上最靠近根的节点。
	// 本实现则在加边时保证路径上没有该种节点。
	for (auto root = nodes[e.j].find_root();;) {
		auto min = LCT::query_min(&nodes[e.j], root);
		if (greater_than_zero(min.first))
			break;
		cut(min.second);
		root = &nodes[min.second];
	}
	
	nodes[p].access();
	nodes[p].val.first = q(e_id);
	nodes[p].push_up();
	nodes[p].fa = &nodes[e.j];
}

void Flow::cut(size_t p) {
	fa[p] = -1;
	nodes[p].cut();
	
	auto e_id = G[p][current_edge[p]];
	add_flow_of_edge(e_id, q(e_id) - nodes[p].val.first);
	
	nodes[p].val.first = std::numeric_limits<Data>::infinity();
	nodes[p].push_up();
}
