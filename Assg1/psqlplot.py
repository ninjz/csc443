#!/usr/bin/pyhton
from subprocess import Popen, PIPE, STDOUT
import os
import numpy as np
import matplotlib.pyplot as plt

xPoints = []
yPoints = []

xPoints2 = []
yPoints2 = []

# xPoints3 = []
# yPoints3 = []


'''for psql WITHOUT indexing in table records in the database'''
i=0
ans =0.0
for j in range(10):
	proc = Popen(['sh','psqlscript.sh'],stdout=PIPE,stderr=STDOUT)
	prev=proc.stdout.readline()
	line=proc.stdout.readline()
	while (line):
		prev = line
		line = proc.stdout.readline()
	ans = float(prev.split(' ')[1])
	print ans
	yPoints.append(ans)
	xPoints.append(i)
	i+=1



#ans /= 10
print ans

#yPoints.append(ans)

'''for psql with indexing on attribute 1 in the table records1 in the database'''
i=0
ans=0.0
for j in range(10):
	proc = Popen(['sh','psqlscript1.sh'],stdout=PIPE,stderr=STDOUT)
	prev=proc.stdout.readline()
	line=proc.stdout.readline()
	while (line):
		prev = line
		line = proc.stdout.readline()
	ans = float(prev.split(' ')[1])
	print ans
	xPoints2.append(i)
	yPoints2.append(ans)	
	#print line
	i+=1		

#ans /= 10
#print ans

'''running the query on our heap on the same page size as psql 8129bytes'''

# ans = 0.0	
# Popen(['./csv2heapfile','toto.csv','hpfile',str(8129)],stdout=PIPE,stderr=STDOUT)

# i=0
# for j in range(10):
# 	proc = Popen(['./query','hpfile',str(0),str(100000000000),str(8129)],stdout=PIPE,stderr=STDOUT)
# 	line = proc.stdout.readline()
# 	line = proc.stdout.readline()
# 	line = proc.stdout.readline()
# 	print line
# 	print i
# 	ans = float(line.split(' ')[1])
# 	xPoints3.append(i)
# 	yPoints3.append(ans)
# 	i+=1


# plt.plot(xPoints, yPoints)
# plt.plot(xPoints2, yPoints2)
# # plt.plot(xPoints3, yPoints3)
# plt.title("psql vs our solution")
# plt.xlabel("trial number")
# plt.ylabel("time in millisecond")
# plt.legend(loc='lower left')
# plt.savefig('psqlvsOurcode.png')


plt.clf()

plt.plot(xPoints, yPoints)
plt.title("psql query execution without indexing size of page is 8219")
plt.xlabel("trial number")
plt.ylabel("time in millisecond")
plt.savefig('psqlrunningtime.png')

plt.clf()

plt.plot(xPoints2, yPoints2)
plt.title("psql query execution with indexing")
plt.xlabel("trial number")
plt.ylabel("time in milliseconds")
plt.savefig('psqlrunningtimeindexed.png')


plt.clf()


# plt.plot(xPoints3, yPoints3)
# plt.title("psql query execution with indexing")
# plt.xlabel("trial number")
# plt.ylabel("time in milliseconds")
# plt.savefig('queryOn8218PageSize.png')
