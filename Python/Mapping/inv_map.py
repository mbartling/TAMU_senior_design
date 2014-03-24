#! /usr/bin/env python

import sys
import numpy as np

for line in sys.stdin:
	line = line.strip()
	(xCoords, yCoords, rssi) = line.split(',')
	print xCoords, yCoords, rssi
