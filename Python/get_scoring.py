#! /usr/bin/env python

import serial
import sys
import os
from sets import Set

try:
	addr_rssi = {}

	good_ant_addresses = Set([101])
	bad_ant_addresses = Set([])

	ant_addresses = good_ant_addresses.union(bad_ant_addresses)

	for addr in ant_addresses:
		addr_rssi[addr] = []

	# opening comport
	xbee = serial.Serial()
	xbee.baudrate = 57600

	if len(sys.argv) > 1:
		xbee.port = sys.argv[1]
	else:
		xbee.port = '/dev/ttyACM0'

	if xbee.isOpen():
		xbee.close()

	xbee.open()
	print xbee

	xbee.write("?")

	if xbee.isOpen:
		for line in xbee:
			line = line.strip()
			packet = line.split()
			print line
			if len(packet) > 1 and packet[0] == '7E':
				if len(packet) < 18 or int(packet[11], 16) not in ant_addresses:
					continue

				gnd_address = int(packet[11], 16)
				rssi = packet[15]

				if gnd_address in ant_addresses:
					addr_rssi[gnd_address].append(int(rssi,16))
					print "appended 0x", rssi , "to array", gnd_address

	print "closing Xbee port"

finally:
	print "scoring"


	print "GOOD ANTENNA(S)"
	for addr in good_ant_addresses:
		total = sum(addr_rssi[addr])
		average = sum(addr_rssi[addr])/len(addr_rssi[addr])
		print "total rssi from" , addr , "is", total
		print "total number of packets received from", addr, "is", len(addr_rssi[addr])
		print "average rssi from", addr, "is", average

	print "BAD ANTENNA(S)"
	for addr in bad_ant_addresses:
		total = sum(addr_rssi[addr])
		average = sum(addr_rssi[addr])/len(addr_rssi[addr])
		print "total rssi from" , addr , "is", total
		print "total number of packets received from", addr, "is", len(addr_rssi[addr])
		print "average rssi from", addr, "is", average

	xbee.close()
			
