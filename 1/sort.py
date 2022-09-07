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


def heapify(array, N, i):
    largest = i  
    l = 2 * i + 1
    r = 2 * i + 2
 
    if l < N and array[largest] < array[l]:
        largest = l
 
    if r < N and array[largest] < array[r]:
        largest = r
 
    if largest != i:
        array[i], array[largest] = array[largest], array[i]
 
        heapify(array, N, largest)

def heap_sort(array):
    N = len(array)
 
    for i in range(N//2 - 1, -1, -1):
        heapify(array, N, i)
 
    for i in range(N-1, 0, -1):
        array[i], array[0] = array[0], array[i]
        heapify(array, i, 0)


def insertion_sort(array):
    for i in range(1,len(array)):
        pivot=array[i]
        j=i-1
        while j>=0 and pivot< array[j]:
            array[j+1] = array[j]
            j -= 1
        array[j+1] = pivot
    
