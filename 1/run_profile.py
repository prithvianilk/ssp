import cProfile
from random import seed
import sys
from sort import heap_sort, insertion_sort, merge_sort, selection_sort, gen_array


def get_sort(algo_name: str):
    if algo_name == "insertion sort":
        return insertion_sort
    elif algo_name == 'heap sort':
        return heap_sort
    elif algo_name == 'merge sort':
        return merge_sort
    elif algo_name == 'selection sort':
        return selection_sort
    raise Exception(f"No sorting algorithm: {algo_name}")


def profile(algo_name: str, array_size: int):
    array = gen_array(array_size)
    sort = get_sort(algo_name)
    sort(array)


if __name__ == "__main__":
    seed(23)
    algo_name, array_size = sys.argv[1], int(sys.argv[2])
    cProfile.run(f"profile('{algo_name}', {array_size})")
