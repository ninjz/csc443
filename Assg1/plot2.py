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
	Popen(['./csv2heapfile','toto.csv','hpfile',str(i)],stdout=PIPE,stderr=STDOUT)


	for j in range(10):
		proc = Popen(['./query','hpfile',str(0),str(100000000000),str(i)],stdout=PIPE,stderr=STDOUT)
		line = proc.stdout.readline()
		line = proc.stdout.readline()
		line = proc.stdout.readline()
		print line
		print i
		ans += float(line.split(' ')[1])		
			
	ans /= 10
	#print ans
	i +=1024
	xPoints2.append(i)
	yPoints2.append(ans)



ans = 0.0	
Popen(['./csv2heapfile','toto.csv','hpfile',str(8129)],stdout=PIPE,stderr=STDOUT)

i=0
for j in range(10):
	proc = Popen(['./query','hpfile',str(0),str(100000000000),str(8129)],stdout=PIPE,stderr=STDOUT)
	line = proc.stdout.readline()
	line = proc.stdout.readline()
	line = proc.stdout.readline()
	print line
	print i
	ans = float(line.split(' ')[1])
	xPoints.append(i)
	yPoints.append(ans)
	i+=1


plt.plot(xPoints2, yPoints2)
plt.title("query vs Pagesize")
plt.xlabel("Pagesize")
plt.ylabel("time in milliseconds")
plt.savefig('query_vs_pagesize.png')


plt.clf()



plt.plot(xPoints, yPoints)
plt.title("query vs pagesize 8219(psql pages size)")
plt.xlabel("trial number")
plt.ylabel("time in milliseconds")
plt.savefig("query_vs_8219.png")


