#!/bin/bash
if [ "$1" == "" ]; then
	echo "Usage ./rp-emulated.sh  <backup folder name>"
	exit
fi

SIM_TIME=1
CODE_DIR="/var/tmp/ln"
BACKUP_FOLDER="/var/tmp/ln_result/backup/from_cade/$1"
echo "$BACKUP_FOLDER"

RESULT="$CODE_DIR/results/emulated"
echo "***removing old *.txt files in the result directory...."
rm $RESULT/*.txt
rm $RESULT/*.dat
rm $RESULT/*.svg


echo "***running emulated UDP ..."
./waf --run "scratch/emulated --sim_time=$SIM_TIME --is_tcp=0" > $RESULT/UDP_LOG 2>&1 


echo "***running emulated TCP ..."
./waf --run "scratch/emulated --sim_time=$SIM_TIME --is_tcp=1" > $RESULT/TCP_LOG 2>&1 

echo "***making the backingup folder $BACKUP_FOLDER ..."
mkdir $BACKUP_FOLDER

echo "***plotting ..."
cd $CODE_DIR/results/
pwd
./dr-emulated.sh

echo "copying results to $BACKUP_FOLDER ..."
cp -r $RESULT/ $BACKUP_FOLDER
cp $CODE_DIR/scratch/emulated.cc $BACKUP_FOLDER
cp $CODE_DIR/emulated-in.txt $BACKUP_FOLDER
cp $CODE_DIR/emulated-out.txt $BACKUP_FOLDER



