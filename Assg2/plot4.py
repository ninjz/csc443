#!/usr/bin/pyhton
from subprocess import Popen, PIPE, STDOUT
import os
import numpy as np
import matplotlib.pyplot as plt

xPoints = []
yPoints = []

xPoints2 = []
yPoints2 = []

xPoints3 = []
yPoints3 = []

ans = 0.0	
for j in range(10):
	proc = Popen(['./msort','file100','out_file100','1024', '2'],stdout=PIPE,stderr=STDOUT)
	line = proc.stdout.readline()
	#print line
	ans = float(line.split(' ')[1])		
	xPoints.append(i)
	yPoints.append(ans)


ans = 0.0
for j in range(10):
	proc = Popen(['./msort','file100', 'out_file100','1024', '1024'],stdout=PIPE,stderr=STDOUT)
	line = proc.stdout.readline()
	#print line
	ans = float(line.split(' ')[1])		
	xPoints2.append(i)
	yPoints2.append(ans)

ans = 0.0

for j in range(10):
	proc = Popen(['./bsort','file100', 'out_file100'],stdout=PIPE,stderr=STDOUT)
	line = proc.stdout.readline()
	#print line
	ans = float(line.split(' ')[1])		
	xPoints3.append(i)
	yPoints3.append(ans)


plt.plot(xPoints, yPoints)
plt.title("trial number vs time")
plt.xlabel("trial number")
plt.ylabel("time in milliseconds")
plt.savefig('msortGood.png')

plt.clf()

plt.plot(xPoints2, yPoints2)
plt.title("trial number vs time")
plt.xlabel("trial number")
plt.ylabel("time in milliseconds")
plt.savefig('msortBad.png')

plt.clf()

plt.plot(xPoints3, yPoints3)
plt.title("trial number vs time")
plt.xlabel("trial number")
plt.ylabel("time in milliseconds")
plt.savefig('bsort.png')

