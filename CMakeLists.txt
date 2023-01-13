cmake_minimum_required(VERSION 3.24)
project(paper)

set(CMAKE_CXX_STANDARD 20)

add_executable(paper src/main.cpp src/ProblemDefine.cpp src/ProblemSolver.cpp src/LCT.cpp src/Flow.cpp src/UniqueQueue.cpp)
