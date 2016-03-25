#include "RandReplPolicy.h"
#include <stdlib.h>

void RandReplPolicy::Insert(const CacheEntry& entry) {
  entries.push_back(entry.getKey());
}

void RandReplPolicy::Touch(const CacheEntry& entry) {

}

std::string RandReplPolicy::RemoveReplacement() {
  unsigned long replace = rand() % entries.size();
  std::string key = entries[replace];
  std::swap(entries[replace], entries.back());
  entries.pop_back();
  return key;
}

void RandReplPolicy::Reset() {
  entries.clear();
}
