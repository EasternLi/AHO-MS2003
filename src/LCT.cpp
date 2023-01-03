//! 改自 ACM Templates by kuangbin(2018)
//! in 3.5.7

#include "LCT.hpp"

#include <algorithm>
#include <climits>
#include <limits>

LCT::Node *LCT::null = []() {
	auto p = new LCT::Node;
	p->clear({
		std::numeric_limits<Data>::infinity(),
		std::numeric_limits<int>::max()
	});
	return p;
}();

void LCT::add(LCT::Node *x, LCT::Node *y, Data w) {
	x->access();
	for (x = null; y != null; x = y, y = y->fa) {
		y->splay();
		if (y->fa == null) {
			y->ch[1]->update_add(w);
			x->update_add(w);
			y->val.first += w;
			y->push_up();
			return;
		}
		y->setc(x, 1);
		y->push_up();
	}
}

LCT::Node::Val LCT::query_min(LCT::Node *x, LCT::Node *y) {
	x->access();
	for (x = null; y != null; x = y, y = y->fa) {
		y->splay();
		if (y->fa == null)
			return std::min({y->val, y->ch[1]->min, x->min});
		y->setc(x, 1);
		y->push_up();
	}
	std::abort();
}

void LCT::Node::clear(Val _val) {
	fa = ch[0] = ch[1] = null;
	val = min = _val;
	rev = 0;
	add = 0;
}

void LCT::Node::push_up() { min = std::min({val, ch[0]->min, ch[1]->min}); }

void LCT::Node::setc(LCT::Node *p, int d) {
	ch[d] = p;
	p->fa = this;
}

bool LCT::Node::d() { return fa->ch[1] == this; }

bool LCT::Node::is_root() {
	return fa == null || (fa->ch[0] != this && fa->ch[1] != this);
}

void LCT::Node::flip() {
	if (this == null)
		return;
	std::swap(ch[0], ch[1]);
	rev ^= 1;
}

void LCT::Node::update_add(Data w) {
	if (this == null)
		return;
	val.first += w;
	add += w;
	min.first += w;
}

void LCT::Node::push_down() {
	if (rev) {
		ch[0]->flip();
		ch[1]->flip();
		rev = 0;
	}
	if (add) {
		ch[0]->update_add(add);
		ch[1]->update_add(add);
		add = 0;
	}
}

void LCT::Node::go() {
	if (!is_root())
		fa->go();
	push_down();
}

void LCT::Node::rot() {
	Node *f = fa, *ff = fa->fa;
	int c = d(), cc = fa->d();
	f->setc(ch[!c], c);
	this->setc(f, !c);
	if (ff->ch[cc] == f)
		ff->setc(this, cc);
	else
		this->fa = ff;
	f->push_up();
}

LCT::Node *LCT::Node::splay() {
	go();
	while (!is_root()) {
		if (!fa->is_root())
			d() == fa->d() ? fa->rot() : rot();
		rot();
	}
	push_up();
	return this;
}

LCT::Node *LCT::Node::access() {
	for (Node *p = this, *q = null; p != null; q = p, p = p->fa) {
		p->splay()->setc(q, 1);
		p->push_up();
	}
	return splay();
}

LCT::Node *LCT::Node::find_root() {
	Node *x = access();
	while (x->push_down(), x->ch[0] != null)
		x = x->ch[0];
	return x;
}

void LCT::Node::make_root() { access()->flip(); }

void LCT::Node::cut() {
	access();
	ch[0]->fa = null;
	ch[0] = null;
	push_up();
}

void LCT::Node::cut(LCT::Node *x) {
	x->make_root();
	cut();
}

void LCT::Node::link(LCT::Node *x) {
	make_root();
	fa = x;
}
