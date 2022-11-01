#!/bin/bash
gcc for_loop.c -o for_loop
gcc large_array.c -o large_array
for i in {1..10}
do
	taskset --cpu-list 7 sudo perf stat --cpu=7 -e task-clock,instructions,cycles,branch-instructions,branch-misses,cache-references,cache-misses -o ./outputs/for_loop/for_loop_$i.txt ./for_loop;
	taskset --cpu-list 7 sudo perf stat --cpu=7 -e task-clock,instructions,cycles,branch-instructions,branch-misses,cache-references,cache-misses -o ./outputs/large_array/large_array_$i.txt ./large_array;
done
taskset --cpu-list 7 sudo perf stat --cpu=7 -r 10 --table -e task-clock,instructions,cycles,branch-instructions,branch-misses,cache-references,cache-misses -o ./outputs/for_loop/average_for_loop_10_times.txt ./for_loop;
taskset --cpu-list 7 sudo perf stat --cpu=7 -r 10 --table -e task-clock,instructions,cycles,branch-instructions,branch-misses,cache-references,cache-misses -o ./outputs/large_array/average_large_array_10_times.txt ./large_array;