#include <functional>

#include "WebCache.h"

static size_t get_hash(const std::string& url)
{
  return std::hash<std::string>{}(url);
}

void WebCache::ins_url(const std::string& url, const std::string& data)
{
  size_t key = get_hash(url);

  cache.insert(std::make_pair(key, data));
}

std::string WebCache::get_url_cached(const std::string& url)
{
  size_t key = get_hash(url);
  return cache[key];
}

void WebCache::del_url(const std::string& url)
{
  size_t key = get_hash(url);

  cache.erase(key);
}

bool WebCache::is_url_cached(const std::string& url)
{
  size_t key = get_hash(url);

  return cache.find(key) != cache.end();
}
