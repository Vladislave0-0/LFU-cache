#include "../include/perfect_cache.hpp"

int slow_get_page(int key) { return key; }

int main() {
  size_t cacheSize, inputLen;
  std::vector<int> input;

  std::cin >> cacheSize >> inputLen;

  for (int i = 0, data = 0; i < inputLen; ++i) {
    std::cin >> data;
    input.push_back(data);
  }

  PerfectCache<int, int> cache(cacheSize, inputLen, input);

  size_t hits = 0;
  for (int i = 0; i < inputLen; ++i) {
    hits += cache.lookup_update(input[i], slow_get_page);
  }

  std::cout << "HITS: " << hits << std::endl;
}
