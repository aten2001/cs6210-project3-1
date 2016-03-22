#ifndef _SIMPLE_CACHE_H
#define _SIMPLE_CACHE_H

#include <cstddef>
#include <string>
#include <unordered_map>
#include <functional>

#include "CacheRepPolicy.h"

class SimpleCache {
public:
  SimpleCache() {}
  SimpleCache(CacheRepPolicy &pol, size_t cache_size);
  ~SimpleCache() {}

  bool is_url_cached(const std::string& url);
  std::string get_url_cached(const std::string& url);
  void cache_url(const std::string& _return, const std::string& url);
  void free_cache(size_t size);

private:
  size_t         m_max_cache_size;
  size_t         m_cur_cache_size;	
  CacheRepPolicy m_rep_policy;
  std::unordered_map<size_t, std::string> m_cache;
};

#endif /* _SIMPLE_CACHE_H */
