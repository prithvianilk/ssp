#! /usr/bin/python3

import os
import psutil

COMMAND_PATH = "./bin/x86_64-linux-gnu/lat_ctx_docker"
MAX_CONTAINER_COUNT = 5

def run_bench():
    container_count = " ".join([str(container_count) for container_count in range(2, MAX_CONTAINER_COUNT + 1)])
    command = f'sudo {COMMAND_PATH} {container_count} > outputs/bench.txt 2>&1'
    os.system(command)

def gen_container_size_stats():
    size_options = ["sm", "m", "l", "xl", "xxl"]
    for size in size_options:
        command = f'sudo {COMMAND_PATH} -I docker_proc_{size} {MAX_CONTAINER_COUNT} > outputs/size_{size}.txt 2>&1'
        os.system(command)

def gen_num_layer_stats():
    for layers in range(1,6):
        command = f'sudo {COMMAND_PATH} -I docker_proc_l{layers} {MAX_CONTAINER_COUNT} > outputs/layers_{layers}.txt 2>&1'
        os.system(command)

def run_memtester(i):
    command = f'sudo memtester {i}G > /dev/null'
    os.system(command)

def gen_memory_sensitivity_stats():
    for gb in range(1,6):
        n=os.fork()
        if n > 0:
            command = f'sudo {COMMAND_PATH} {MAX_CONTAINER_COUNT} > outputs/mem_{gb}.txt 2>&1'
            os.system(command)
            parent = psutil.Process(os.getpid())
            for child in parent.children(recursive=True):
                child.kill()
  
        else:
            run_memtester(gb)

if __name__ == "__main__":
    print("NORMAL EXECUTION")
    run_bench()
    print("VARYING CONTAINER SIZE")
    gen_container_size_stats()
    print("VARYING CONTAINER LAYERS")
    gen_num_layer_stats()
    print("VARYING MEMORY")
    gen_memory_sensitivity_stats()

