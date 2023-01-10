#!/bin/bash

# ---- input params ----
# $1 cpu
# $2 container_id
# $3 read_pipe_path
# $4 write_pipe_path
# $5 process size
# $6 monitoring tool (perf | valgrind)

if [[ "$6" == "perf" ]]; then
    perf stat --cpu=$1 -e mem_load_retired.l1_hit,mem_load_retired.l1_miss,mem_load_retired.l2_hit,mem_load_retired.l2_miss,mem_load_retired.l3_hit,mem_load_retired.l3_miss -o /perf-stats/$2.txt /bin/docker_proc $3 $4 $5
else
    valgrind --log-file=/valgrind-stats/$2.txt --tool=cachegrind /bin/docker_proc $3 $4 $5
fi