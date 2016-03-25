#ifndef PROJECT3_MAXSIZEREPLPOLICY_H
#define PROJECT3_MAXSIZEREPLPOLICY_H

#include <queue>
#include "WebPageCache.h"


class MaxSizeReplPolicy : public CacheReplPolicy {
 private:
  class MaxSizeEntry {
   public:
    MaxSizeEntry(const std::string& key, unsigned long size)
            : key_(key), size_(size) {}
    ~MaxSizeEntry() {}

    const std::string& getKey() const {
      return key_;
    }


    unsigned long getSize() const {
      return size_;
    }

    bool operator <(const MaxSizeEntry& rhs) const {
      return size_ < rhs.size_;
    }
   private:
    std::string key_;
    unsigned long size_;
  };

 public:
  MaxSizeReplPolicy() {}
  ~MaxSizeReplPolicy() {}

  void Insert(const CacheEntry& entry);
  void Touch(const CacheEntry& entry);
  std::string RemoveReplacement();
  void Reset();

 private:
  std::priority_queue<MaxSizeEntry> prio_queue_;
};


#endif //PROJECT3_MAXSIZEREPLPOLICY_H
