add_library(aho_ms2003 ProblemDefine.cpp ProblemSolver.cpp Flow.cpp UniqueQueue.cpp)

target_link_libraries(aho_ms2003 lct)

target_include_directories(aho_ms2003 PUBLIC ./include)
