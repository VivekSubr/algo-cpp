cmake_minimum_required(VERSION 3.10)
project(topKTest VERSION 1.0.0 LANGUAGES CXX)

enable_testing()
find_package(GTest REQUIRED)

add_executable(gtest gtest.cc)

target_compile_features(gtest PRIVATE cxx_std_17)
target_compile_options(gtest PRIVATE -Wall -Wextra -Wpedantic -O0 -g3 -ggdb)

target_link_libraries(
    gtest
    GTest::gtest_main
    GTest::gtest
    pthread
)

add_test(
    NAME gtest
    COMMAND gtest
)