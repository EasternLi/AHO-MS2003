add_executable(test_exe test.cpp)

target_link_libraries(test_exe testa)
target_link_libraries(test_exe aho_ms2003)

foreach(test_name IN ITEMS
	empty_graph
	single_node_graph_1
	single_node_graph_2
	mini_graph_1
	mini_graph_2
	mini_graph_3
	mini_graph_4
	mini_graph_5
	no_solution_1
	no_solution_2
	no_solution_3
)
	add_test(NAME ${test_name} COMMAND test_exe ${test_name})
endforeach()

# random compare test
add_executable(randomTest test_compare_with_golden.cpp)
TARGET_LINK_LIBRARIES(randomTest testa brute_force_src testa aho_ms2003 problem_src generator_src)
target_include_directories(randomTest PUBLIC ../src ../test/golden_impl ../3rd)
