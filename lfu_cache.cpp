// #define DEBUG

#include "lfu_cache.hpp"
#include <fstream>
#include <vector>


int slow_get_page(int key) {
    return key;
}

int main() {
    size_t cacheSize, pagesNum;
    std::vector<int> cacheBuf;
    std::ifstream inputFile("data.txt");

    inputFile >> cacheSize >> pagesNum;
    LFUCache<int, int> cache(cacheSize);

    for(int i = 0, data = 0; i < pagesNum; ++i) {
        inputFile >> data;
        cacheBuf.push_back(data);
    }

    size_t hits = 0;
    for(int i = 0; i < pagesNum; i++) {
        hits += cache.lookup_update(cacheBuf[i], slow_get_page);
    }

    std::cout << "HITS: " << hits << std::endl;
}
