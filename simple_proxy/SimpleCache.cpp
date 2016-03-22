#include "SimpleCache.h"

SimpleCache::SimpleCache(CacheRepPolicy &pol, size_t cache_size)
{
	m_max_cache_size = cache_size;
	m_cur_cache_size = 0;
	m_rep_policy     = pol;
}

bool SimpleCache::is_url_cached(const std::string& url)
{
	return false;
}

std::string SimpleCache::get_url_cached(const std::string& url)
{
	return NULL;
}

void SimpleCache::cache_url(const std::string& _return, const std::string& url)
{
	size_t hash_key = std::hash<std::string>{}(url);
	m_cache.insert(std::make_pair(hash_key, _return));
}
