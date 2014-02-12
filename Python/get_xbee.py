#! /usr/bin/env python
import serial
import sys

xbee = serial.Serial()
xbee.baudrate = 115200

if len(sys.argv) > 1:
	xbee.port = sys.argv[1]
else:
	xbee.port = '/dev/ttyACM0'

if xbee.isOpen():
	xbee.clos()

xbee.open()
print xbee

xbee.write("?")

if xbee.isOpen:
	for line in xbee:
		line = line.strip()
		packet = line.split()
		if len(packet) > 1:

			sf = packet[0]
			#address = packet[4:12]
			#addressH = int(packet[8:12], 16) #convert to int
			addressH = packet[8:12] #convert to int
			rssi = packet[12]
			#payload = packet[13:]
			if sf == '7E':
				print addressH , " RSSI = ", rssi

print "Closing Xbee Port"

xbee.close()
