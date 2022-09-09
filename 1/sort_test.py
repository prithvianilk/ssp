import unittest

from sort import (gen_array, heap_sort, insertion_sort, merge_sort,
                  selection_sort)


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
        array = gen_array(int(1e3))
        sort(array)
        self.assertEqual(array, sorted(array))


if __name__ == "__main__":
    unittest.main()
