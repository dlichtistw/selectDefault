include(CTest)
enable_testing()

add_library(test_util test_util.cpp)

add_executable(select_test select_test.cpp)
target_link_libraries(select_test test_util)
add_test(select_test select_test test_util.cpp)

add_executable(select_or_default_test select_or_default_test.cpp)
target_link_libraries(select_or_default_test test_util)
add_test(select_or_default_test select_or_default_test)

add_executable(select_if_test select_if_test.cpp)
target_link_libraries(select_if_test test_util)
add_test(select_if_test select_if_test)

add_executable(research research.cpp)
target_link_libraries(research test_util)
