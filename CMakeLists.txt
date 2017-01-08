project(argparse)
cmake_minimum_required( VERSION 3.3 )

set(CMAKE_CXX_STANDARD          14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(main src/argparse.h src/argparse.cpp main.cpp)
add_executable(example src/example.cpp)
add_executable(lambda_example src/lambda_example.cpp)
add_executable(specification_example src/specification_example.cpp)
add_executable(tuple_cin src/tuple_cin_example.cpp)

add_executable(runUnitTest src/argparse.h src/argparse.cpp src/argparse_test.cpp)
target_link_libraries(runUnitTest ${GTEST_MAIN_LIBRARIES} ${GTEST_LIBRARIES})

enable_testing()

add_test(NAME unit COMMAND runUnitTest)
