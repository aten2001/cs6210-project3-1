#include "CacheManager.h"

CacheManager::CacheManager(CacheRepPolicy &pol, size_t cache_size)
{
  m_max_cache_size = cache_size;
  m_cur_cache_size = 0;
  m_rep_policy     = pol;
}

bool CacheManager::is_url_cached(const std::string& url)
{
  return m_cache.is_url_cached(url);
}

std::string CacheManager::get_url_cached(const std::string& url)
{
  return m_cache.get_url_cached(url);
}

void CacheManager::cache_url(const std::string& _return, const std::string& url)
{
  size_t size     = _return.size();

  if (m_cur_cache_size + size > m_max_cache_size)
    free_cache(size);
  
  m_cache.ins_url(url, _return);
  m_cur_cache_size += _return.size();
}

void CacheManager::free_cache(size_t size)
{
}
