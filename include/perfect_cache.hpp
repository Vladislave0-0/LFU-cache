#pragma once

// #define DEBUG

#include <cstddef>
#include <deque>
#include <iostream>
#include <list>
#include <map>
#include <unordered_map>
#include <vector>

template <typename PageT, typename KeyT = int> class PerfectCache {
  using LenT = int;
  using ListIt = typename std::list<PageT>::iterator;

  size_t cacheSize;
  size_t inputLen;
  size_t curPage = -1;
  size_t curAmount = 0;

  std::list<PageT> cache;
  std::unordered_map<KeyT, ListIt> hash;
  std::map<LenT, std::deque<KeyT>, std::less<LenT>> nextUse;
  std::unordered_map<KeyT, std::list<KeyT>> input;

  bool full() const { return (curAmount < cacheSize) ? false : true; }

  void debug_print() {
    std::cout << "==========================================\n";

    for (auto it = input.begin(); it != input.end(); ++it) {
      auto list = it->second;
      auto end = list.end();
      --end;
      std::cout << "[" << it->first << "]: ";
      for(auto iter = list.begin(); iter != end; ++iter) {
          std::cout << *iter << " ";
      }
      std::cout << std::endl;
    }
    std::cout << "==========================================\n\n";
  }

  int getNextHit(KeyT key) {
#ifdef DEBUG
    debug_print();
#endif
    input[key].pop_front();
    return input[key].front();
  }

public:
  PerfectCache(size_t &cacheSize, size_t &inputLen, std::vector<int> input_arr)
      : cacheSize(cacheSize), inputLen(inputLen) {
    int curPage = 0;
    for (auto it = input_arr.begin(); it != input_arr.end(); ++it, ++curPage) {
      input[*it].push_back(curPage);
    }
    for (auto it = input.begin(); it != input.end(); ++it) {
      it->second.push_back(inputLen);
    }
  }

  template <typename F> bool lookup_update(KeyT key, F slow_get_page) {
    ++curPage;

    int nextItPlace = getNextHit(key);

    if (hash.find(key) == hash.end()) { // page not found

      // if you never meet this page again  
      if (nextItPlace == inputLen) {
#ifdef DEBUG
        debug_print();
#endif
        return false;
      }

      if (full()) {
        auto it = nextUse.end();
        --it;
        cache.erase(hash[it->second.back()]);
        hash.erase(it->second.back());
        it->second.pop_back();

        if (it->second.empty()) {
          nextUse.erase(it);
        }

        --curAmount;
      }

      ++curAmount;
      cache.push_front(slow_get_page(key));
      hash[key] = cache.begin();
      nextUse.erase(curPage);
      nextUse[nextItPlace].push_back(key);

      return false;
    }

    // found
    nextUse.erase(curPage);
    nextUse[nextItPlace].push_back(key);

#ifdef DEBUG
    debug_print();
#endif
    return true;
  }
};
