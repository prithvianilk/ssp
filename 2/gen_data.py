import os


fnames = ['og_parallel_pi.cc', 'parallel_pi.cc']
for fname in fnames:
    os.system(f'g++ -std=c++20 {fname}')
    for i in range(1, 6):
        thread_count = 10 ** i
        os.system(f'(time ./a.out {thread_count}) &> ./data/{fname}_{thread_count}.txt')