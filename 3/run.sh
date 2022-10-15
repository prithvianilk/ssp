#!/bin/sh
gcc for_loop.c -o for_loop
gcc large_array.c -o large_array
taskset --cpu-list 0 sudo perf stat --cpu=0 -o ./outputs/for_loop.txt ./for_loop
taskset --cpu-list 0 sudo perf stat --cpu=0 -o ./outputs/large_array.txt ./large_array