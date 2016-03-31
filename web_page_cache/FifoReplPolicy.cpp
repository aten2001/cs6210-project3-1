#include "FifoReplPolicy.h"

void FifoReplPolicy::Insert(const CacheEntry& entry) {
  fifo_.push(entry.getKey());
}

void FifoReplPolicy::Touch(const CacheEntry& entry) {

}

std::string FifoReplPolicy::GetReplacement(int32_t insert_content_size, int32_t current_cache_size, int32_t max_cache_size) {
  std::string ret = fifo_.front();
  fifo_.pop();
  return ret;
}

void FifoReplPolicy::Reset() {
  fifo_ = std::queue<std::string>();
}