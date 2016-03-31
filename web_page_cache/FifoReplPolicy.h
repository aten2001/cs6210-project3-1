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
  std::string GetReplacement(int32_t insert_content_size, int32_t current_cache_size, int32_t max_cache_size);
  void Reset();

 private:
  std::queue<std::string> fifo_;
};

#endif //PROJECT3_FIFOREPLPOLICY_H
