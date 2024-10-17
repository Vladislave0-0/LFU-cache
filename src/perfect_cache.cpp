#include "../include/perfect_cache.hpp"
#ifdef TIME
  #include <chrono>
#endif // TIME

int slow_get_page(int key) { return key; }

int main() {
  size_t cacheSize, inputLen;
  std::vector<int> input;

  std::cin >> cacheSize >> inputLen;

#ifdef TIME
  auto start = std::chrono::high_resolution_clock::now();
#endif // TIME

  for (int i = 0, data = 0; i < inputLen; ++i) {
    std::cin >> data;
    input.push_back(data);
  }

  PerfectCache<int, int> cache(cacheSize, inputLen, input);

  size_t hits = 0;
  for (int i = 0; i < inputLen; ++i) {
    hits += cache.lookup_update(input[i], slow_get_page);
  }

#ifdef TIME
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout << "LFU-cache working time: " << duration.count() << " us" << std::endl; 
#endif // TIME

  std::cout << hits << std::endl;
}
