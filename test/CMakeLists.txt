add_executable(test_exe test.cpp Graph.cpp generator.cpp CostScaling.cpp SimpleSolver.cpp)

target_link_libraries(test_exe testa)
target_link_libraries(test_exe aho_ms2003)
target_link_libraries(test_exe generator)

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
	SimpleCorrectTest1
	ImproveCorrectTest1
	SimpleCorrectTest2
	ImproveCorrectTest2
	CompareTest1
	CompareTest2
	CompareTest3
	CompareTest4
	CompareTest5
	Simple_TimeTest1
	Improve_TimeTest1
	Simple_TimeTest2
	Improve_TimeTest2
	Simple_TimeTest3
	Improve_TimeTest3
)
	add_test(NAME ${test_name} COMMAND test_exe ${test_name})
endforeach()
