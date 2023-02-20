//! 改自 ACM Templates by kuangbin(2018)
//! in 3.5.7

#include "LCT.hpp"
#include <cassert>

LCT::Node *LCT::Node::find_root() {
	Node *x = access();
	while (x->ch[0])
		x = x->ch[0];
	return x;
}

void LCT::Node::set_fa(Node *_fa) {
	splay();
	assert(!fa);
	fa = _fa;
}

void LCT::Node::cut() {
	access();
	assert(ch[0]);
	ch[0]->fa = nullptr;
	ch[0] = nullptr;
	push_up();
}

const Data &LCT::Node::get_val() {
	if(not is_root())
		access();
	return val;
}

LCT::Node::Val LCT::Node::get_min_wtrbnr() {
	access();
	Val ret = {val, this};
	for (Node *p = this;; p = p->ch[0]) {
		p->push_down();
		if ( p->ch[1] && p->ch[1]->min.first <= ret.first) ret = p->ch[1]->min;
		if (!p->ch[0])break;
		if (             p->val              <= ret.first) ret = {p->val, p};
	}
	return ret;
}

void LCT::Node::set_val(Data _val) {
	if(not is_root())
		access();
	val = _val;
	push_up();
}

void LCT::Node::add_val_way_to_root(Data d) {
	access();
	update_add(d);
}

void LCT::Node::push_up() {
	min = {val, this};
	if (ch[0] && ch[0]->min.first <= min.first) min = ch[0]->min;
	if (ch[1] && ch[1]->min.first <  min.first) min = ch[1]->min;
}

void LCT::Node::setc(LCT::Node *p, int d) {
	ch[d] = p;
	if (p) p->fa = this;
}

bool LCT::Node::d() { return fa->ch[1] == this; }

bool LCT::Node::is_root() {
	return !fa || (fa->ch[0] != this && fa->ch[1] != this);
}

void LCT::Node::update_add(Data w) {
	val += w;
	add += w;
	min.first += w;
}

void LCT::Node::push_down() {
	if (add != 0) {
		if (ch[0]) ch[0]->update_add(add);
		if (ch[1]) ch[1]->update_add(add);
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
	int c = d(), cc = (ff&&fa->d());
	f->setc(ch[!c], c);
	this->setc(f, !c);
	if (ff && ff->ch[cc] == f)
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
	for (Node *p = this, *q = nullptr; p; q = p, p = p->fa) {
		p->splay()->setc(q, 1);
		p->push_up();
	}
	return splay();
}
