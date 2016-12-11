project(argparse)
cmake_minimum_required( VERSION 3.3 )

set(CMAKE_CXX_STANDARD          14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(runUnitTest src/argparse.h src/argparse.cpp src/argparse_test.cpp)
target_link_libraries(runUnitTest ${GTEST_MAIN_LIBRARIES} ${GTEST_LIBRARIES})

enable_testing()

add_test(NAME unit COMMAND runUnitTest)
