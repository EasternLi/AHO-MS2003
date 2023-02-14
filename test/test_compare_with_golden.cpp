//generate random cases
#include "testa.hpp"
#include "prettyprint.hpp"

#include <type_traits>
#include <tuple>
#include <limits>
#include <cstdio>

#include"graph.hpp"
#include"brute_force.hpp"
#include"problem.hpp"
#include"ProblemSolver.hpp"
#include"generator.hpp"

using namespace std;

double SolveByBF(const tuple<Graph>& in){
  Graph g=get<0>(in);
  BruteForce bf;
  bf.ChangeGraph(g);
  std::vector<int> value;
  double answer = bf.Solve(value);
  if (answer == 1e9) {
    answer = nan("");
  }
  return answer;
}

double SolveBySimple(const tuple<Graph>& in){
  Graph g=get<0>(in);
  Problem pro(g);
  double res=pro.Solve();
  return res;
}

double SolveByImprove(const tuple<Graph>& in){
  Graph g=get<0>(in);
  std::vector<μLimit> μs;
	std::vector<ωLimit> ωs;
  for(auto x:g.node_){
    μs.push_back({x.l_,x.r_,x.F_});
  }
  for(auto x:g.edge_){
    ωs.push_back({{x.l_,x.r_,x.F_},(size_t)x.u_,(size_t)x.v_});
  }
  ProblemSolver pro(g.n_,μs,ωs);
  auto back=(pro.solve());
  double res=(back.has_value()?back.value():nan(""));
  return res;
}

void SmallGen1(const string& name,function<void(const tuple<Graph>&)>cs){
  int n=(name.substr(0,7)=="Compare"?10000:100);
  for(int i=1;i<=n;i++){
    cs(TinyGen(5,10,0,{-2,2},{-2,6},{-10,10},i));
  }
}

void SmallGen2(const string& name,function<void(const tuple<Graph>&)>cs){
  int n=(name.substr(0,7)=="Compare"?10000:100);
  for(int i=1;i<=n;i++){
    cs(TinyGen(4,10,1,{-2,2},{-2,6},{-10,10},i));
  }
}

void HugeGen1(const string& name,function<void(const tuple<Graph>&)>cs){
  int n=(name.substr(0,7)=="Compare"?100:1);
  for(int i=1;i<=n;i++){
    cs(TinyGen(50,100,0,{-20,20},{-20,60},{-100,100},i));
  }
}

void HugeGen2(const string& name,function<void(const tuple<Graph>&)>cs){
  int n=(name.substr(0,7)=="Compare"?100:1);
  for(int i=1;i<=n;i++){
    cs(TinyGen(50,200,1,{-20,20},{-20,60},{-100,100},i));
  }
}

void HugeGen3(const string& name,function<void(const tuple<Graph>&)>cs){
  int n=(name.substr(0,7)=="Compare"?10:1);
  for(int i=1;i<=n;i++){
    cs(TinyGen(100,300,1,{-40,40},{-40,40},{-100,100},i));
  }
}

void BFVerifier(const double& res, const tuple<Graph>& in)
{   
    double ans=SolveByBF(get<0>(in));
    TESTA_ASSERT((std::isnan(ans) && std::isnan(res))||res==ans)
        (res)
        (get<0>(in).info)
        .issue();
}

void SimpleVerifier(const double& res, const tuple<Graph>& in)
{   
    double ans=SolveBySimple(get<0>(in));
    TESTA_ASSERT((std::isnan(ans) && std::isnan(res))||res==ans)
        (res)
        (get<0>(in).info)
        .issue();
}

void ImproveVerifier(const double& res, const tuple<Graph>& in)
{   
    double ans=SolveByImprove(get<0>(in));
    TESTA_ASSERT((std::isnan(ans) && std::isnan(res))||res==ans)
        (res)
        (get<0>(in).info)
        .issue();
}


TESTA_DEF_VERIFY_WITH_TB(SimpleCorrectTest1, SmallGen1, BFVerifier, SolveBySimple);
TESTA_DEF_VERIFY_WITH_TB(ImproveCorrectTest1, SmallGen1, BFVerifier, SolveByImprove);
TESTA_DEF_VERIFY_WITH_TB(SimpleCorrectTest2, SmallGen2, BFVerifier, SolveBySimple);
TESTA_DEF_VERIFY_WITH_TB(ImproveCorrectTest2, SmallGen2, BFVerifier, SolveByImprove);
TESTA_DEF_VERIFY_WITH_TB(CompareTest1, SmallGen1, SimpleVerifier, SolveByImprove);
TESTA_DEF_VERIFY_WITH_TB(CompareTest2, SmallGen2, SimpleVerifier, SolveByImprove);
TESTA_DEF_VERIFY_WITH_TB(CompareTest3, HugeGen1, SimpleVerifier, SolveByImprove);
TESTA_DEF_VERIFY_WITH_TB(CompareTest4, HugeGen2, SimpleVerifier, SolveByImprove);
TESTA_DEF_VERIFY_WITH_TB(CompareTest5, HugeGen3, SimpleVerifier, SolveByImprove);
TESTA_DEF_VERIFY_WITH_TB(Simple_TimeTest1, HugeGen1, SimpleVerifier, SolveBySimple);
TESTA_DEF_VERIFY_WITH_TB(Improve_TimeTest1, HugeGen1, ImproveVerifier, SolveByImprove);
TESTA_DEF_VERIFY_WITH_TB(Simple_TimeTest2, HugeGen2, SimpleVerifier, SolveBySimple);
TESTA_DEF_VERIFY_WITH_TB(Improve_TimeTest2, HugeGen2, ImproveVerifier, SolveByImprove);
TESTA_DEF_VERIFY_WITH_TB(Simple_TimeTest3, HugeGen3, SimpleVerifier, SolveBySimple);
TESTA_DEF_VERIFY_WITH_TB(Improve_TimeTest3, HugeGen3, ImproveVerifier, SolveByImprove);

