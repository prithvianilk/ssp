import os


fnames = ['og_parallel_pi', 'parallel_pi']
for fname in fnames:
    os.system(f'g++ -std=c++20 -o {fname} {fname}.cc')

print(','.join(['#Threads'] + fnames))

for i in range(15):
    thread_count = 2 ** i
    data = [str(thread_count)]
    for fname in fnames:
        path = f'./data/{fname}_{thread_count}.txt'
        os.system(f'(time ./{fname} {thread_count}) &> {path}')
        with open(path) as f:
            data.append(f.readlines()[2].split()[1][2: -1])
    print(','.join(data))
