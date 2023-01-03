cmake_minimum_required(VERSION 3.24)
project(paper)

set(CMAKE_CXX_STANDARD 20)

add_executable(paper main.cpp Problem.cpp LCT.cpp Flow.cpp UniqueQueue.cpp)
