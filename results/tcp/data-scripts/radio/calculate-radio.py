#!/usr/bin/python

import os
import sys
import FileHandle

if __name__ == "__main__":

	##########calculate dl radio rx rate (from DlRxPhyStats.txt) #############
	INPUT_FILE =  "/Users/binh/Documents/workspace/lena/results/DlRxPhyStats.txt" 
	OUTPUT_FILE = "/Users/binh/Documents/workspace/lena/results/results/tcp/data-scripts/radio/radio-rx-rate.txt"

	file = open (INPUT_FILE)
	line = file.readline()
	tokens = {}

	if (os.path.isfile(OUTPUT_FILE)):      ##if output file not exist
	    open(OUTPUT_FILE,'w').close()
	outfile = open (OUTPUT_FILE,'w+')

	while (line):
		tokens = line.split()
		outfile.write(tokens[0][0:-1]+"\n")
		line = file.readline()