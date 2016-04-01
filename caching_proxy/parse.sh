#!/bin/bash

workloads=( random size )
for workload in "${workloads[@]}"
do
    grep Items ${workload}_results.txt | awk '{ print $4; }' >> ${workload}_pages.txt
    grep Occupancy ${workload}_results.txt | awk '{ print $4; }' >> ${workload}_occ.txt
    grep Hit ${workload}_results.txt | awk '{ print $3; }' >> ${workload}_hit.txt
    grep Time ${workload}_results.txt | awk '{ print $3; }' >> ${workload}_time.txt
done
