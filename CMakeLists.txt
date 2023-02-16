cmake_minimum_required(VERSION 3.20)
project(paper)

set(CMAKE_CXX_STANDARD 20)

enable_testing()

add_subdirectory(3rd)
add_subdirectory(src)
add_subdirectory(test)
