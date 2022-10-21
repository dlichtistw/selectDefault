cmake_minimum_required(VERSION 3.0.0)
project(selectTools VERSION 1.0)

include(doxygen.cmake)

set(CMAKE_CXX_STANDARD 20)

include(CTest)
enable_testing()

include_directories(src)

add_subdirectory(test)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
