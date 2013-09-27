#!/bin/bash
E_NAME='CUBIC_noname'
SIM_TIME='100'
RE_DIR='/var/tmp/ln_result'
EXE_DIR='/home/binhn/ln_'
if [ "$1" != "" ];
	then
	E_NAME=$1
	else
		echo "usage rp-...sh <name of backup folder>"
		exit;
fi
echo $E_NAME 
echo $RE_DIR

cd $RE_DIR/emulated/
rm *.svg
rm *.txt
rm *.dat
rm *_LOG
rm queue

BACKUP_FOLDER="$RE_DIR/backup/from_cade/$E_NAME"


cd $EXE_DIR
echo "running emulated nsc UDP ..."
./waf --run "scratch/emulated_nsc --sim_time=$SIM_TIME --is_tcp=0" > $RE_DIR/emulated/UDP_LOG 2>&1 

echo "running emulated nsc TCP ..."
./waf --run "scratch/emulated_nsc --sim_time=$SIM_TIME --is_tcp=1" > $RE_DIR/emulated/TCP_LOG 2>&1 

cd $RE_DIR
./dr-emulated-nsc.sh $E_NAME

mkdir $BACKUP_FOLDER
cp -r $RE_DIR/emulated/ $BACKUP_FOLDER
cp  $EXE_DIR/scratch/emulated_nsc.cc $BACKUP_FOLDER
rm $EXE_DIR/*.pcap

