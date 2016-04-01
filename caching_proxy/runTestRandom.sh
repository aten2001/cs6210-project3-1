#!/bin/bash

policies=( RAND FIFO LRU MAXS )
cache_sizes=( 1024 2048 4096 8192 12288 16384 20480 )
for policy in "${policies[@]}"
do
    echo $policy
    for cache_size in "${cache_sizes[@]}"
    do
	./CachingProxyTest --host starlotus.cc.gt.atl.ga.us -t ../tests/random_1100.txt -w 100 -r $policy -c $cache_size
    done
done
