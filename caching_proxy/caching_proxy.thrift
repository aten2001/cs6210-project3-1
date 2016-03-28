namespace cpp cachingproxy

service CachingProxy {
  string get_url(1:string url)

  void reset_cache()

  void set_cache_size(1:i32 cache_size)

  void set_warmup_period(1:i32 period)

  double get_hit_rate()

}
