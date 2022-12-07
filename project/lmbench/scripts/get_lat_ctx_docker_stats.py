#! /usr/bin/python3

import os

COMMAND_PATH = "./bin/x86_64-linux-gnu/lat_ctx_docker"
MAX_CONTAINER_COUNT = 5

def run_bench():
    for container_count in range(1, MAX_CONTAINER_COUNT + 1):
        command = f'sudo {COMMAND_PATH} {container_count + 1}'
        os.system(command)

def gen_container_size_stats():
    pass

def gen_num_layer_stats():
    pass

def gen_memory_sensitivity_stats():
    pass

if __name__ == "__main__":
    run_bench()

