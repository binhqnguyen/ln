#!/bin/bash
E_NAME="CUBIC_noname"
RE_DIR="/var/tmp/ln_result"
PY_DIR="$REDIR/radio/"
SIM_TIME="100"
DISTANCE="100"
PEDESTRIAN="1"
lte="lte"
processing="processing"

if [ "$1" != "" ];
	then
	E_NAME=$1
	else
		echo "usage rp-..sh <name of backup folder> [<simTime> <distance> <isPedestrian>]"
		exit;
fi

echo "removing old files....."
rm $RE_DIR/radio/*.dat
rm $RE_DIR/radio/*.tmp
rm $RE_DIR/radio/*.raw
rm $RE_DIR/radio/TCP_LOG
rm $RE_DIR/radio/UDP_LOG
rm $RE_DIR/radio/*.txt
rm $RE_DIR/radio/*.svg
rm $RE_DIR/radio/queue

rm /home/binhn/ln/*.pcap
rm /home/binhn/ln/DlRxPhyStats.txt
rm /home/binhn/ln/DlTxPhyStats.txt
rm /home/binhn/ln/UlRxPhyStats.txt
rm /home/binhn/ln/DlTxPhyStats.txt
rm /home/binhn/ln/lte.out

if [ "$2" != "" ];
	then
		SIM_TIME=$2
		DISTANCE=$3
		PEDESTRIAN=$4
		echo "t=$SIM_TIME d=$DISTANCE p=$PEDESTRIAN"
fi

echo $E_NAME 
echo $PY_DIR
echo $RE_DIR

BACKUP_FOLDER="$RE_DIR/from_cade/$E_NAME"
echo "backup folder: $BACKUP_FOLDER"

echo "running $lte nsc UDP ..."
./waf --run "scratch/$lte --simTime=$SIM_TIME --isTcp=0 --distance=$DISTANCE --isFading=1 --isPedestrian=$PEDESTRIAN" > /var/tmp/ln_result/radio/UDP_LOG 2>&1 

echo "running $lte nsc TCP ..."
./waf --run "scratch/$lte --simTime=$SIM_TIME --isTcp=1 --distance=$DISTANCE --isFading=1 --isPedestrian=$PEDESTRIAN" > /var/tmp/ln_result/radio/TCP_LOG 2>&1 

cp ~/ln/*.out $RE_DIR/radio/
cp ~/ln/*.pcap $RE_DIR/radio/
cp ~/ln/*.txt $RE_DIR/radio/
cp ~/ln/.dat $RE_DIR/radio/
cp ~/ln/lte.in $RE_DIR/radio/
cp ~/ln/scratch/$lte.cc $RE_DIR/radio
rm ~/ln/*.pcap
mkdir $BACKUP_FOLDER
cd $RE_DIR
./$processing.sh $E_NAME
cp -rf $RE_DIR/radio/ $BACKUP_FOLDER 



