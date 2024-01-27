cmake_minimum_required(VERSION 3.0.0)
project(CombinationsAlgo VERSION 1.0.0)

# Setup testing
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

# Add test cpp file
add_executable(CombinationsTest combination_test.cc)

# Link test executable against gtest & gtest_main
target_link_libraries(CombinationsTest ${GTEST_LIBRARY} ${GTEST_MAIN_LIBRARY})

target_link_libraries(
    CombinationsTest
    gtest_main
    gtest
    gmock
    pthread
)