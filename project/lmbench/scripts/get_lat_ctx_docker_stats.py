#! /usr/bin/python3

import os

COMMAND_PATH = "./bin/x86_64-linux-gnu/lat_ctx_docker"
MAX_CONTAINER_COUNT = 5

def run_bench():
    container_count = " ".join([str(container_count) for container_count in range(2, MAX_CONTAINER_COUNT + 1)])
    command = f'sudo {COMMAND_PATH} {container_count}'
    os.system(command)

def gen_container_size_stats():
    size_options = ["sm", "m", "l", "xl", "xxl"]
    for size in size_options:
        command = f'sudo {COMMAND_PATH} -I docker_proc_{size} {MAX_CONTAINER_COUNT}'
        os.system(command)

def gen_num_layer_stats():
    for layers in range(1,6):
        command = f'sudo {COMMAND_PATH} -I docker_proc_{layers} {MAX_CONTAINER_COUNT}'
        os.system(command)

def gen_memory_sensitivity_stats():
    pass

if __name__ == "__main__":
    run_bench()

