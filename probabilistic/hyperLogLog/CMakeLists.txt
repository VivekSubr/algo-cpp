cmake_minimum_required(VERSION 3.0.0)
project(hyperLogLog VERSION 1.0.0)
include(ExternalProject)

# Setup testing
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

#include xxHash
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/xxHash)

ExternalProject_Add(xxHash
    GIT_REPOSITORY https://github.com/Cyan4973/xxHash
    GIT_TAG dev
    CONFIGURE_COMMAND ""
    BUILD_IN_SOURCE true
    BUILD_COMMAND "make"
    INSTALL_COMMAND make DESTDIR=${CMAKE_BINARY_DIR}/xxHash install
    SOURCE_DIR ""
)

include_directories( ${CMAKE_BINARY_DIR}/xxHash/usr/local/include)
link_directories(${CMAKE_BINARY_DIR}/xxHash/usr/local/lib)

# Add test cpp file
add_executable(estimatorTest hyperLogLog_test.cc)

# Link test executable against gtest & gtest_main
target_link_libraries(estimatorTest ${GTEST_LIBRARY} ${GTEST_MAIN_LIBRARY})
target_compile_options(estimatorTest PRIVATE -Wall -Wextra -Wpedantic -O0 -g3 -ggdb)

add_test(
    NAME estimatorTest
    COMMAND estimatorTest
)

target_link_libraries(
    estimatorTest
    gtest_main
    gtest
    gmock
    xxhash
    pthread
)