#!/usr/bin/python3

import sys

with open(sys.argv[1], "rb") as drive:
	charat = 0
	letter = drive.read(1)
	while letter != '':
		if letter != b'\x00':
			print("Found ", str(letter), " at ", str(charat))
		charat += 1
		letter = drive.read(1)
