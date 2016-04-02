#!/bin/bash

workloads=( rev_size_new )
for workload in "${workloads[@]}"
do
    grep Items ${workload}_results.txt | awk '{ print $4; }' | xargs -n7 >> ${workload}_pages.txt
    grep Occupancy ${workload}_results.txt | awk '{ print $4; }' | xargs -n7 >> ${workload}_occ.txt
    grep Hit ${workload}_results.txt | awk '{ print $3; }' | xargs -n7 >> ${workload}_hit.txt
    grep Time ${workload}_results.txt | awk '{ print $3; }' | xargs -n7 >> ${workload}_time.txt
done
