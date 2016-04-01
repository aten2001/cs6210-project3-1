namespace cpp cachingproxy

service CachingProxy {
  string get_url(1:string url)

  void set_cache_policy(1:string policy)

  void reset_cache()

  void set_cache_size(1:i32 cache_size)

  void set_warmup_period(1:i32 period)

  string get_cache_stats()
}
