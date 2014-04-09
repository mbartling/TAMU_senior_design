#! /usr/bin/env python
import serial
import sys
import os
import MySQLdb
from subprocess import call
from datetime import date
FORCE_WRITE = 0
today = date.today()
try:

	address_array = []

	# open data base
	db = MySQLdb.connect(host="localhost", user="", passwd="team05", db="xbee_teensy")
	cur = db.cursor()
	cur.execute("select version()")
	data = cur.fetchone()
	print "Database version: ", data
	cur.execute("truncate table raw_data")
	
	# open serial port
	xbee = serial.Serial()
	xbee.baudrate =  57600
	
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
			print line;
			if len(packet) > 1 and packet[0] == '7E':
				if len(packet) < 25  or  int(packet[11], 16) != 0x64:
					print "Packet len is: " + "{0}".format(len(packet))
					continue;
				# calling system command for timestamp
				p = os.popen('date "+%F %T"')
				timestamp = p.readline()
	    			p.close()
				timestamp = timestamp.rstrip('\n')
				timestamp = timestamp.rstrip('\0')
				print "Time is: " + timestamp
	
				# parse address
				addressH = packet[8:11]
				addressH.append(packet[14]) 
	
				# convert to dec, then string
				addressString = ''
				for item in addressH:
					x = int(item, 16)
					addressString += str(x) + '.'
				addressString = addressString[:-1]
				print "Initial Address: " + addressString

				if not addressString in address_array:
					print "Adding address string: " + addressString
					address_array.append(addressString)
	
				# parse rssi
				rssi = int(packet[15], 16)
				print "RSSI = ", rssi

				# parse survo position
				servoPos = int(packet[16], 16)
				print "servoPos =", servoPos
	
				# parse gps
				latArray = packet[17:21]
				latHex = ''.join(latArray)
				print latHex
				if latHex == '0000':
					lat = 0
				else:
					lat = int(latHex, 16)
	
				lonArray = packet [21:25]
				lonHex = ''.join(lonArray)
				print lonHex
				if lonHex == '0000':
					lon = 0;
				else:
					lon = int(lonHex, 16)
					lon = lon ^ 0xFFFFFFFF
					lon += 1
					lon *= -1

				print lat, lon
				if FORCE_WRITE:
					cmd = "insert into raw_data values(\"%s\",\"%s\", %d, %d, %d, %d)" %(timestamp, addressString, servoPos, rssi, lat, lon)
	    				print cmd
	    				cur.execute(cmd)
	    				db.commit()
	    				print "new row added to mysql"

				else:
					if lon > -970000000 and lon < -960000000 and lat > 306000000 and lat < 307000000:
						cmd = "insert into raw_data values(\"%s\",\"%s\", %d, %d, %d, %d)" %(timestamp, addressString, servoPos, rssi, lat, lon)
	    					print cmd
	    					cur.execute(cmd)
	    					db.commit()
	    					print "new row added to mysql"

	print "Closing Xbee Port"

finally:
	print "output data to file"
	os.popen('rm -f /home/walter/Code/rawData/*.txt')
	os.popen('rm -f /tmp/raw101.txt')
	for address in address_array:
		address_split = address.split('.');
		filename = '/tmp/raw' + address_split[3] + '.txt'
		os.popen('rm ' + filename)
		print filename
		cmd = "select row, col, rssi from raw_data where address = \'%s\' into outfile \'%s\' fields terminated by ','" %(address, filename)
		print cmd
		cur.execute(cmd)
		cmd = 'cp ' + filename + ' /home/walter/Code/rawData/raw' + address_split[3] + today.strftime("-%y-%m-%d") + '.out'
		print cmd
		os.popen(cmd)
	print "closing xbee port and database"
	db.close()
	xbee.close()
