#ifndef PROJECT3_WEBPAGECACHE_H
#define PROJECT3_WEBPAGECACHE_H

#include <string>
#include <unordered_map>
#include <iostream>
#include <sstream>

class CacheEntry {
 public:
  CacheEntry(const std::string& key, const std::string& data) : key_(key), data_(data) { }

  ~CacheEntry() { }

  const std::string& getKey() const { return key_; }

  const std::string& getData() const { return data_; }

  unsigned long getSize() const { return data_.size(); }

 private:
  std::string key_;
  std::string data_;
};

class WebPageDownloader {
 public:
  bool DownloadWebPage(const std::string& url, std::string& content);

 private:
  static size_t WriteWebPageCallback(char* buf, size_t size, size_t nmemb, void* up);

};

class CacheReplPolicy {
 public:
  virtual ~CacheReplPolicy() {};
  virtual void Insert(const CacheEntry& entry) = 0;
  virtual void Touch(const CacheEntry& entry) = 0;
  virtual std::string GetReplacement(int32_t insert_content_size, int32_t current_cache_size, int32_t max_cache_size) = 0;
  virtual void Reset() = 0;
};

class WebPageCache {
 public:
  WebPageCache(const std::string& repl_policy, int32_t max_size = 1024, int32_t warmup = 0);

  ~WebPageCache();
  const std::string GetWebPage(const std::string& url);


  void SetCacheSize(int32_t max_size) {
    max_size_ = max_size;
  }

  void Reset() {
    cache.clear();
    current_size_ = 0;
    repl_policy_->Reset();
    cache_hits_ = 0;
    sum_cache_items_ = 0;
    sum_cache_occupancy_ = 0;
    num_accesses_ = 0;
  }

  void SetWarmupPeriod(int32_t period) {
    warmup_period_ = period;
  }

  double GetHitRate() {
    if (num_accesses_ == 0)
      return 0;
    return (double)cache_hits_ / (double)num_accesses_;
  }

  double GetAvgNumCacheItems() {
    if (num_accesses_ == 0)
      return 0;
    return (double)sum_cache_items_ / (double)num_accesses_;
  }

  double GetAvgCacheOccupancy() {
    if (num_accesses_ == 0)
      return 0;
    return sum_cache_occupancy_ / num_accesses_;
  }

  void PrintOccupancy() {
    std::cout << "Cache: " << cache.size() << " items, ";
    std::cout << (current_size_ / 1024) << "/" << (max_size_ / 1024) << "KB" << std::endl;
  }

  std::string GetCacheStats() {
    std::ostringstream stats_stream;
    stats_stream << "Num Accesses: " << num_accesses_ << std::endl;
    stats_stream << "Hit Rate: " << GetHitRate() << std::endl;
    stats_stream << "Avg. Num Items: " << GetAvgNumCacheItems() << std::endl;
    stats_stream << "Avg. Cache Occupancy: " << GetAvgCacheOccupancy();
    return stats_stream.str();
  }

 private:
  int32_t current_size_;
  int32_t max_size_;
  int32_t cache_hits_;
  int64_t sum_cache_items_;
  double sum_cache_occupancy_;
  int32_t num_accesses_;
  int32_t warmup_period_;
  std::hash<std::string> hash_fn_;
  std::unordered_multimap<size_t, CacheEntry> cache;
  CacheReplPolicy* repl_policy_;
  WebPageDownloader downloader_;

 private:
  void RemoveWebPage(const std::string& url);

  size_t get_hash(const std::string& str) {
    return hash_fn_(str);
  }

};

#endif //PROJECT3_WEBPAGECACHE_H
