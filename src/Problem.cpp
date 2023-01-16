#include "Problem.hpp"
#include "Flow.hpp"
#include <numeric>

Problem::Problem(
		size_t n,
		std::vector<int> P_ls,
		std::vector<int> P_us,
		std::vector<Fn> P_Bs,
		std::vector<std::vector<size_t>> Q_to,
		std::vector<std::vector<int>> Q_ls,
		std::vector<std::vector<int>> Q_us,
		std::vector<std::vector<Fn>> Q_Fs
) : n(n), P_ls(P_ls), P_us(P_us), P_Bs(P_Bs), Q_to(Q_to), Q_ls(Q_ls), Q_us(Q_us), Q_Fs(Q_Fs) {
	assert(n > 0);
	assert(P_ls.size() == n + 1);
	assert(P_us.size() == n + 1);
	assert(P_Bs.size() == n + 1);
	for (size_t i = 1; i <= n; ++i) {
		assert(P_ls[i] <= P_us[i]);
		handle_fn<false>(P_ls[i], P_us[i], P_Bs[i]);
	}
	assert(Q_to.size() == n + 1);
	assert(Q_ls.size() == n + 1);
	assert(Q_us.size() == n + 1);
	assert(Q_Fs.size() == n + 1);
	
	for (size_t i = 1; i <= n; ++i) {
		assert(Q_ls[i].size() == Q_to[i].size());
		assert(Q_us[i].size() == Q_to[i].size());
		assert(Q_Fs[i].size() == Q_to[i].size());
		
		for (size_t j = 0; j < Q_to[i].size(); ++j) {
			assert(Q_ls[i][j] <= Q_us[i][j]);
			handle_fn<true>(Q_ls[i][j], Q_us[i][j], Q_Fs[i][j]);
			Q_ls[i][j] = P_ls[i] - P_us[Q_to[i][j]];
			assert(Q_ls[i][j] <= Q_us[i][j]);
		}
	}
}

/*
May not Correct.
example :
B_1(u_1)=-u_1,u_1\in[0,0]
B_2(u_2)=u_2,u_2\in[0,0]
F_{12}(w_{12})=2x^2+3x,w_{ij}\in[-2,2]
L_f=F_{12}(-2)+B_1(0)+B_2(0)=14
L_d=F_{12}(1)+B_1(0)+B_2(0)=-1
expected M>15 , found function result M=4
*/

template<bool is_F>
void Problem::handle_fn(int l, int u, Fn &fn) {
	auto fn_max = fn(u);//May fn_max=max(fn(l),fn(u))
	if (not is_F)
		fn_max = std::max(fn(l), fn_max);
	
	while (l != u) {
		auto mid = std::midpoint(l, u);
		if (fn(mid) <= fn(mid + 1)) u = mid;
		else l = mid + 1;
	}
	//printf("fn_max = %lf , l = %d , fn(l) = %lf\n",fn_max,l,fn(l));
	M += fn_max - fn(l);
	if (is_F)
		fn = [l, fn](int x) { return fn(std::max(l, x)); };
}

Data Problem::GetM(){
	return M;
};

Data Problem::solve() {
	Flow flow(n, M, U, merge_edge());
	return flow.min_cost();
}

void Problem::bound_fn(int l, int u, Fn &fn) {
	fn = [this, l, u, fn](int x) {
		if (x > u) return fn(u) + M * (x - u);
		if (x < l) return fn(l) - M * (x - l);
		return fn(x);
	};
}

std::map<std::pair<size_t, size_t>, std::forward_list<Problem::Edge>> Problem::merge_edge() {
	std::map<std::pair<size_t, size_t>, std::forward_list<Problem::Edge>> mp;
	for (size_t i = 1; i <= n; ++i)
		mp[{i, 0}].push_front(Edge{P_ls[i], P_us[i], P_Bs[i]});
	P_ls.clear();
	P_us.clear();
	P_Bs.clear();
	
	for (size_t i = 1; i <= n; ++i) {
		for (size_t j = 0; j < Q_to[i].size(); ++j) {
			auto to = Q_to[i][j];
			if (i < to)
				mp[{i, to}].push_front(
						Edge{+Q_ls[i][j], +Q_us[i][j], Q_Fs[i][j]});
			else
				mp[{to, i}].push_front(
						Edge{-Q_us[i][j], -Q_ls[i][j], [fn(Q_Fs[i][j])](int x) {
							return fn(-x);
						}});
		}
	}
	Q_to.clear();
	Q_ls.clear();
	Q_us.clear();
	Q_Fs.clear();
	
	for (auto &[_, es]: mp) {
		if (std::next(es.begin()) != es.end()) {
			// assert(es.size()==2);
			auto e = std::move(es.front());
			es.pop_front();
			es.front().l = std::max(es.front().l, e.l);
			es.front().u = std::min(es.front().u, e.u);
			assert(es.front().l <= es.front().u);
			es.front().fn = [fn1(e.fn), fn2(es.front().fn)](int x) {
				return fn1(x) + fn2(x);
			};
		}
		auto &e = es.front();
		bound_fn(e.l, e.u, e.fn);
		U = std::max(U, e.u - e.l);
	}
	return mp;
}
