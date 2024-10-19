#pragma once

// #define DEBUG
// #define TIME

#include <cstddef>
#include <iostream>
#include <limits>
#include <list>
#include <unordered_map>

template <typename PageT, typename KeyT = int> class LFUCache {
  using FreqT = size_t;
  using ListIt = typename std::list<PageT>::iterator;

  size_t cacheSize;
  size_t curAmount = 0;
  FreqT minFreq = 1;
  size_t hits = 0;

  std::list<PageT> cache;
  std::unordered_map<KeyT, ListIt> hash;
  std::unordered_map<KeyT, FreqT> freq;
  std::unordered_map<FreqT, std::list<KeyT>> listsOfFreqs;
  std::unordered_map<KeyT, ListIt> listsOfFreqsHash;

  bool full() const { return (curAmount < cacheSize) ? false : true; }

  void debug_print(const KeyT key) {
    std::cout << "==========================================\n";
    std::cout << "curPage = " << key << "\n";
    std::cout << "cache[minFreq = " << minFreq << "]:\n";
    for (auto it = cache.begin(); it != cache.end(); ++it) {
      std::cout << *it << "(" << freq[*it] << ")" << " ";
    }
    std::cout << "\n\n";

    std::cout << "listsOfFreqs:\n";
    for (int i = 1; i <= listsOfFreqs.size(); ++i) {
      std::cout << "freq[" << i << "]: ";
      for (auto it = listsOfFreqs[i].begin(); it != listsOfFreqs[i].end();
           ++it) {
        std::cout << *it << " ";
      }
      std::cout << std::endl;
    }
    std::cout << "==========================================\n\n";
  }

  void replace(KeyT key) {
    listsOfFreqs[freq[key]].erase(listsOfFreqsHash[key]);
    ++freq[key];
    listsOfFreqs[freq[key]].push_back(key);
    ListIt preLastIt = listsOfFreqs[freq[key]].end();
    listsOfFreqsHash[key] = --preLastIt;
  }

  void update_min_freq(const KeyT key) {
    minFreq = std::numeric_limits<FreqT>::max();
    for (const auto &pair : listsOfFreqs) {
      if (!pair.second.empty() && pair.first < minFreq) {
        minFreq = pair.first;
      }
    }
  }

public:
  LFUCache(size_t cacheSize) : cacheSize(cacheSize){};

  template <typename F> bool lookup_update(KeyT key, F slow_get_page) {
    if (hash.find(key) == hash.end()) { // page not found
      if (full()) {
        cache.erase(hash[listsOfFreqs[minFreq].front()]);
        hash.erase(listsOfFreqs[minFreq].front());
        listsOfFreqsHash.erase(listsOfFreqs[minFreq].front());
        listsOfFreqs[minFreq].pop_front();
        --curAmount;
      }

      ++curAmount;

      cache.push_front(slow_get_page(key));
      hash[key] = cache.begin();
      freq[key] = 1;

      listsOfFreqs[freq[key]].push_back(key);
      ListIt preLastIt = listsOfFreqs[freq[key]].end();
      listsOfFreqsHash[key] = --preLastIt;

      update_min_freq(key);

#ifdef DEBUG
      debug_print(key);
#endif // DEBUG
      return false;
    }

    replace(key); // page found

    // updating minimum frequency (faster than update_min_freq(key))
    if (listsOfFreqs[minFreq].empty() || minFreq > freq[key]) {
      minFreq = freq[key];
    }

#ifdef DEBUG
    debug_print(key);
#endif // DEBUG
    return true;
  }
};
