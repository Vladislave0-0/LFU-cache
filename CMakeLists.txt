cmake_minimum_required(VERSION 3.11)

project("LFU-cache")

include_directories(include/)

add_executable (perfect_cache src/perfect_cache.cpp)
add_executable (lfu_cache src/lfu_cache.cpp)

find_package(GTest REQUIRED)
enable_testing()
add_executable(google_test src/google_test.cpp)

target_link_libraries(
    google_test
  GTest::gtest_main
)
