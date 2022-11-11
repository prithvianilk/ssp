import sys
import os
import csv

vals = []

def parse(filename):
	with open(filename,'r') as f:
		N = int(filename.strip().split('_')[-1].strip().split('.')[0])
		lines = f.readlines()
		data = lines[7].strip().split()
		vals.append([N,float(data[0].replace(',','')),float(data[3])])

def main():
	dir = str(sys.argv[1])
	for filename in os.listdir('./'+dir):
		f = os.path.join(dir, filename)
		parse(f)
	vals.sort()
	vals.insert(0,["N","cache_misses","percentage"])
	
	with open(dir+'.csv', 'w') as csvfile: 
		csvwriter = csv.writer(csvfile)
		csvwriter.writerows(vals)

if __name__=="__main__":
    main()