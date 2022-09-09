import re

def parse(filename):
	with open(filename,'r') as f:
		lines = f.readlines()
		func_calls = lines[0].strip().split()[0]
		tot_time = lines[0].strip().split()[4]

		ncalls = []
		tottime = []
		percall = []
		cumtime = []
		percall = []
		func_name = []

		for i in range(5, len(lines)):
			line = lines[i]
			line = line.strip().split()
			if len(line)==0:
				continue
			ncalls.append(line[0])
			tottime.append(line[1])
			percall.append(line[2])
			cumtime.append(line[3])
			percall.append(line[4])
			s = ' '.join(line[5:])
			if '(' in s:
				res = re.findall(r'\(.*?\)', s)[0]
				func_name.append(res[1:-1])
			elif '{' in s:
				res = re.findall(r'\{.*?\}', s)[0]
				func_name.append(res[1:-1])
			else:
				func_name.append(s.strip())
		
		
		return (func_calls, tot_time, [ncalls, tottime, percall, cumtime, func_name])