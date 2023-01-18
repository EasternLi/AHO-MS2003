#include "../src/Problem.hpp"
#include <math.h>
#include <cstdio>
#include <iostream>

using namespace std;

const int inf = 0x3f3f3f3f;
Fn testf = [](int x) {
  Data res = 2 * x * x - 3 * x;
  // printf("x = %d , res = %lf\n", x , res);
  return res;
};
int main() {
  Data mx = -inf, mi = inf;
  for (int i = -2; i <= 2; i++) {
    // printf("i = %d , res = %lf\n", i , testf(i));
    mx = max(testf(i), mx);
    mi = min(testf(i), mi);
  }
  Data ans = mx - mi;
  Problem p(2, {0, 0, 0}, {0, 0, 0},
            {[](int) { return 1; }, [](int x) { return -x; },
             [](int x) { return +x; }},
            {{}, {2}, {}}, {{}, {-2}, {}}, {{}, {+2}, {}},
            {{}, {[](int x) { return 2 * x * x - 3 * x; }}, {}});

  Data res = p.GetM();
  printf("MAX = %lf , Min = %lf . expected > %lf , found = %lf\n", mx, mi, ans, res);
  assert(res > ans);
  return 0;
}