cmake_minimum_required(VERSION 3.24)
project(paper)

set(CMAKE_CXX_STANDARD 20)

# add_executable(paper src/main.cpp src/Problem.cpp src/LCT.cpp src/Flow.cpp src/UniqueQueue.cpp)

ENABLE_TESTING()

# ADD_SUBDIRECTORY(src)

# ADD_SUBDIRECTORY(test)

ADD_EXECUTABLE(problemTest test/problem_test.cpp src/Problem.cpp src/LCT.cpp src/Flow.cpp src/UniqueQueue.cpp)
