cmake_minimum_required(VERSION 3.11)

project("LFU-cache")

include_directories(include/)

add_executable (lfu_cache src/lfu_cache.cpp)
