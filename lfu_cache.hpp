#pragma once 

#include <cstddef>
#include <list>
#include <unordered_map>
#include <iostream>
#include <algorithm>

template <typename PageT, typename KeyT = int>
class LFUCache {
    using FreqT  = size_t;
    using ListIt = typename std::list<PageT>::iterator;

    size_t cacheSize;
    size_t curAmount = 0;
    FreqT minFreq = 1;

    std::list<PageT> cache;
    std::unordered_map<KeyT, ListIt> hash;
    std::unordered_map<KeyT, FreqT> freq;
    std::unordered_map<FreqT, std::list<KeyT>> listsOfFreqs;
    // std::unordered_map<KeyT, ListIt> listsOfFreqsHash;

    void debug_print() {
        std::cout << "==========================================\n";
        std::cout << "cache[minFreq = " << minFreq << "]:\n";
        for (auto it = cache.begin(); it != cache.end(); ++it) {
            std::cout << *it << "(" << freq[*it] << ")"<< " ";
        }
        std::cout << "\n\n";

        std::cout << "listsOfFreqs:\n";
        for (int i = 0; i < listsOfFreqs.size(); ++i) {
            std::cout << "freq[" << i << "]: ";
            for (auto it = listsOfFreqs[i].begin(); it != listsOfFreqs[i].end(); ++it) {
                std::cout << *it << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "==========================================\n\n";
    }

    void replace(KeyT key) {
        listsOfFreqs[freq[key]].erase(std::find(listsOfFreqs[freq[key]].begin(), listsOfFreqs[freq[key]].end(), *hash[key]));
        ++freq[key];
        listsOfFreqs[freq[key]].push_back(key);
    }

public:
    LFUCache(size_t cacheSize) : cacheSize(cacheSize) {};

    template <typename F>
    bool lookup_update(KeyT key, F slow_get_page) {
        if(hash.find(key) == hash.end()) { // not found
            if(full()) {
                cache.erase(hash[listsOfFreqs[minFreq].front()]);
                hash.erase(listsOfFreqs[minFreq].front());
                listsOfFreqs[minFreq].pop_front();
                --curAmount;
            }

            cache.push_front(slow_get_page(key));
            hash[key] = cache.begin();

            if(freq.find(key) == freq.end()) {
                freq[key] = 1;
            } 
            else {
                ++freq[key];
            }

            listsOfFreqs[freq[key]].push_back(key);

            if (listsOfFreqs[minFreq].empty() || minFreq > freq[key]) {
                minFreq = freq[key];
            }

            ++curAmount;
            // debug_print();
            return false;
        }

        replace(key); //found

        if (listsOfFreqs[minFreq].empty() || minFreq > freq[key]) {
            minFreq = freq[key];
        }

        // debug_print();
        return true;
    }

    bool full() const {
        return (curAmount < cacheSize) ? false : true;
    }
};
