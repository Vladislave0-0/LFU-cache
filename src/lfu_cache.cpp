#include "../include/lfu_cache.hpp"
#include <vector>
#ifdef TIME
  #include <chrono>
#endif // TIME

int slow_get_page(int key) { return key; }

int main() {
  size_t cacheSize, pagesNum;
  std::vector<int> cacheBuf;

  std::cin >> cacheSize >> pagesNum;

#ifdef TIME
  auto start = std::chrono::high_resolution_clock::now();
#endif // TIME

  LFUCache<int, int> cache(cacheSize);

  for (int i = 0, data = 0; i < pagesNum; ++i) {
    std::cin >> data;
    cacheBuf.push_back(data);
  }

  size_t hits = 0;
  for (int i = 0; i < pagesNum; ++i) {
    hits += cache.lookup_update(cacheBuf[i], slow_get_page);
  }

#ifdef TIME
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout << "LFU-cache working time: " << duration.count() << " us" << std::endl; 
#endif // TIME

  std::cout << hits << std::endl;
}
