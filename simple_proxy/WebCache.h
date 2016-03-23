#ifndef _WEB_CACHE_H_
#define _WEB_CACHE_H_

#include <unordered_map>

typedef struct {
  size_t key;
  std::string data;
} web_element;

class WebCache {
private:
  // hash key <==> vector index mapping for cache
  std::unordered_map<size_t, std::string> cache;
public:
  void ins_url(const std::string& url, const std::string& data);
  std::string get_url_cached(const std::string& url);
  void del_url(const std::string& url);
  bool is_url_cached(const std::string& url);
};

#endif /* _WEB_CACHE_H_ */
