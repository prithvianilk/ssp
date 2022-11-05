#!/bin/bash
gcc 2d-array.c -o 2d-array
gcc 3d-array.c -o 3d-array
gcc 2d-array-col.c -o 2d-array-col
gcc 3d-array-col.c -o 3d-array-col

N=( 100 250 500 750 1000 2500 5000 7500 10000 )

rm ./outputs/traversal_times

for i in "${N[@]}"
do
	taskset --cpu-list 7 sudo perf stat --cpu=7 -e task-clock,cache-references,cache-misses -o ./outputs/2d-array/2d-array_$i.txt ./2d-array $i >> ./outputs/traversal_times;
done

for i in "${N[@]}"
do
	taskset --cpu-list 7 sudo perf stat --cpu=7 -e task-clock,cache-references,cache-misses -o ./outputs/2d-array-col/2d-array_$i.txt ./2d-array-col $i >> ./outputs/traversal_times;
done

N=( 10 25 50 75 100 250 500 750 1000 )
for i in "${N[@]}"
do
	taskset --cpu-list 7 sudo perf stat --cpu=7 -e task-clock,cache-references,cache-misses -o ./outputs/3d-array/3d-array_$i.txt ./3d-array $i >> ./outputs/traversal_times;
done

for i in "${N[@]}"
do
	taskset --cpu-list 7 sudo perf stat --cpu=7 -e task-clock,cache-references,cache-misses -o ./outputs/3d-array-col/3d-array_col_$i.txt ./3d-array-col $i >> ./outputs/traversal_times;
done