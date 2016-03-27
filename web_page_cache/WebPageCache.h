#ifndef PROJECT3_WEBPAGECACHE_H
#define PROJECT3_WEBPAGECACHE_H

#include <string>
#include <map>
#include <iostream>

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
  static bool DownloadWebPage(const std::string& url, std::string& content);

 private:
  static size_t WriteWebPageCallback(char* buf, size_t size, size_t nmemb, void* up);

};

class CacheReplPolicy {
 public:
  virtual ~CacheReplPolicy() {};
  virtual void Insert(const CacheEntry& entry) = 0;
  virtual void Touch(const CacheEntry& entry) = 0;
  virtual std::string RemoveReplacement() = 0;
  virtual void Reset() = 0;
};

class WebPageCache {
 public:
  WebPageCache(int32_t max_size, const std::string& repl_policy, int32_t warmup = 0);

  ~WebPageCache();
  const std::string GetWebPage(const std::string& url);

  void PrintOccupancy() {
    std::cout << "Cache: " << cache.size() << " items, ";
    std::cout << (current_size_ / 1024) << "/" << (max_size_ / 1024) << "KB" << std::endl;
  }

  double GetHitRate() {
    if (num_accesses_ == 0)
      return 0;
    return (double)cache_hits_ / (double)num_accesses_;
  }

 private:
  int32_t current_size_;
  int32_t max_size_;
  int32_t cache_hits_;
  int32_t num_accesses_;
  int32_t warmup_;
  std::hash<std::string> hash_fn_;
  std::multimap<size_t, CacheEntry> cache;
  CacheReplPolicy* repl_policy_;

 private:
  void RemoveWebPage(const std::string& url);

  size_t get_hash(const std::string& str) {
    return hash_fn_(str);
  }

};

#endif //PROJECT3_WEBPAGECACHE_H
