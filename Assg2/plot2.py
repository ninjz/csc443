#!/usr/bin/pyhton
from subprocess import Popen, PIPE, STDOUT
import os
import numpy as np
import matplotlib.pyplot as plt

xPoints = []
yPoints = []

ans = 0.0


for j in range(10):
	proc = Popen(['./msort','file_in100','file_out100',str((j+1)*1024), '2'],stdout=PIPE,stderr=STDOUT)
	line = proc.stdout.readline()
	print line
	ans = float(line.split(' ')[1])		
	xPoints.append((j+1)*1024)
	yPoints.append(ans)



plt.plot(xPoints, yPoints)
plt.title("memory_size vs time")
plt.xlabel("memory_size")
plt.ylabel("time in milliseconds")
plt.savefig('memory_sizeVStime.png')


