#! /usr/bin/python3

# script to generate all lmbench container context switch results along with perf-stat results

# ------------------------------------------------------
# Example usage
# sudo ./scripts/get_lat_ctx_docker_stats.py demo/run1
# ------------------------------------------------------

import os
import psutil
import sys

COMMAND_PATH = "taskset --cpu-list {} ./bin/x86_64-linux-gnu/lat_ctx_docker -s 5 -F {}"
MAX_CONTAINER_COUNT = 5


def create_output_dir(dirpath):
    os.makedirs(dirpath, mode = 0o777, exist_ok = True)

def run_bench(dirpath):
    for no_of_containers in range(10,50+1,10):
        perf_stats_path = dirpath+"/perf_stats/no_of_containers/container_{}".format(no_of_containers)
        os.makedirs(perf_stats_path, mode = 0o777, exist_ok=True)
        command_path = COMMAND_PATH.format(3, perf_stats_path)
        command = f'sudo {command_path} {no_of_containers} > {dirpath}/container_{no_of_containers}.txt 2>&1'
        os.system(command)

def gen_container_size_stats(dirpath):
    size_options = ["sm", "m", "l", "xl", "xxl"]
    for index, size in enumerate(size_options):
        perf_stats_path = dirpath+"/perf_stats/sizes/size_{}".format(size)
        os.makedirs(perf_stats_path, mode = 0o777, exist_ok=True)
        command_path = COMMAND_PATH.format(index + 3, perf_stats_path)
        command = f'sudo {command_path} -I docker_proc_{size} {MAX_CONTAINER_COUNT} > {dirpath}/size_{size}.txt 2>&1'
        os.system(command)

def gen_num_layer_stats(dirpath):
    for layers in range(1,6):
        perf_stats_path = dirpath+"/perf_stats/layers/layers_{}".format(layers)
        os.makedirs(perf_stats_path, mode = 0o777, exist_ok=True)
        command_path = COMMAND_PATH.format(layers + 2, perf_stats_path)
        command = f'sudo {command_path} -I docker_proc_l{layers} {MAX_CONTAINER_COUNT} > {dirpath}/layers_{layers}.txt 2>&1'
        os.system(command)

def run_memtester(size_in_GB):
    command = f'sudo taskset --cpu-list 2 memtester {size_in_GB}G > /dev/null'
    os.system(command)

def gen_memory_sensitivity_stats(dirpath):
    for iteration in range(1,6):
        size_in_GB = iteration * 3
        n=os.fork()
        if n > 0:
            perf_stats_path = dirpath+"/perf_stats/mem/mem_{}".format(size_in_GB)
            os.makedirs(perf_stats_path, mode = 0o777, exist_ok=True)
            command_path = COMMAND_PATH.format(iteration + 2, perf_stats_path)
            command = f'sudo {command_path} {MAX_CONTAINER_COUNT} > {dirpath}/mem_{size_in_GB}.txt 2>&1'
            os.system(command)
            parent = psutil.Process(os.getpid())
            for child in parent.children(recursive=True):
                child.kill()
        else:
            run_memtester(size_in_GB)

if __name__ == "__main__":
    dirpath = str(sys.argv[1])
    create_output_dir(dirpath)

    print("NORMAL EXECUTION")
    run_bench(dirpath)

    print("VARYING CONTAINER SIZE")
    gen_container_size_stats(dirpath)

    print("VARYING CONTAINER LAYERS")
    gen_num_layer_stats(dirpath)

    print("VARYING MEMORY")
    gen_memory_sensitivity_stats(dirpath)

