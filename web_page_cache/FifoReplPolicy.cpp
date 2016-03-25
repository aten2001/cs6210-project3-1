#include "FifoReplPolicy.h"

void FifoReplPolicy::Insert(const CacheEntry& entry) {
  fifo_.push(entry.GetKey());
}

void FifoReplPolicy::Touch(const CacheEntry& entry) {

}

std::string FifoReplPolicy::RemoveReplacement() {
  std::string ret = fifo_.front();
  fifo_.pop();
  return ret;
}

void FifoReplPolicy::Reset() {
  fifo_ = std::queue<std::string>();
}