#!/usr/bin/python

import os
import sys
import FileHandle

if __name__ == "__main__":

	##########Error received packets (from DlRxPhyStats.txt) #############
	INPUT_FILE =  "/Users/binh/Documents/workspace/lena/DlRxPhyStats.txt" 
	OUTPUT_FILE = "/Users/binh/Documents/workspace/lena/results/tcp/data-scripts/radio/radio_error_rx.dat"

	file = open (INPUT_FILE)
	line = file.readline()
	tokens = {}

	if (os.path.isfile(OUTPUT_FILE)):      ##if output file not exist
	    open(OUTPUT_FILE,'w').close()
	outfile = open (OUTPUT_FILE,'w+')


	while (line):
		tokens = line.split()
		if (tokens[10] == '0'):
			outfile.write(str ( (float (tokens[0]))/1000 )+"\n")
		line = file.readline()