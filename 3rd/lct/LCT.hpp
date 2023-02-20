//! 改自 ACM Templates by kuangbin(2018)
//! in 3.5.7

#pragma once

#include <cstddef>
#include <utility>

using Data = double;

namespace LCT {
	struct Node {
		using Val = std::pair<Data, Node*>;
		
		Node *find_root();
		void  set_fa(Node *_fa);
		void  cut();
		
		const Data &get_val();
		Val         get_min_wtrbnr();       // wtrbnr: way to root but no root. ub when call on a root
		void        set_val(Data _val);
		void        add_val_way_to_root(Data d);
		
		Node() = default;
		Node(const Node&) = delete;
		Node(Node&&) = delete;
		Node& operator=(const Node&) = delete;
		Node& operator=(Node&&) = delete;
	private:
		void push_up();
		
		void setc(Node *p, int d);
		
		bool d();
		
		bool is_root();
		
		void update_add(Data w);
		
		void push_down();
		
		void push_down_from_root();
		
		void rot();
		
		Node *splay();
		
		Node *access();
		
		Node *fa, *ch[2];
		Val  min;
		Data val, add;
	};
}
