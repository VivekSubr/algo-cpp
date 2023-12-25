cmake_minimum_required(VERSION 3.0.0)
project(dpExamples VERSION 1.0.0)

# Setup testing
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

# Add test cpp file
add_executable(dpTest dp_test.cc)

# Link test executable against gtest & gtest_main
target_link_libraries(dpTest ${GTEST_LIBRARY} ${GTEST_MAIN_LIBRARY})
target_compile_options(dpTest PRIVATE -Wall -Wextra -Wpedantic -O0 -g3 -ggdb)

add_test(
    NAME dpTest
    COMMAND dpTest
)

target_link_libraries(
    dpTest
    gtest_main
    gtest
    gmock
    pthread
)