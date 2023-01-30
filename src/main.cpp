#include "ProblemSolver.hpp"
#include <cassert>
#include <cmath>
#include <cstdio>

void test_1();

void test_2();

void test_3();

void test_4();

void test_5();

void test_6();

int main() {
	test_1();
	test_2();
	test_3();
	test_4();
	test_5();
	test_6();
}

void test_1() {
	ProblemSolver p(
		1,
		{
			{0, 0, [](int) { return 0; }},
			{2, 3, [](int) { return 1; }},
		},
		{}
	);
	assert(*p.solve() == 1);
	puts("pass test 1");
}

void test_2() {
	ProblemSolver p(
		2,
		{
			{0,  0, [](int) { return 0; }},
			{-2, 2, [](int x) { return -x; }},
			{-2, 2, [](int x) { return +x; }},
		},
		{
			{{-4, 4, [](int x) { return (x - 3) * (x - 3); }}, 1, 2}
		}
	);
	Data ans = INFINITY;
	for (int i = -2; i <= 2; i++)
		for (int j = -2; j <= 2; j++)
		for (int k = -4; k <= 4; k++)
		if (i - j <= k)
		ans = std::min(
			ans,
			Data(-i + j + (k - 3) * (k - 3))
		);
	assert(*p.solve() == ans);
	puts("pass test 2");
}

void test_3() {
	ProblemSolver p(
		2,
		{
			{0,  0, [](int) { return 0; }},
			{-2, 2, [](int x) { return -x; }},
			{-2, 2, [](int x) { return +x; }},
		},
		{
			{{-4, 4, [](int x) { return (x + 3) * (x + 3); }}, 1, 2}
		}
	);
	Data ans = INFINITY;
	for (int i = -2; i <= 2; i++)
		for (int j = -2; j <= 2; j++)
		for (int k = -4; k <= 4; k++)
		if (i - j <= k)
		ans = std::min(
			ans,
			Data(-i + j + (k + 3) * (k + 3))
		);
	assert(*p.solve() == ans);
	puts("pass test 3");
}

#define loop(ident, l, r) for (int ident = l, _##ident = r; ident <= _##ident; ++ident)

void test_4() {
	ProblemSolver p(
		3,
		{
			{ 0, 0, [](int) { return 0; }},
			{ 0, 0, [](int x) { return 5 * x * x + -9 * x + -6 ; }},
			{ 0, 1, [](int x) { return 8 * x * x +  8 * x + -6 ; }},
			{-2, 1, [](int x) { return 4 * x * x +  6 * x + 10 ; }},
		},
		{
			{{-2, 3, [](int x) { return 1 * x * x + -4 * x +  9; }}, 2, 1},
			{{-1, 3, [](int x) { return 2 * x * x +  7 * x +  2; }}, 3, 2},
			{{ 1, 2, [](int x) { return 2 * x * x +  8 * x + -2; }}, 3, 1},
		}
	);
	Data ans = INFINITY;
	loop(n1, 0, 0)loop(n2, 0, 1)loop(n3,-2, 1)
	loop(e0,-2, 3)loop(e1,-1, 3)loop(e2, 1, 2) {
		if(e0<n2-n1)continue;
		if(e1<n3-n2)continue;
		if(e2<n3-n1)continue;
		ans = std::min(
			ans,
			0.0
			+ 5 * n1 * n1 + -9 * n1 + -6
			+ 8 * n2 * n2 +  8 * n2 + -6
			+ 4 * n3 * n3 +  6 * n3 + 10
			+ 1 * e0 * e0 + -4 * e0 +  9
			+ 2 * e1 * e1 +  7 * e1 +  2
			+ 2 * e2 * e2 +  8 * e2 + -2
		);
	}
	assert(ans == *p.solve());
	printf("ans = %lf, pass test 4\n", ans);
}

void test_5() {
	ProblemSolver p(
		4,
		{
			{ 0, 0, [](int) { return 0; }},
			{-2, 0, [](int x) { return 2 * x * x +  9 * x + 4 ; }},
			{ 1, 2, [](int x) { return 5 * x * x + -1 * x + 6 ; }},
			{-1, 0, [](int x) { return 0 * x * x + 10 * x + 5 ; }},
			{ 0, 0, [](int x) { return 0 * x * x + 10 * x + 5 ; }},
		},
		{
			{{ 0, 7, [](int x) { return  4 * x * x + 9 * x + -1 ; }}, 2, 4},
			{{ 2, 9, [](int x) { return  8 * x * x + 5 * x + -7 ; }}, 1, 4},
			{{ 5, 9, [](int x) { return 10 * x * x + 9 * x +  4 ; }}, 3, 4},
			{{-2, 1, [](int x) { return  8 * x * x + 5 * x +  8 ; }}, 1, 2},
		}
	);
	Data ans = INFINITY;
	loop(n1,-2, 0)loop(n2, 1, 2)loop(n3,-1, 0)loop(n4, 0, 0)
	loop(e0, 0, 7)loop(e1, 2, 9)loop(e2, 5, 9)loop(e3,-2, 1) {
		if(e0<n2-n4)continue;
		if(e1<n1-n4)continue;
		if(e2<n3-n4)continue;
		if(e3<n1-n2)continue;
		ans = std::min(
			ans,
			0.0
			+  2 * n1 * n1 +  9 * n1 +  4
			+  5 * n2 * n2 + -1 * n2 +  6
			+  0 * n3 * n3 + 10 * n3 +  5
			+  0 * n4 * n4 + 10 * n4 +  5
			+  4 * e0 * e0 +  9 * e0 + -1
			+  8 * e1 * e1 +  5 * e1 + -7
			+ 10 * e2 * e2 +  9 * e2 +  4
			+  8 * e3 * e3 +  5 * e3 +  8
		);
	}
	assert(ans == *p.solve());
	printf("ans = %lf, pass test 5\n", ans);
}

#undef loop

void test_6() {
	ProblemSolver p(
		3,
		{
			{0, 0, [](int) { return 0; }},
			{0, 0, [](int) { return 0; }},
			{0, 1, [](int) { return 0; }},
			{0, 0, [](int) { return 0; }},
		},
		{
			{{-1, -1, [](int) { return 0; }}, 1, 2},
			{{+0, +0, [](int) { return 0; }}, 2, 3},
		}
	);
	assert(not p.solve().has_value());
	puts("ans donot exist, pass test 6");
}
