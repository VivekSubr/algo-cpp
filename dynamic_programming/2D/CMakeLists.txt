cmake_minimum_required(VERSION 3.0.0)
project(dpExamples VERSION 1.0.0)

# Setup testing
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

# Add test cpp file
add_executable(2dTest 2d_test.cc)

# Link test executable against gtest & gtest_main
target_link_libraries(2dTest ${GTEST_LIBRARY} ${GTEST_MAIN_LIBRARY})
target_compile_options(2dTest PRIVATE -Wall -Wextra -Wpedantic -O0 -g3 -ggdb)

add_test(
    NAME 2dTest
    COMMAND 2dTest
)

target_link_libraries(
    2dTest
    gtest_main
    gtest
    gmock
    pthread
)