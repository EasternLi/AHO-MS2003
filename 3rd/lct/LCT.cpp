//! 改自 ACM Templates by kuangbin(2018)
//! in 3.5.7

#include "LCT.hpp"
#include <algorithm>
#include <limits>
#include <cassert>

static LCT::Node *null = []() {
	auto p = new LCT::Node;
	p->init(std::numeric_limits<Data>::infinity());
	return p;
}();

void LCT::Node::init(Data _val) {
	fa = ch[0] = ch[1] = null;
	val = min = {_val, this};
	add = 0;
}

LCT::Node *LCT::Node::find_root() {
	Node *x = access();
	while (x->ch[0] != null)
		x = x->ch[0];
	return x;
}

void LCT::Node::set_fa(Node *_fa) {
	splay();
	assert(fa == null);
	fa = _fa;
}

void LCT::Node::cut() {
	access();
	assert(ch[0]);
	ch[0]->fa = null;
	ch[0] = null;
	push_up();
}

const LCT::Node::Val &LCT::Node::get_val() {
	if(not is_root())
		access();
	return val;
}

const LCT::Node::Val &LCT::Node::get_min_way_to_root() {
	access();
	return min;
}

void LCT::Node::set_val(Data _val) {
	if(not is_root())
		access();
	val.first = _val;
	push_up();
}

void LCT::Node::add_val_way_to_root(Data d) {
	access();
	update_add(d);
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

void LCT::Node::update_add(Data w) {
	if (this == null)
		return;
	val.first += w;
	add += w;
	min.first += w;
}

void LCT::Node::push_down() {
	if (add) {
		ch[0]->update_add(add);
		ch[1]->update_add(add);
		add = 0;
	}
}

void LCT::Node::push_down_from_root() {
	if (!is_root())
		fa->push_down_from_root();
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
	push_down_from_root();
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
