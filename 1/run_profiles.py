import cProfile
import os
import sys
from random import seed

from sort import (gen_array, heap_sort, insertion_sort, merge_sort,
                  selection_sort)

SEED_VALUE = 23


def run_profiles():
    array_size = int(1e4)
    for _ in range(2):
        for name in ["insertion_sort", "selection_sort", "merge_sort", "heap_sort"]:
            print("Running", name, "on", array_size)
            command = f"python3 ./run_profiles.py {name} {array_size} > ./profiles/{name.replace(' ', '_')}_{array_size}.txt"
            os.system(command)
        array_size *= 10


def get_sort(algo_name: str):
    if algo_name == "insertion_sort":
        return insertion_sort
    elif algo_name == 'heap_sort':
        return heap_sort
    elif algo_name == 'merge_sort':
        return merge_sort
    elif algo_name == 'selection_sort':
        return selection_sort
    raise Exception(f"No sorting algorithm: {algo_name}")


def profile(algo_name: str, array_size: int):
    array = gen_array(array_size)
    sort = get_sort(algo_name)
    sort(array)


if __name__ == "__main__":
    command_type = sys.argv[1]
    if command_type == "all":
        run_profiles()
    else:
        seed(SEED_VALUE)
        algo_name, array_size = sys.argv[1], int(sys.argv[2])
        cProfile.run(f"profile('{algo_name}', {array_size})")
