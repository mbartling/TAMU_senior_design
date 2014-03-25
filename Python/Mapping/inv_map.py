#! /usr/bin/env python

import sys
import numpy
from numpy.linalg import inv

DEBUGMODE = 1
if DEBUGMODE == 1:
	from mpl_toolkits.mplot3d import Axes3D
	import matplotlib.pyplot as plt
	import matplotlib.cm as cm

# Parameters
xSize = 300
ySize = 500

# Lists to maintain
xCoords = []
yCoords = []
rssi = []
fptr = open('testpoints.out', 'r')

def pass1(i, j, xmean, ymean, coefmat):
	xs = i-xmean
	ys = j-ymean
	temp = numpy.dot(numpy.array([xs**2, ys**2, xs*ys, xs, ys, 1]), coefmat)
	#print temp
	return temp

def pass2(i, j, xmean, ymean, coefmat):
	xs = i-xmean
	ys = j-ymean
	func = numpy.array([xs**2, ys**2, 2*xs*ys])
	#print func.T
	#print coefmat
	temp = numpy.dot( func.T , coefmat)
	#print temp
	temp = numpy.exp(-temp)
	#print temp
	return temp

#for line in sys.stdin:
for line in fptr:
	line = line.strip()
	(xCoordIn, yCoordIn, rssiIn) = line.split(',')
	# print xCoordIn, yCoordIn, rssiIn
	xCoords.append(xCoordIn)
	yCoords.append(yCoordIn)
	rssi.append(rssiIn)

fptr.close()
xloc = numpy.asarray(xCoords, dtype='int32')
yloc = numpy.asarray(yCoords, dtype='int32')
rssi = numpy.asarray(rssi, dtype = 'int32')
# print xCoords
xmean = numpy.mean(xloc)
ymean = numpy.mean(yloc)

# print xmean, ymean

xs = xloc - xmean
ys = yloc - ymean

# print xs

# Form the A Matrix
amat = numpy.ones((len(xs),6))

amat[: , 0] = xs**2
amat[: , 1] = ys**2;
amat[: , 2] = xs*ys;
amat[: , 3] = xs;
amat[: , 4] = ys;
amat[: , 5] = 1;

#print amat

# Note in numpy we have to use dot(A,B) to do matrix multiplication of arrays
amatPI = numpy.dot(amat.T,amat)
coefmat = numpy.dot(numpy.dot(inv(amatPI),amat.T),rssi.T)

print coefmat
gamemap = numpy.zeros((xSize, ySize))

for i in range(xSize):
	for j in range(ySize):
		gamemap[i,j] = pass1(i, j, xmean, ymean, coefmat)

gamemap = -gamemap/numpy.sum(numpy.sum(numpy.abs(gamemap)))

# Display Gamemap
if DEBUGMODE == 1:
	plt.figure()
	CS = plt.contourf(gamemap)
	#plt.show()

(xe,ye) = numpy.nonzero(gamemap == numpy.max(numpy.max(gamemap)))

print xe, ye

## Begin Pass 2

xs = xloc - xe
ys = yloc - ye

# print xs

# Form the A Matrix
amat = numpy.ones((len(xs),3))

amat[: , 0] = xs**2
amat[: , 1] = ys**2;
amat[: , 2] = 2*xs*ys;

#print amat

# Note in numpy we have to use dot(A,B) to do matrix multiplication of arrays
amatPI = numpy.dot(amat.T,amat)
coefmat = numpy.dot(numpy.dot(inv(amatPI),amat.T),rssi.T)

print coefmat
gamemap = numpy.zeros((xSize, ySize))

for i in range(xSize):
	for j in range(ySize):
		gamemap[i,j] = pass2(i, j, xe, ye, coefmat)


# Display Gamemap
if DEBUGMODE == 1:
	plt.figure()
	CS = plt.contourf(gamemap)
	#X,Y = numpy.mgrid[range(xSize), range(ySize)]
	#ax = fig.gca(projection='3d')
	#CS = ax.plot_wireframe(gamemap, rstride=1, cstride=1)
	plt.figure()
	im = plt.imshow(gamemap, interpolation='bilinear', cmap=cm.hot)
	plt.show()
