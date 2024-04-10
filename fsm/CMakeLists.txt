cmake_minimum_required(VERSION 3.0.0)
project(FSMexample VERSION 1.0.0)

# Setup testing
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

# Add test cpp file
add_executable(FSMexampleTest fsm_gtest.cc)

# Link test executable against gtest & gtest_main
target_link_libraries(FSMexampleTest ${GTEST_LIBRARY} ${GTEST_MAIN_LIBRARY})

target_link_libraries(
    FSMexampleTest
    gtest_main
    gtest
    gmock
    pthread
)