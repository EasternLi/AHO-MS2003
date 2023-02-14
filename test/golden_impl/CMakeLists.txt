ADD_LIBRARY(graph_src graph.cpp)
ADD_LIBRARY(brute_force_src brute_force.cpp)
ADD_LIBRARY(cost_scaling_src cost_scaling.cpp graph.cpp)
add_library(problem_src problem.cpp cost_scaling.cpp graph.cpp)
add_library(generator_src generator.cpp graph.cpp)

TARGET_INCLUDE_DIRECTORIES(graph_src PUBLIC .)
TARGET_INCLUDE_DIRECTORIES(brute_force_src PUBLIC .)
TARGET_INCLUDE_DIRECTORIES(cost_scaling_src PUBLIC .)
target_include_directories(problem_src PUBLIC .)
target_include_directories(generator_src PUBLIC ../../3rd/generator . )


