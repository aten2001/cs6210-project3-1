#include "LruReplPolicy.h"

void LruReplPolicy::Insert(const CacheEntry& entry) {
  lru_list_.push_back(entry.getKey());
}

void LruReplPolicy::Touch(const CacheEntry& entry) {
  std::list<std::string>::iterator it = std::find(lru_list_.begin(), lru_list_.end(), entry.getKey());
  lru_list_.splice(lru_list_.end(), lru_list_, it);
}

std::string LruReplPolicy::GetReplacement(int32_t insert_content_size, int32_t current_cache_size, int32_t max_cache_size) {
  std::string key = lru_list_.front();
  lru_list_.pop_front();
  return key;
}

void LruReplPolicy::Reset() {
  lru_list_.clear();
}
