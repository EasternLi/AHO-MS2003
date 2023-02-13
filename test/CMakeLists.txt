add_executable(test_exe test.cpp)

target_link_libraries(test_exe testa)
target_link_libraries(test_exe aho_ms2003)

add_test(NAME empty_graph COMMAND test_exe empty_graph)
