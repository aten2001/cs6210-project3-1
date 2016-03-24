#include "FifoReplPolicy.h"

void FifoReplPolicy::Insert(std::string key) {
  fifo.push(key);
}

void FifoReplPolicy::Touch(std::string key) {

}

std::string FifoReplPolicy::RemoveReplacement() {
  std::string ret = fifo.front();
  fifo.pop();
  return ret;
}

void FifoReplPolicy::Reset() {
  fifo = std::queue<std::string>();
}