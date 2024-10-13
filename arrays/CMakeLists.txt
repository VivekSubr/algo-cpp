cmake_minimum_required(VERSION 3.0.0)
project(arrayExample VERSION 1.0.0)

# Setup testing
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

# Add test cpp file
add_executable(arrayTest array_test.cc)

# Link test executable against gtest & gtest_main
target_link_libraries(arrayTest ${GTEST_LIBRARY} ${GTEST_MAIN_LIBRARY})
target_compile_options(arrayTest PRIVATE -Wall -Wextra -Wpedantic -O0 -g3 -ggdb)

add_test(
    NAME arrayTest
    COMMAND arrayTest
)

target_link_libraries(
    arrayTest
    gtest_main
    gtest
    gmock
    pthread
)