#! /usr/bin/python3

# script to generate all lmbench container context switch results along with perf-stat results

# ------------------------------------------------------
# Example usage
# sudo ./scripts/get_lat_ctx_docker_stats.py demo/run1
# ------------------------------------------------------

import os
import psutil
import sys


COMMAND_PATH = "./bin/x86_64-linux-gnu/lat_ctx_docker -s 5 -F {}"
MAX_CONTAINER_COUNT = 5
PERF_STAT_NAMES = ["# of containers", "LLC retired instruction load miss rate"]
NUMBER_OF_EVENTS = 2

def create_output_dir(dirpath):
    os.makedirs(dirpath, mode = 0o777, exist_ok = True)

def parse_perf(filename):
    with open(filename, 'r') as f:
        lines = f.readlines()
        data = list(map(lambda x: int(x.strip().split()[0]), lines[5: 7]))
        return data

def get_average_perf_stats(no_of_containers, perf_stats_path):
    values = [0] * NUMBER_OF_EVENTS 
    valid_perf_stat_count = 0
    for container_id in range(1, no_of_containers + 1):
        try:
            data = parse_perf(f"{perf_stats_path}/{container_id}.txt")
            for i in range(NUMBER_OF_EVENTS):
                values[i] += data[i]
            valid_perf_stat_count += 1
        except:
            continue

    for i in range(NUMBER_OF_EVENTS):
        values[i] /= valid_perf_stat_count

    return values

def write_averages_to_lmbench_log(dirpath, no_of_containers, values):
    with open(f"{dirpath}/container_{no_of_containers}.txt", "a") as f:
        f.write(",\t".join(PERF_STAT_NAMES))
        f.write("\n")
        retired_l3_miss_rate = (values[1] * 100) / (values[0] + values[1])
        f.write(",\t".join(list(map(str, [no_of_containers, retired_l3_miss_rate]))))

def run_bench(dirpath):
    for no_of_containers in [2, 4, 6, 8, 16, 24, 32, 64, 128]:
        perf_stats_path = dirpath+"/perf_stats/no_of_containers/container_{}".format(no_of_containers)
        os.makedirs(perf_stats_path, mode = 0o777, exist_ok=True)
        command_path = COMMAND_PATH.format(perf_stats_path)
        command = f'sudo {command_path} {no_of_containers} > {dirpath}/container_{no_of_containers}.txt 2>&1'
        os.system(command)

        values = get_average_perf_stats(no_of_containers, perf_stats_path)
        write_averages_to_lmbench_log(dirpath, no_of_containers, values)

def get_data_for_graph(dirpath):
    l3_cache_mr = dict()
    context_switch_time = dict()
    for filename in os.listdir('./'+dirpath):
        try:
            filename = os.path.join(dirpath,filename)
            with open(filename,'r') as f:
                lines = f.readlines()
                data = lines[5].strip().split()
                data = list(map(lambda x: x.replace(',',""), data))
                l3_cache_mr[data[0]] = float(data[1])
                ctx_switch_time = lines[3].strip().split(',')
                context_switch_time[data[0]] = float(ctx_switch_time[1])
        except:
            continue
    print("l3_cache_mr= ", sorted(l3_cache_mr.items(), key = lambda x: int(x[0])))
    print("ctx_swt_time= ", sorted(context_switch_time.items(), key = lambda x: int(x[0])))

def gen_container_size_stats(dirpath):
    size_options = ["sm", "m", "l", "xl", "xxl"]
    for size in size_options:
        perf_stats_path = dirpath+"/perf_stats/sizes/size_{}".format(size)
        os.makedirs(perf_stats_path, mode = 0o777, exist_ok=True)
        command_path = COMMAND_PATH.format(perf_stats_path)
        command = f'sudo {command_path} -I docker_proc_{size} {MAX_CONTAINER_COUNT} > {dirpath}/size_{size}.txt 2>&1'
        os.system(command)

def gen_num_layer_stats(dirpath):
    for layers in range(1,6):
        perf_stats_path = dirpath+"/perf_stats/layers/layers_{}".format(layers)
        os.makedirs(perf_stats_path, mode = 0o777, exist_ok=True)
        command_path = COMMAND_PATH.format(perf_stats_path)
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
            command_path = COMMAND_PATH.format(perf_stats_path)
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

    get_data_for_graph(dirpath)

    # print("VARYING CONTAINER SIZE")
    # gen_container_size_stats(dirpath)

    # print("VARYING CONTAINER LAYERS")
    # gen_num_layer_stats(dirpath)

    # print("VARYING MEMORY")
    # gen_memory_sensitivity_stats(dirpath)

