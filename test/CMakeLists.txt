add_library(test_util test_util.cpp)

add_executable(select_test select_test.cpp)
target_link_libraries(select_test test_util)
add_test(select_test select_test test_util.cpp)

add_executable(select_default_test select_default_test.cpp)
target_link_libraries(select_default_test test_util)
add_test(select_default_test select_default_test)

add_executable(research research.cpp)
target_link_libraries(research test_util)
