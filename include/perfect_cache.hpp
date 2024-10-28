#pragma once

// #define DEBUG
// #define TIME

#include <cstddef>
#include <deque>
#include <iostream>
#include <list>
#include <map>
#include <unordered_map>
#include <vector>

template <typename PageT, typename KeyT = int> class PerfectCache {
  using ListIt = typename std::list<PageT>::iterator;

  size_t cacheSize;
  size_t inputLen;
  size_t curPage = -1;
  size_t curAmount = 0;

  std::list<PageT> cache;
  std::unordered_map<KeyT, ListIt> hash;
  std::map<KeyT, std::deque<KeyT>, std::less<KeyT>> nextUse;
  std::unordered_map<KeyT, std::list<KeyT>> input;

  bool full() const { return (curAmount < cacheSize) ? false : true; }

  void debug_print() const {
    std::cout << "==========================================\n";

    for (const auto &it : input) {
      auto list = it.second;
      auto end = list.end();
      --end;

      std::cout << "[" << it.first << "]: ";
      for (const auto &iter : list)
        std::cout << iter << " ";

      std::cout << "\n";
    }
    std::cout << "==========================================\n\n";
  }

  int getNextHit(KeyT key) {
#ifdef DEBUG
    debug_print();
#endif // DEBUG
    input[key].pop_front();
    return input[key].front();
  }

public:
  PerfectCache(size_t &cacheSize, size_t &inputLen, std::vector<int> input_arr)
      : cacheSize(cacheSize), inputLen(inputLen) {
    int curPage = 0;

    for (const auto &it : input_arr) {
      input[it].push_back(curPage);
      ++curPage;
    }

    for (auto &it : input)
      it.second.push_back(inputLen);
  }

  template <typename F> bool lookup_update(KeyT key, F slow_get_page) {
    ++curPage;

    int nextItPlace = getNextHit(key);

    if (hash.find(key) == hash.end()) { // page not found

      // if you never meet this page again
      if (nextItPlace == inputLen) {
#ifdef DEBUG
        debug_print();
#endif // DEBUG
        return false;
      }

      if (full()) {
        auto it = nextUse.end();
        --it;
        cache.erase(hash[it->second.back()]);
        hash.erase(it->second.back());
        it->second.pop_back();

        if (it->second.empty())
          nextUse.erase(it);

        --curAmount;
      }

      ++curAmount;

      cache.push_front(slow_get_page(key));
      hash[key] = cache.begin();
      nextUse.erase(curPage);
      nextUse[nextItPlace].push_back(key);

      return false;
    }

    // page found
    nextUse.erase(curPage);
    nextUse[nextItPlace].push_back(key);

#ifdef DEBUG
    debug_print();
#endif // DEBUG
    return true;
  }
};
