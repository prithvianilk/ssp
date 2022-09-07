from copy import deepcopy
from random import randint as rand
import unittest
from sort import selection_sort, heap_sort, merge_sort, insertion_sort


class TestSortAlgos(unittest.TestCase):
    def test_selection_sort(self):
        self.exec_and_test_sort(selection_sort)

    def test_merge_sort(self):
        self.exec_and_test_sort(merge_sort)

    def test_heap_sort(self):
        self.exec_and_test_sort(heap_sort)

    def test_insertion_sort(self):
        self.exec_and_test_sort(insertion_sort)

    def exec_and_test_sort(self, sort):
        array, sorted_array = self.gen_array()
        sort(array)
        self.assertEqual(array, sorted_array)

    def gen_array(self):
        n, max_val = int(1e3), int(1e7)
        array = [rand(0, max_val) for _ in range(n)]
        sorted_array = sorted(deepcopy(array))
        return array, sorted_array


if __name__ == "__main__":
    unittest.main()
