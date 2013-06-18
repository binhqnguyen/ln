#!/usr/bin/python

import os
import sys
import FileHandle

if __name__ == "__main__":

	INPUT_FILE =  "/Users/binh/Documents/workspace/lena/results/tcp/data-scripts/emulated/endhost_dev_dequeue_tmp.txt" 
	OUTPUT_FILE = "/Users/binh/Documents/workspace/lena/results/tcp/data-scripts/emulated/endhost_dev_dequeue.txt"

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

	INPUT_FILE =  "/Users/binh/Documents/workspace/lena/results/tcp/data-scripts/emulated/enb_dev_enqueue_tmp.txt" 
	OUTPUT_FILE = "/Users/binh/Documents/workspace/lena/results/tcp/data-scripts/emulated/enb_dev_enqueue.txt"


	file = open (INPUT_FILE)
	line = file.readline()

	if (os.path.isfile(OUTPUT_FILE)):      ##if output file not exist
	    open(OUTPUT_FILE,'w').close()
	outfile = open (OUTPUT_FILE,'w+')

	while (line):
	    tokens = line.split()
	    outfile.write(tokens[0][:-1]+"\n")
	    line = file.readline()


	INPUT_FILE =  "/Users/binh/Documents/workspace/lena/results/tcp/data-scripts/emulated/enb_dev_dequeue_tmp.txt" 
	OUTPUT_FILE = "/Users/binh/Documents/workspace/lena/results/tcp/data-scripts/emulated/enb_dev_dequeue.txt"


	file = open (INPUT_FILE)
	line = file.readline()
	tokens = {}

	if (os.path.isfile(OUTPUT_FILE)):      ##if output file not exist
	    open(OUTPUT_FILE,'w').close()
	outfile = open (OUTPUT_FILE,'w+')

	while (line):
	    tokens = line.split()
	    outfile.write(tokens[0][:-1]+"\n")
	    line = file.readline()

######################################
#RttEstimator refining
#Adding unchanged points to the data files
######################################

	INPUT_FILE =  "/Users/binh/Documents/workspace/lena/results/tcp/data-scripts/emulated/last_rtt_sample_tmp.txt" 
	OUTPUT_FILE = "/Users/binh/Documents/workspace/lena/results/tcp/data-scripts/emulated/last_rtt_sample.txt"
	INTERVAL = 0.01 #10ms


	file = open (INPUT_FILE)
	line = file.readline()
	tokens = {}

	if (os.path.isfile(OUTPUT_FILE)):      ##if output file not exist
	    open(OUTPUT_FILE,'w').close()
	outfile = open (OUTPUT_FILE,'w+')

	last_sampling_time = 0
	last_value = "0"
	while (line):
		tokens = line.split()
		while (float (tokens[0]) > last_sampling_time ):
			if (last_sampling_time > 0.7):
		 		outfile.write(str (last_sampling_time)+"\t"+last_value+"\n")
		  	last_sampling_time += INTERVAL
	  	last_value = tokens[2]
	  	last_sampling_time = float (tokens[0])
	  	line = file.readline()
	while (last_sampling_time < 190):
		outfile.write(str (last_sampling_time) + "\t" +last_value+"\n")
		last_sampling_time += 1



	INPUT_FILE =  "/Users/binh/Documents/workspace/lena/results/tcp/data-scripts/emulated/rto_value_tmp.txt" 
	OUTPUT_FILE = "/Users/binh/Documents/workspace/lena/results/tcp/data-scripts/emulated/rto_value.txt"


	file = open (INPUT_FILE)
	line = file.readline()
	tokens = {}

	if (os.path.isfile(OUTPUT_FILE)):      ##if output file not exist
	    open(OUTPUT_FILE,'w').close()
	outfile = open (OUTPUT_FILE,'w+')

	last_sampling_time = 0
	last_value = "0"
	while (line):
		tokens = line.split()
		while (float (tokens[0]) > last_sampling_time ):
			if (last_sampling_time > 0.7):
		 		outfile.write(str (last_sampling_time)+"\t"+last_value+"\n")
	  		last_sampling_time += INTERVAL
	  	last_value = tokens[2]
	  	last_sampling_time = float (tokens[0])
	  	line = file.readline()
	while (last_sampling_time < 190):
		outfile.write(str (last_sampling_time) + "\t" +last_value+"\n")
		last_sampling_time += 1


	# INPUT_FILE =  "/Users/binh/Documents/workspace/lena/results/tcp/data-scripts/emulated/rtt_value_tmp.txt" 
	# OUTPUT_FILE = "/Users/binh/Documents/workspace/lena/results/tcp/data-scripts/emulated/rtt_value.txt"


	# file = open (INPUT_FILE)
	# line = file.readline()
	# tokens = {}

	# if (os.path.isfile(OUTPUT_FILE)):      ##if output file not exist
	#     open(OUTPUT_FILE,'w').close()
	# outfile = open (OUTPUT_FILE,'w+')

	# # last_sampling_time = 0
	# # last_value = "0"
	# while (line):
	# 	tokens = line.split()
	# 	# while (float (tokens[0]) > last_sampling_time ):
	# 		# if (last_sampling_time > 0.7):
	# 	outfile.write(tokens[0][:-1]+"\t"+tokens[5]+"\n")
	#   	# last_sampling_time += INTERVAL
	#   	# last_value = tokens[2]
	#   	# last_sampling_time = float (tokens[0])
	#   	line = file.readline()


	INPUT_FILE =  "/Users/binh/Documents/workspace/lena/results/tcp/data-scripts/emulated/enb_dev_queue_drop_tmp.txt" 
	OUTPUT_FILE = "/Users/binh/Documents/workspace/lena/results/tcp/data-scripts/emulated/enb_dev_queue_drop.txt"


	file = open (INPUT_FILE)
	line = file.readline()

	if (os.path.isfile(OUTPUT_FILE)):      ##if output file not exist
	    open(OUTPUT_FILE,'w').close()
	outfile = open (OUTPUT_FILE,'w+')

	while (line):
	    tokens = line.split()
	    outfile.write(tokens[0][:-1]+"\n")
	    line = file.readline()



	INPUT_FILE =  "/Users/binh/Documents/workspace/lena/results/tcp/data-scripts/emulated/next_tx_seq.txt"
	OUTPUT_FILE = "/Users/binh/Documents/workspace/lena/results/tcp/data-scripts/emulated/next_tx_pktseq.txt"


	file = open (INPUT_FILE)
	line = file.readline()

	if (os.path.isfile(OUTPUT_FILE)):      ##if output file not exist
	    open(OUTPUT_FILE,'w').close()
	outfile = open (OUTPUT_FILE,'w+')

	while (line):
	    tokens = line.split()
	    pktseq = (int (tokens[2])-1)/536
	    outfile.write(tokens[0] + "\t\t"+ str(pktseq)+ "\n")
	    line = file.readline()


