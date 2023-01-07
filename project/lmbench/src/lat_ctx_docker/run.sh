#!/bin/bash

# ---- input params ----
# $1 cpu
# $2 container_id
# $3 read_pipe_path
# $4 write_pipe_path
# $5 process size

perf stat --cpu=$1 -e cache-references,cache-misses,L1-dcache-loads,L1-dcache-load-misses,L1-dcache-stores,LLC-loads,LLC-load-misses,LLC-stores,l2_rqsts.references,l2_rqsts.miss -o /perf-stats/$2.txt /bin/docker_proc $3 $4 $5