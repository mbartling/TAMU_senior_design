#! /usr/bin/env python

import sys
import numpy
from numpy.linalg import inv

DEBUGMODE = 1
WINDOWS_MODE = 1
if DEBUGMODE == 1:
    #from mpl_toolkits.mplot3d import Axes3D
    import matplotlib.pyplot as plt
    import matplotlib.cm as cm
    from mpl_toolkits.mplot3d import Axes3D

# Parameters Make sure these values correspond with the input locations otherwise we will get weird results
xres = 300
yres = 500
weight = 0.5
# Lists to maintain
xCoords = []
yCoords = []
rssi = []

#Fast Max index finder
def max_ij(x):
    i, j = divmod(x.argmax(), x.shape[1])
    return i, j
    
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
i = 0
if WINDOWS_MODE == 1:
    fptr = open('raw101.out', 'r')
    for line in fptr:
      line = line.strip()
      (xCoordIn, yCoordIn, rssiIn) = line.split(',')
      # print xCoordIn, yCoordIn, rssiIn
      xCoords.append(xCoordIn)
      yCoords.append(yCoordIn)
      rssi.append(rssiIn)
    
    fptr.close()
#    if i == 0:
#        line = line.strip()
#        (xres,yres) = line.split(',')
#        xres = int(xres)
#        yres = int(yres)
#        i += 1
#    else:
else:
    for line in sys.stdin:
      line = line.strip()
      (xCoordIn, yCoordIn, rssiIn) = line.split(',')
      # print xCoordIn, yCoordIn, rssiIn
      xCoords.append(xCoordIn)
      yCoords.append(yCoordIn)
      rssi.append(rssiIn)

#fptr.close()
xloc = numpy.asarray(xCoords, dtype='int32')
yloc = numpy.asarray(yCoords, dtype='int32')
rssi = numpy.asarray(rssi, dtype = 'int32')

latmin = numpy.min(xloc)
latmax = numpy.max(xloc)
lonmin = numpy.min(yloc)
lonmax = numpy.max(yloc)

fig = plt.figure()
ax = fig.add_subplot(111,projection='3d')

ax.scatter(xloc,yloc,rssi)
#plt.show()
(latmin, latmax) = ax.get_xlim()
(lonmin, lonmax) = ax.get_ylim()

xlin = numpy.linspace(latmin,latmax,xres)
ylin = numpy.linspace(lonmin,lonmax,yres)
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

#print coefmat
gamemap = numpy.zeros((xres, yres))

for i in range(xres):
    for j in range(yres):
        gamemap[i,j] = pass1(xlin[i], ylin[j], xmean, ymean, coefmat)

#gamemap = -gamemap/numpy.sum(numpy.sum(numpy.abs(gamemap)))
gamemap = gamemap/numpy.max(numpy.max(numpy.abs(gamemap)))

# Display Gamemap
if DEBUGMODE == 1:
    plt.figure()
    CS = plt.contourf(gamemap, origin='image')
    plt.colorbar()

    #plt.figure()
    #im = plt.imshow(gamemap, interpolation='bicubic',origin='image', cmap=cm.jet)
    #plt.colorbar()
    #plt.show()

(xe,ye) = numpy.nonzero(gamemap == numpy.max(numpy.max(gamemap)))

#Save the original game map, we might be able to exploit it
gamemapFat = gamemap
# print xe, ye

## Begin Pass 2_

xs = xloc - xlin[xe]
ys = yloc - ylin[ye]

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

# print coefmat
gamemap = numpy.zeros((xres, yres))

# There must be a faster way to do this
for i in range(xres):
    for j in range(yres):
        gamemap[i,j] = pass2(xlin[i], ylin[j], xlin[xe], ylin[ye], coefmat)


# Display Gamemap
if DEBUGMODE == 1:
  #  plt.figure()
  #  CS = plt.contourf(gamemap, origin='image')
  #  plt.colorbar()
    #X,Y = numpy.mgrid[range(xres), range(yres)]
    #ax = fig.gca(projection='3d')
    #CS = ax.plot_wireframe(gamemap, rstride=1, cstride=1)
    plt.figure()
    im = plt.imshow(gamemap, interpolation='spline36',origin='image', cmap=cm.jet)
    plt.colorbar()
        
    #plt.figure()
    gamemapFat += numpy.max(numpy.max(abs(gamemapFat)))
    gamemapFat *= 1.0/numpy.max(numpy.max(numpy.abs(gamemapFat)))
    #im = plt.imshow(gamemapFat, interpolation='spline36',origin='image', cmap=cm.jet)
    #plt.colorbar()

    plt.figure()
    gamemap = (weight*gamemap+gamemapFat)/(weight+1)
    gamemap = gamemap/numpy.max(numpy.max(gamemap))
    im = plt.imshow(gamemap,interpolation='bicubic',origin='image', cmap=cm.jet)
    plt.colorbar()
    plt.show()

print xe, ye
numpy.savetxt("forJennyIns.out", gamemap, delimiter=" ")
numpy.savetxt("forJennyIns.csv", gamemap, delimiter=",")

# First entry corresponds to index 1, second entry corresponds to index 2 etc
numpy.savetxt("xSpace.out", xlin, delimiter = " ")
numpy.savetxt("ySpace.out", ylin, delimiter = " ")
#for i in range(xres):
#    for j in range(yres):
#        print gamemap[i,j]

numpy.savetxt("xSpace.out", xlin, delimiter=" ")
numpy.savetxt("yypace.out", ylin, delimiter=" ")
