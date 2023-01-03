//! 改自 ACM Templates by kuangbin(2018)
//! in 3.5.7

#pragma once

#include <cstddef>
#include <utility>

using Data = double;

namespace LCT {
	struct Node {
		Node *fa, *ch[2];
		int rev; //旋转标记
		
		using Val = std::pair<Data, size_t>;
		Val min, val;
		Data add;
		
		void clear(Val _val);
		
		void push_up();
		
		void setc(Node *p, int d);
		
		bool d();
		
		bool is_root();
		
		//翻转
		void flip();
		
		void update_add(Data w);
		
		void push_down();
		
		//直接标记下放
		void go();
		
		void rot();
		
		Node *splay();
		
		Node *access();
		
		//找该点的根
		Node *find_root();
		
		//变为树根 (换根操作)
		void make_root();
		
		//切断该点和父亲结点的边
		void cut();
		
		//切断该点以 x 为根时, 该点和父亲结点的根
		//要求这个点和 x 在同一颗树而且不能相同
		// x 变为所在树的树根
		void cut(Node *x);
		
		//该点连接到 x
		//假如是有虚边信息的,需要先 x->access() 再连接
		void link(Node *x);
	};
	
	void add(Node *x, Node *y, Data w);
	
	Node::Val query_min(Node *x, Node *y);
	
	extern Node *null;
}
