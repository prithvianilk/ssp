def swap(i, j, array):
    temp = array[i]
    array[i] = array[j]
    array[j] = temp


def selection_sort(array):
    n = len(array)
    for i in range(n):
        for j in range(i + 1, n):
            if array[i] > array[j]:
                swap(i, j, array)


def merge_sort(array):
    raise NotImplementedError()


def heap_sort(array):
    raise NotImplementedError()
