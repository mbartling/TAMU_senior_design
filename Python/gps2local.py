from numpy import *

p1 = [30.625621, -96.336753]
p2 = [30.624388, -96.335755]
p3 = [30.626050, -96.333368]
p4 = [30.627195, -96.334945]
xres = 10
yres = 15

xlin = linspace(p1[0],p2[0],xres)
ylin = linspace(p1[1],p2[1],yres)

s1 = [30.625383, -96.336161]
s2 = [30.624978, -96.335295]
s3 = [30.625749, -96.334460]

rssi1 = -16.2342
rssi2 = -20.2342
rssi3 = -22.2342
thresh = 0.15 #15% of minimum distance

gamemap = zeros([len(xlin),len(ylin)])

dx = xlin - s1[0]
dy = ylin - s1[1]
xloc = abs(dx) <= (1+thresh)*min(abs(dx)) #consider doing average
yloc = abs(dy) <= (1+thresh)*min(abs(dy))
gamemap[xloc,yloc] = rssi1

dx = xlin - s2[0]
dy = ylin - s2[1]
xloc = abs(dx) <= (1+thresh)*min(abs(dx)) #consider doing average
yloc = abs(dy) <= (1+thresh)*min(abs(dy))
gamemap[xloc,yloc] = rssi2

dx = xlin - s3[0]
dy = ylin - s3[1]
xloc = abs(dx) <= (1+thresh)*min(abs(dx)) #consider doing average
yloc = abs(dy) <= (1+thresh)*min(abs(dy))
gamemap[xloc,yloc] = rssi3

temp = zeros([len(xlin),len(ylin)])
mask = gamemap != 0
temp[mask] = 

