#!/usr/bin/pyhton
from subprocess import Popen, PIPE, STDOUT
import os
import numpy as np
import matplotlib.pyplot as plt

xPoints = []
yPoints = []

xPoints2 = []
yPoints2 = []

i=1024
while(i< 64000):
	ans = 0.0	
	for j in range(10):
		proc = Popen(['./write_fixed_len_pages','toto.csv','page_file',str(i)],stdout=PIPE,stderr=STDOUT)
		line = proc.stdout.readline()
		line = proc.stdout.readline()
		line = proc.stdout.readline()
		line = proc.stdout.readline()
		#print line
		ans += float(line.split(' ')[1])		
	
	ans /= 10
	xPoints.append(i)
	yPoints.append(ans)

	ans = 0.0

	for j in range(10):
		proc = Popen(['./read_fixed_len_pages','page_file',str(i)],stdout=PIPE,stderr=STDOUT)
		line = proc.stdout.readline()
		line = proc.stdout.readline()
		line = proc.stdout.readline()
		line = proc.stdout.readline()
		#print line
		ans += float(line.split(' ')[1])		
	
	ans /= 10
	#print ans
	i +=1024
	xPoints2.append(i)
	yPoints2.append(ans)


plt.plot(xPoints, yPoints)
plt.title("write fixed length pages vs Pagesize")
plt.xlabel("Pagesize")
plt.ylabel("time in milliseconds")
plt.savefig('write_fixed_len_pages.png')

plt.clf()

plt.plot(xPoints2, yPoints2)
plt.title("read fixed length pages vs Pagesize")
plt.xlabel("Pagesize")
plt.ylabel("time in milliseconds")
plt.savefig('read_fixed_len_pages.png')

