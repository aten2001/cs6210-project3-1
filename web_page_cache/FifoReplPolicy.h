#ifndef PROJECT3_FIFOREPLPOLICY_H
#define PROJECT3_FIFOREPLPOLICY_H

#include <queue>
#include "WebPageCache.h"

class FifoReplPolicy : public CacheReplPolicy {
 public:
  FifoReplPolicy() {}
  ~FifoReplPolicy() {}

  void Insert(const CacheEntry& entry);
  void Touch(const CacheEntry& entry);
  std::string RemoveReplacement();
  void Reset();

 private:
  std::queue<std::string> fifo_;
};

#endif //PROJECT3_FIFOREPLPOLICY_H
