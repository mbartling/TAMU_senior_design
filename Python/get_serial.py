#! /usr/bin/env python

import serial
import sys
from zig_packet import myparse

#xbee = serial.Serial('/dev/ttyACM0', 115200)
xbee  = serial.Serial()
xbee.baudrate = 115200
xbee.port = '/dev/ttyACM0'

if xbee.isOpen():
    xbee.close()
xbee.open()
print xbee
xbee.write("?")
if xbee.isOpen():
    for line in xbee:    
        line = line.strip()
        packet = line.split()
        sf = packet[0]
        lenH = packet[1]
        lenL = packet[2]
        apiFid = packet[3]
        address = packet[4:12]
        rssi = packet[12]
        payload = packet[13:]
        if sf == '7E':
            print address , "rssi = ", rssi
        
        #print line
        #rssi = myparse(line)
        #print address , rssi
        #print line
        #if(sys.stdin.read() == 'x'):
        #    break

print "Closing Xbee"
xbee.close()
