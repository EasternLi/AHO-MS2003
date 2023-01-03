#pragma once

#include <cstddef>
#include <cassert>
#include <forward_list>
#include <functional>
#include <iterator>
#include <map>
#include <utility>
#include <vector>

using Data = double;
using Fn = std::function<Data(int)>;

class Problem {
public:
	Problem(
			size_t n,
			std::vector<int> P_ls,
			std::vector<int> P_us,
			std::vector<Fn> P_Bs,
			std::vector<std::vector<size_t>> Q_to,
			std::vector<std::vector<int>> Q_ls,
			std::vector<std::vector<int>> Q_us,
			std::vector<std::vector<Fn>> Q_Fs
	);
	
	Data solve();
	
	class Edge {
	public:
		int l, u;
		Fn fn;
	};

private:
	template<bool is_F>
	void handle_fn(int l, int u, Fn &fn);
	
	void bound_fn(int l, int u, Fn &fn);
	
	std::map<std::pair<size_t, size_t>, std::forward_list<Edge>> merge_edge();
	
	size_t n;
	std::vector<int> P_ls, P_us;
	std::vector<Fn> P_Bs;
	
	std::vector<std::vector<size_t>> Q_to;
	std::vector<std::vector<int>> Q_ls, Q_us;
	std::vector<std::vector<Fn>> Q_Fs;
	
	Data M = 1;
	int U = 0;
};
