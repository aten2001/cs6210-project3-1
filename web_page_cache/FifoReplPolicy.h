#ifndef PROJECT3_FIFOREPLPOLICY_H
#define PROJECT3_FIFOREPLPOLICY_H

#include <queue>
#include "WebPageCache.h"

class FifoReplPolicy : public CacheReplPolicy {
 public:
  FifoReplPolicy() {}
  ~FifoReplPolicy() {}

  void Insert(std::string key);
  void Touch(std::string key);
  std::string RemoveReplacement();
  void Reset();

 private:
  std::queue<std::string> fifo;
};

#endif //PROJECT3_FIFOREPLPOLICY_H
