#!/bin/bash
if [ "$1" == "" ]; then
	echo "Usage ./rp-emulated.sh <sim_time> <backup folder name, no backing-up if empty>"
	exit
fi

BACKUP_FOLDER="/Users/binh/Documents/ln_result/from_cade/$2"

RESULT="/Users/binh/Documents/ln/results/tcp/data-scripts/emulated"
echo "***removing old *.txt files in the result directory...."
rm /Users/binh/Documents/ln/results/tcp/data-scripts/emulated/*.txt

echo "***running emulated UDP ..."
./waf --run "scratch/emulated --sim_time=$1 --is_tcp=0" > $RESULT/UDP_LOG 2>&1 


echo "***running emulated TCP ..."
./waf --run "scratch/emulated --sim_time=$1 --is_tcp=1" > $RESULT/TCP_LOG 2>&1 

echo "***making the backingup folder $BACKUP_FOLDER ..."
mkdir $BACKUP_FOLDER

echo "***plotting ..."
cd /Users/binh/Documents/ln/results/tcp/
pwd
./dr-emulated.sh

if [ "$2" == "" ]; then 
	echo "copying results to $BACKUP_FOLDER ..."
	cp $RESULT/ $BACKUP_FOLDER
	cp /Users/binh/Documents/ln/scratch/emulated.cc $BACKUP_FOLDER
	cp /Users/binh/Documents/ln/emulated-in.txt $BACKUP_FOLDER
	cp /Users/binh/Documents/ln/emulated-out.txt $BACKUP_FOLDER
fi



