#ifndef PROJECT3_RANDREPLPOLICY_H
#define PROJECT3_RANDREPLPOLICY_H

#include <vector>
#include "WebPageCache.h"

class RandReplPolicy : public CacheReplPolicy {
 public:
  RandReplPolicy() {}
  ~RandReplPolicy() {}

  void Insert(const CacheEntry& entry);
  void Touch(const CacheEntry& entry);
  std::string GetReplacement(int32_t insert_content_size, int32_t current_cache_size, int32_t max_cache_size);
  void Reset();

 private:
  std::vector<std::string> entries;
};

#endif //PROJECT3_RANDREPLPOLICY_H
