cmake_minimum_required(VERSION 3.10)
project(treeAlgo VERSION 1.0.0)

# Setup testing
enable_testing()
find_package(GTest REQUIRED)

# Add test cpp file
add_executable(treeAlgo tree_test.cc)

target_compile_options(treeAlgo PRIVATE -Wall -Wextra -Wpedantic -O0 -g3 -ggdb)

add_test(
    NAME treeAlgo
    COMMAND treeAlgo
)

target_link_libraries(
    treeAlgo
    GTest::gtest_main
    GTest::gtest
    pthread
)