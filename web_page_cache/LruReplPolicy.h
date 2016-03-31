#ifndef PROJECT3_LRUREPLPOLICY_H
#define PROJECT3_LRUREPLPOLICY_H

#include <list>
#include <algorithm>
#include "WebPageCache.h"

class LruReplPolicy : public CacheReplPolicy {
 public:
  LruReplPolicy() { }

  ~LruReplPolicy() { }

  void Insert(const CacheEntry& entry) override;

  void Touch(const CacheEntry& entry) override;

  std::string GetReplacement(int32_t insert_content_size, int32_t current_cache_size, int32_t max_cache_size) override;

  void Reset() override;

 private:
  std::list<std::string> lru_list_;
};

#endif //PROJECT3_LRUREPLPOLICY_H
