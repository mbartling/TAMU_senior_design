#! /usr/bin/env python

# Mapping Algorithm by Michael Bartling

## Imports
import sys

class mapSample:
	"""A simple struct for holding the map data """
	def __init__(self):
		self.x = 0
		self.y = 0
		self.rssi = 0


def parse( line ):
	#do stuff here

	return sample

def main():
	# Here, we will guarantee that the input will be of this form
	for line in sys.stdin:
		line = line.strip()
		line = line.split()

		#un-pack the line
		address, polarization, xloc, yloc, rssi = line

		#parse(line)

main():

