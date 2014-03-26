#! /usr/bin/env python
from numpy import *
import sys

#p1 = [30.625621, -96.336753]
#p2 = [30.624388, -96.335755]
#p3 = [30.626050, -96.333368]
#p4 = [30.627195, -96.334945]
xres = 300
yres = 500

lat = []
lon  = []

for line in sys.stdin:
	line.strip()
	(latS, lonS, dummy) = line.split(',')
	lat.append(latS)
	lon.append(lonS)

lat = array(lat, dtype='int32')
lon = array(lon, dtype='int32')
latmin = min(lat)
latmax = max(lat)
lonmin = min(lon)
lonmax = max(lon)

xlin = linspace(latmin,latmax,xres)
ylin = linspace(lonmin,lonmax,yres)

print xlin, ylin
#s1 = [30.625383, -96.336161]
#s2 = [30.624978, -96.335295]
#s3 = [30.625749, -96.334460]

#rssi1 = -16.2342
#rssi2 = -20.2342
#rssi3 = -22.2342
thresh = 0.15 #15% of minimum distance

#gamemap = zeros([len(xlin),len(ylin)])
#
#dx = xlin - s1[0]
#dy = ylin - s1[1]
#xloc = abs(dx) <= (1+thresh)*min(abs(dx)) #consider doing average
#yloc = abs(dy) <= (1+thresh)*min(abs(dy))
#gamemap[xloc,yloc] = rssi1
#
#dx = xlin - s2[0]
#dy = ylin - s2[1]
#xloc = abs(dx) <= (1+thresh)*min(abs(dx)) #consider doing average
#yloc = abs(dy) <= (1+thresh)*min(abs(dy))
#gamemap[xloc,yloc] = rssi2
#
#dx = xlin - s3[0]
#dy = ylin - s3[1]
#xloc = abs(dx) <= (1+thresh)*min(abs(dx)) #consider doing average
#yloc = abs(dy) <= (1+thresh)*min(abs(dy))
#gamemap[xloc,yloc] = rssi3
#
#temp = zeros([len(xlin),len(ylin)])
#mask = gamemap != 0
#temp[mask] = 
#
