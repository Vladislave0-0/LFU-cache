#pragma once

#include <gtest/gtest.h>

#include "lfu_cache.hpp"
#include "perfect_cache.hpp"

#include <cstdlib>

int slow_get_page(int key);

TEST(TestPerfectCache, googleTest) {
  size_t cacheSize = 4;
  size_t inputLen = 12;
  std::vector<int> input{1, 2, 3, 4, 1, 2, 5, 1, 2, 4, 3, 4};

  PerfectCache<int> cache(cacheSize, inputLen, input);

  std::vector<bool> result{0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1};

  for (int i = 0; i < inputLen; ++i) {
    EXPECT_EQ(cache.lookup_update(input[i], slow_get_page), result[i]);
  }
}

TEST(TestLFUCache, googleTest) {
  size_t cacheSize = 4;
  size_t inputLen = 12;

  LFUCache<int> cache(cacheSize);

  std::vector<int> input{1, 2, 3, 4, 1, 2, 5, 1, 2, 4, 3, 4};
  std::vector<bool> result{0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1};

  for (int i = 0; i < inputLen; ++i) {
    EXPECT_EQ(cache.lookup_update(input[i], slow_get_page), result[i]);
  }
}