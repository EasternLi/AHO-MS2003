#include "Flow.hpp"
#include <cmath>
#include <limits>

AHO_MS2003::Flow::Flow(size_t n, Data M, int U, std::vector<ωLimit> limits)
		: n(n), M(M), ε(U), U(U), nodes(n + 1), imbalances(n + 1), current_edge(n + 1), uq(n + 1), fa(n + 1, -1) {
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

Data AHO_MS2003::Flow::min_cost() {
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

void AHO_MS2003::Flow::initialization() {
	// 部分变量在迭代前后不变。
	for (size_t i = 0; i <= n; ++i)
		nodes[i].init(std::numeric_limits<Data>::infinity());
	// std::fill(imbalances.begin(), imbalances.end(), 0);
	std::fill(current_edge.begin(), current_edge.end(), 0);
	// uq.clear();
	// std::fill(fa.begin(), fa.end(), -1);
}

void AHO_MS2003::Flow::push_all_admissible_edge() {
	for (size_t e_id = 0; e_id < edges.size(); ++e_id) {
		auto &e = edges[e_id];
		auto _q = q(e_id);
		if (_q > 0) {
			imbalances[e.i] -= _q;
			imbalances[e.j] += _q;
			add_flow_of_edge(e_id, _q);
		}
	}
	for (size_t i = 0; i <= n; ++i)
		if (imbalances[i] > 0)
			uq.push(i);
}

void AHO_MS2003::Flow::refine() {
	while (not uq.empty()) {
		choice_operator(uq.front());
		if (imbalances[uq.front()] == 0)
			uq.pop();
	}
	// 如论文二 p15 第三段最后一句话。
	for (size_t i = 0; i <= n; ++i)
		if (fa[i] != size_t(-1))
			cut(i);
}

void AHO_MS2003::Flow::choice_operator(size_t p) {
	for (; current_edge[p] < G[p].size(); ++current_edge[p]) {
		auto e_id = G[p][current_edge[p]];
		if (q(e_id) > 0)
			return link(p), send(p);
	}
	update_scaling(p);
}

void AHO_MS2003::Flow::send(size_t p) {
	auto root_id = nodes[p].find_root() - nodes.data();
	// 从 p 点推 x 单位流至根节点。
	auto push = [&](Data x) {
		nodes[p].add_val_way_to_root(-x);
		imbalances[p] -= x;
		imbalances[root_id] += x;
		if (imbalances[root_id] > 0)
			uq.push(root_id);
	};
	while (p != root_id) {
		auto min = nodes[p].get_min_way_to_root();
		if (imbalances[p] < min.first) {
			push(imbalances[p]);
			return;
		}
		push(min.first);
		root_id = min.second - nodes.data();
		cut(root_id);
	}
}

void AHO_MS2003::Flow::update_scaling(size_t p) {
	// 如论文二 p15 第二段最后一句话。
	for (auto e_id : G[p]) {
		auto cut_p = edges[e_id].j;
		if (fa[cut_p] == p)
			cut(cut_p);
	}
	// 在`cut`之后修改。因其需要之前的`q`，其又需要之前的`scaling`。
	scaling[p] += ε / 2;
	current_edge[p] = 0;
}

Data AHO_MS2003::Flow::q(size_t e_id) {
	auto &e = edges[e_id];
	// 论文中该值为  ⌊...⌋，但  ⌈...⌉-1 同样满足要求。
	// 且其在 ... 为 [l,u] 中的整数时避免了对拉伸后价格为零的部分流量的推流。
	int t = std::ceil(scaling[e.i] - scaling[e.j]) - 1;
	// 下面两个条件语句起到了论文第一章假设二的作用。
	if (t + 1 > e.u)return M - flows[e_id];
	if (t < e.l)return 0;
	return std::max(Data(0), e.fn(t+1) - e.fn(t) - flows[e_id]);
}

void AHO_MS2003::Flow::add_flow_of_edge(size_t e_id, Data flow_add) {
	flows[e_id] += flow_add;
	flows[e_id ^ 1] = -flows[e_id];
}

void AHO_MS2003::Flow::link(size_t p) {
	auto e_id = G[p][current_edge[p]];
	auto &e = edges[e_id];
	fa[p] = e.j;
	
	nodes[p].set_val(q(e_id));
	nodes[p].set_fa(&nodes[e.j]);
}

void AHO_MS2003::Flow::cut(size_t p) {
	fa[p] = -1;
	nodes[p].cut();
	
	auto e_id = G[p][current_edge[p]];
	add_flow_of_edge(e_id, q(e_id) - nodes[p].get_val());
	
	nodes[p].set_val(std::numeric_limits<Data>::infinity());
}
