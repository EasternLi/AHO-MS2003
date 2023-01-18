#include "ProblemSolver.hpp"
#include <cassert>
#include <cmath>
#include <cstdio>

void test_1();

void test_2();

void test_3();

void test_4();

void test_5();

int main() {
	test_1();
	test_2();
	test_3();
	test_4();
	test_5();
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
	assert(p.solve() == 1);
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
	assert(p.solve() == ans);
	printf("ans = %lf,pass test 2\n",ans);
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
	assert(p.solve() == ans);
	printf("ans = %lf,pass test 3\n",ans);
}
void test_4() {
	ProblemSolver p(
		2,
		{
			{0,  0, [](int) { return 0; }},
			{-4, 4, [](int x) { return x*x; }},
			{-5, 3, [](int x) { return 3*x*x+4*x; }},
		},
		{
			{{-3, 3, [](int x) { return 2*x*x-3*x; }}, 1, 2}
		}
	);
	Data ans = INFINITY;
	for (int i = -4; i <= 4; i++)
		for (int j = -5; j <= 3; j++)
		for (int k = -3; k <= 3; k++)
		if (i - j <= k)
		ans = std::min(
			ans,
			Data(i*i + 3*j*j+4*j + 2*k*k-3*k)
		);
	assert(p.solve() == ans);
	printf("ans = %lf,pass test 4\n",ans);
}

/*
n = 4, m = 4
node 1 : l = -2 , u = -2 , funtion =  ( 6 ) * x * x + ( -4 ) * x + ( 8 )
node 2 : l = -1 , u = 2 , funtion =  ( 6 ) * x * x + ( -2 ) * x + ( -7 )
node 3 : l = -1 , u = 2 , funtion =  ( 2 ) * x * x + ( 6 ) * x + ( 8 )
node 4 : l = -2 , u = 2 , funtion =  ( 8 ) * x * x + ( -1 ) * x + ( 1 )
edge 1 : from = 1, to = 2 , l = 6 , u = 10 , function =  ( 10 ) * x * x + ( -6 ) * x + ( -3 )
edge 2 : from = 3, to = 4 , l = 7 , u = 10 , function =  ( 2 ) * x * x + ( -1 ) * x + ( -10 )
edge 3 : from = 1, to = 4 , l = 2 , u = 2 , function =  ( 6 ) * x * x + ( 7 ) * x + ( 6 )
edge 4 : from = 1, to = 4 , l = -1 , u = 1 , function =  ( 5 ) * x * x + ( 2 ) * x + ( 6 )

answer:

-2  40.000000
0  -7.000000
-1  4.000000
0  1.000000
6  321.000000 6 >= -2 - 0 = -2
7  81.000000 7 >= -1 - 0 = -1
2  44.000000 2 >= -2 - 0 = -2
-1  9.000000 -1 >= -2 - 0 = -2
*/
void test_5() {
	ProblemSolver p(
		4,
		{
			{0,  0, [](int) { return 0; }},
			{-2, 2, [](int x) { return 6*x*x-4*x+8; }},
			{-1, 2, [](int x) { return 6*x*x-2*x-7; }},
			{-1, 2, [](int x) { return 2*x*x+6*x+8; }},
			{-2, 2, [](int x) { return 8*x*x-x+1; }},
		},
		{
			{{6, 10, [](int x) { return 10*x*x-6*x-3; }}, 1, 2},
			{{7, 10, [](int x) { return 2*x*x-x-10; }}, 3, 4},
			{{2, 2, [](int x) { return 6*x*x+7*x+6; }}, 1, 4},
			{{-1, 1, [](int x) { return 5*x*x+2*x+6; }}, 1, 4},
		}
	);
	Data ans = 493;
	Data res=p.solve();
	assert(res == ans);
	printf("ans = %lf,pass test 5\n",ans);
}