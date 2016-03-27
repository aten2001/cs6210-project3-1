#include "WebPageCache.h"
#include "FifoReplPolicy.h"
#include "RandReplPolicy.h"
#include "MaxSizeReplPolicy.h"
#include <curl/curl.h>
#include <assert.h>

#define DEBUG 1

WebPageCache::WebPageCache(int32_t max_size, const std::string& repl_policy, int32_t warmup)
        : max_size_(max_size), warmup_(warmup), current_size_(0),
          cache_hits_(0), num_accesses_(0) {
  if (repl_policy == "FIFO") {
    repl_policy_ = new FifoReplPolicy();
  } else if (repl_policy == "MAXS") {
    repl_policy_ = new MaxSizeReplPolicy();
  } else if (repl_policy == "RAND") {
    repl_policy_ = new RandReplPolicy();
  } else {
    std::cerr << "Invalid Replacement Policy: " << repl_policy;
    assert(0);
  }
}

WebPageCache::~WebPageCache() {
  delete repl_policy_;
}

const std::string WebPageCache::GetWebPage(const std::string& url) {
  if (!warmup_)
    num_accesses_++;

  size_t hash_val = get_hash(url); // Hash the url

  // Identify the cache entries for the hashed value
  std::pair<std::multimap<size_t, CacheEntry>::iterator, std::multimap<size_t, CacheEntry>::iterator> entries_range;
  entries_range = cache.equal_range(hash_val);

  // Identify if there is an entry whose key matches the URL
  for (std::multimap<size_t, CacheEntry>::iterator entry = entries_range.first;
       entry != entries_range.second; ++entry) {
    CacheEntry& cache_entry = entry->second;
    if (cache_entry.getKey() == url) {
      if (!warmup_)
        cache_hits_++;
    #if DEBUG
      std::cout << "Cache Hit - Returning Page: " << url << std::endl;
    #endif
      repl_policy_->Touch(cache_entry);
      return cache_entry.getData();
    }
  }

  // Not present in the cache, fetch it
  std::string content;
  downloader_.DownloadWebPage(url, content);

  if (content.size() > max_size_) {
    std::cerr << "Content too large for cache" << std::endl;
    exit(EXIT_FAILURE);
  }

  while ((current_size_ + content.size()) > max_size_) {
    // Cache too large, need to replace data until  within size requirements
    std::string remove_url = repl_policy_->RemoveReplacement();
#if DEBUG
    std::cout << "Removing Page: " << remove_url << std::endl;
#endif
    RemoveWebPage(remove_url);
  }

#if DEBUG
  std::cout << "Inserting Page: " << url << std::endl;
#endif
  CacheEntry entry(url, content);

  repl_policy_->Insert(entry);
  current_size_ += content.size();
  cache.insert(std::pair<size_t, CacheEntry>(hash_val, entry));

  if (!warmup_)
    --warmup_;

  return content;
}

void WebPageCache::RemoveWebPage(const std::string& url) {
  size_t hash_val = get_hash(url); // Hash the url

  // Identify the cache entries for the hashed value
  std::pair<std::multimap<size_t, CacheEntry>::iterator, std::multimap<size_t, CacheEntry>::iterator> entries_range;
  entries_range = cache.equal_range(hash_val);

  // Identify if there is an entry whose key matches the URL
  for (std::multimap<size_t, CacheEntry>::iterator entry = entries_range.first;
       entry != entries_range.second; ++entry) {
    if (entry->second.getKey() == url) {
      // Remove the cache entry and adjust the new cache size
      current_size_ -= entry->second.getData().size();
      cache.erase(entry);
      return;
    }
  }
  assert(false);
}

size_t WebPageDownloader::WriteWebPageCallback(char* buf, size_t size, size_t nmemb, void* up) {
  std::string* str = (std::string*) up;
  str->reserve(str->size() + size * nmemb);
  str->append(buf, size * nmemb);
  return size * nmemb;
}

bool WebPageDownloader::DownloadWebPage(const std::string& url, std::string& content) {
  CURL* curl_handle;
  CURLcode res;

  curl_global_init(CURL_GLOBAL_ALL);

  /* init the curl session */
  curl_handle = curl_easy_init();

  /* specify URL to get */
  curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());

  /* send all data to this function  */
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteWebPageCallback);

  /* we pass our 'chunk' struct to the callback function */
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void*) &content);

  /* some servers don't like requests that are made without a user-agent
     field, so we provide one */
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

#if DEBUG
  /* output progress from curl */
  curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L); //tell curl to output its progress
#endif

  /* get it! */
  res = curl_easy_perform(curl_handle);

  /* check for errors */
  if (res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
  }

  /* cleanup curl stuff */
  curl_easy_cleanup(curl_handle);

  /* we're done with libcurl, so clean it up */
  curl_global_cleanup();

  return res == CURLE_OK;
}

