#!/usr/bin/pyhton
from subprocess import Popen, PIPE, STDOUT
import os
import numpy as np
import matplotlib.pyplot as plt

xPoints = []
yPoints = []


ans = 0.0	
proc = Popen(['./msort','file_in100','file_out100','1024', '2'],stdout=PIPE,stderr=STDOUT)
line = proc.stdout.readline()
print line
ans = float(line.split(' ')[1])		
xPoints.append(100)
yPoints.append(ans)

ans = 0.0	
proc = Popen(['./msort','file_in1000','file_out1000','1024', '2'],stdout=PIPE,stderr=STDOUT)
line = proc.stdout.readline()
print line
ans = float(line.split(' ')[1])		
xPoints.append(1000)
yPoints.append(ans)

ans = 0.0	
proc = Popen(['./msort','file_in10000','file_out10000','1024', '2'],stdout=PIPE,stderr=STDOUT)
line = proc.stdout.readline()
print line
ans = float(line.split(' ')[1])		
xPoints.append(10000)
yPoints.append(ans)

ans = 0.0	
proc = Popen(['./msort','file_in100000','file_out100000','1024', '2'],stdout=PIPE,stderr=STDOUT)
line = proc.stdout.readline()
print line
ans = float(line.split(' ')[1])		
xPoints.append(100000)
yPoints.append(ans)

# ans = 0.0	
# proc = Popen(['./msort','file_in1000000','file_out1000000','1024', '2'],stdout=PIPE,stderr=STDOUT)
# line = proc.stdout.readline()
# print line
# ans = float(line.split(' ')[1])		
# xPoints.append(1000000)
# yPoints.append(ans)

# ans = 0.0	
# proc = Popen(['./msort','file_in10000000','file_out10000000','1024', '2'],stdout=PIPE,stderr=STDOUT)
# line = proc.stdout.readline()
# print line
# ans = float(line.split(' ')[1])		
# xPoints.append(10000000)
# yPoints.append(ans)



plt.plot(xPoints, yPoints)
plt.title("file_size vs time")
plt.xlabel("file_size")
plt.ylabel("time in milliseconds")
plt.savefig('file_sizeVStime.png')


