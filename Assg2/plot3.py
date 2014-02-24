#!/usr/bin/pyhton
from subprocess import Popen, PIPE, STDOUT
import os
import numpy as np
import matplotlib.pyplot as plt

xPoints = []
yPoints = []

ans = 0.0


for j in range(10):
	proc = Popen(['./msort','file_in100','file_out100','1024', str(j*10)],stdout=PIPE,stderr=STDOUT)
	line = proc.stdout.readline()
	#print line
	ans = float(line.split(' ')[1])		
	xPoints.append(100)
	yPoints.append(ans)



plt.plot(xPoints, yPoints)
plt.title("number_of_buffers vs time")
plt.xlabel("number_of_buffers")
plt.ylabel("time in milliseconds")
plt.savefig('number_of_buffersVStime.png')


