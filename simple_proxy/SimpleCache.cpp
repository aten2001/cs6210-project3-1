#include "SimpleCache.h"

SimpleCache::SimpleCache(CacheRepPolicy &pol, size_t cache_size)
{
  m_max_cache_size = cache_size;
  m_cur_cache_size = 0;
  m_rep_policy     = pol;
}

bool SimpleCache::is_url_cached(const std::string& url)
{
  size_t hash_key = std::hash<std::string>{}(url);

  auto search = m_cache.find(hash_key);
  if (search != m_cache.end())
    return true;

  return false;
}

std::string SimpleCache::get_url_cached(const std::string& url)
{
  size_t hash_key = std::hash<std::string>{}(url);

  return m_cache[hash_key];
}

void SimpleCache::cache_url(const std::string& _return, const std::string& url)
{
  size_t hash_key = std::hash<std::string>{}(url);
  size_t size     = _return.size();

  if (m_cur_cache_size + size > m_max_cache_size)
    free_cache(size);
  
  m_cache.insert(std::make_pair(hash_key, _return));
  m_cur_cache_size += _return.size();
}

void SimpleCache::free_cache(size_t size)
{
}
