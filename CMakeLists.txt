cmake_minimum_required(VERSION 3.0.0)
project(selectDefault VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

include(CTest)
enable_testing()

add_library(test_util test_util.cpp)

add_executable(select_test select_test.cpp)
target_link_libraries(select_test test_util)
add_test(select_test select_test test_util.cpp)

add_executable(select_default_test select_default_test.cpp)
target_link_libraries(select_default_test test_util)
add_test(select_default_test select_default_test)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
