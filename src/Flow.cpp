#include "Flow.hpp"

static bool greater_than_zero(Data x) {
	return x > 1e-9;
}

Flow::Flow(size_t n, Data M, int U, std::map<std::pair<size_t, size_t>, std::forward_list<Problem::Edge>> mp)
		: n(n), M(M), ε(U), U(U), nodes(n + 1), imbalances(n + 1), current_edge(n + 1), uq(n + 1), sons(n + 1),
		  fa(n + 1, -1) {
	edges.reserve(mp.size() * 2);
	G.resize(n + 1);
	scaling.resize(n + 1);
	for (auto &[ij, _e]: mp) {
		auto [i, j] = ij;
		auto &e = _e.front();
		
		G[i].push_back(edges.size());
		edges.push_back({i, j, +e.l, +e.u, 0, e.fn});
		
		G[j].push_back(edges.size());
		edges.push_back({j, i, -e.u, -e.l, 0, [fn(edges.back().fn)](int x) {
			return fn(-x);
		}});
	}
}

Data Flow::min_cost() {
	for (; ε * (n + 1) >= Data(1); ε /= 2) {
		initialization();
		push_all_admissible_edge();
		while (not uq.empty())
			choice_operator(uq.pop());
		for (size_t i = 0; i <= n; ++i)
			if (fa[i] != size_t(-1))
				cut(i);
	}
	Data answer = 0;
	for (auto [_i, _j, l, u, x, fn]: edges) {
		fn = [x, fn](int w) { return fn(w) - x * w; };
		while (l != u) {
			auto mid = std::midpoint(l, u);
			if (fn(mid) <= fn(mid + 1))u = mid;
			else l = mid + 1;
		}
		answer += fn(l);
	}
	answer /= 2;
	return answer;
}

Data Flow::q(size_t e_id) {
	auto &e = edges[e_id];
	auto t = scaling[e.from] - scaling[e.to];
	if (t >= e.u)return M - e.flow;
	return std::max(Data(0), e.b(std::floor(t)) - e.flow);
}

void Flow::initialization() {
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
			imbalances[e.from] -= _q;
			imbalances[e.to  ] += _q;
			e.flow += _q;
			edges[e_id ^ 1].flow -= _q;
		}
	}
	for (size_t i = 0; i <= n; ++i)
		if (greater_than_zero(imbalances[i]))
			uq.push(i);
}

void Flow::choice_operator(size_t p) {
	for (; current_edge[p] < G[p].size(); ++current_edge[p]) {
		if (greater_than_zero(q(G[p][current_edge[p]])))
			return tree_push(p);
	}
	update_scaling(p);
}

void Flow::tree_push(size_t p) {
	link(p);
	auto root = nodes[p].find_root();
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
	choice_operator(p);
}

void Flow::update_scaling(size_t p) {
	for (auto cut_p: sons[p])
		if (fa[cut_p] == p)
			cut(cut_p);
	sons[p].clear();
	current_edge[p] = 0;
	// after `cut`, where we need old `scaling` to get old `q`
	scaling[p] += ε / 2;
	choice_operator(p);
}

void Flow::link(size_t p) {
	auto e_id = G[p][current_edge[p]];
	auto &e = edges[e_id];
	sons[e.to].push_back(p);
	fa[p] = e.to;
	
	nodes[p].push_down();
	nodes[p].val.first = q(e_id);
	nodes[p].push_up();
	nodes[p].fa = &nodes[e.to];
}

void Flow::cut(size_t p) {
	fa[p] = -1;
	nodes[p].cut();
	nodes[p].push_down();
	
	auto e_id = G[p][current_edge[p]];
	auto &e = edges[e_id];
	e.flow += q(e_id) - nodes[p].val.first;
	edges[e_id ^ 1].flow = -e.flow;
	
	nodes[p].val.first = std::numeric_limits<Data>::infinity();
	nodes[p].push_up();
}

//int Flow::Edge::cost() {
//	auto _l=l, _r=u;
//	while (_l!=_r) {
//		auto mid = std::midpoint(_l, _r);
//		if(b(mid)>flow)_r=mid;
//		else _l=mid+1;
//	}
//	return _l;
//}

//Data Flow::scaled_cost(size_t i) {
//	return edges[i].cost()-scaling[edges[i].from]+scaling[edges[i].to];
//}
