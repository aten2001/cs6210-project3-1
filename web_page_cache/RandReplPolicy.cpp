#include "RandReplPolicy.h"
#include <stdlib.h>

void RandReplPolicy::Insert(const CacheEntry& entry) {
  entries.push_back(entry.getKey());
}

void RandReplPolicy::Touch(const CacheEntry& entry) {

}

std::string RandReplPolicy::GetReplacement(int32_t insert_content_size, int32_t current_cache_size, int32_t max_cache_size) {
  unsigned long replace = rand() % entries.size();
  std::string key = entries[replace];
  std::swap(entries[replace], entries.back());
  entries.pop_back();
  return key;
}

void RandReplPolicy::Reset() {
  entries.clear();
}
