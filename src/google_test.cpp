#include <bitset>
#include <gtest/gtest.h>

#include "../include/lfu_cache.hpp"
#include "../include/perfect_cache.hpp"

int slow_get_page(int key) { return key; }

TEST(TestPerfectCache, googleTest) {
  size_t cacheSize = 4;
  size_t inputLen = 12;
  std::vector<int> input{1, 2, 3, 4, 1, 2, 5, 1, 2, 4, 3, 4};

  PerfectCache<int> cache(cacheSize, inputLen, input);

  std::bitset<12> result("111110110000");

  for (int i = 0; i < inputLen; ++i) {
    EXPECT_EQ(cache.lookup_update(input[i], slow_get_page), result[i]);
  }
}

TEST(TestLFUCache, googleTest) {
  size_t cacheSize = 4;
  size_t inputLen = 12;

  LFUCache<int> cache(cacheSize);

  std::vector<int> input{1, 2, 3, 4, 1, 2, 5, 1, 2, 4, 3, 4};
  std::bitset<12> result("101110110000");

  for (int i = 0; i < inputLen; ++i) {
    EXPECT_EQ(cache.lookup_update(input[i], slow_get_page), result[i]);
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
