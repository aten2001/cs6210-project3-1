#include "FifoReplPolicy.h"

void FifoReplPolicy::Insert(const CacheEntry& entry) {
  fifo.push(entry.GetKey());
}

void FifoReplPolicy::Touch(const CacheEntry& entry) {

}

std::string FifoReplPolicy::RemoveReplacement() {
  std::string ret = fifo.front();
  fifo.pop();
  return ret;
}

void FifoReplPolicy::Reset() {
  fifo = std::queue<std::string>();
}