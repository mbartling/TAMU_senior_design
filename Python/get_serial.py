#! /usr/bin/env python

import serial

xbee = serial.Serial('/dev/ttyACM0', 115200)

if serial.isOpen():
    for line in xbee:
        print line

xbee.close
