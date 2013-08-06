#!/bin/bash
E_NAME="CUBIC_noname"
PY_DIR="/var/tmp/ln_result/radio/"
RE_DIR="/var/tmp/ln_result"
SIM_TIME="100"
DISTANCE="100"
PEDESTRIAN="1"

echo "removing old files....."
rm $RE_DIR/radio/*.dat
rm $RE_DIR/radio/*.tmp
rm $RE_DIR/radio/*.raw
rm $RE_DIR/radio/TCP_LOG
rm $RE_DIR/radio/UDP_LOG
rm $RE_DIR/radio/*.txt
rm $RE_DIR/radio/*.svg

rm /home/binhn/ln/*.pcap
rm /home/binhn/ln/DlRxPhyStats.txt
rm /home/binhn/ln/DlTxPhyStats.txt
rm /home/binhn/ln/UlRxPhyStats.txt
rm /home/binhn/ln/DlTxPhyStats.txt
rm /home/binhn/ln/lte-nsc-full.out

if [ "$2" != "" ];
	then
		SIM_TIME=$2
		DISTANCE=$3
		PEDESTRIAN=$4
		echo "t=$SIM_TIME d=$DISTANCE p=$PEDESTRIAN"
fi

if [ "$1" != "" ];
	then
	E_NAME=$1
	else
		echo "usage rp-..sh <name of backup folder> [<simTime> <distance> <isPedestrian>]"
		exit;
fi
echo $E_NAME 
echo $PY_DIR
echo $RE_DIR

BACKUP_FOLDER="$RE_DIR/from_cade/$E_NAME"
echo "backup folder: $BACKUP_FOLDER"

echo "running lte nsc UDP ..."
./waf --run "scratch/lte_nsc_full --simTime=$SIM_TIME --isTcp=0 --distance=$DISTANCE --isFading=1 --isPedestrian=$PEDESTRIAN" > /var/tmp/ln_result/radio/UDP_LOG 2>&1 

echo "running lte nsc TCP ..."
./waf --run "scratch/lte_nsc_full --simTime=$SIM_TIME --isTcp=1 --distance=$DISTANCE --isFading=1 --isPedestrian=$PEDESTRIAN" > /var/tmp/ln_result/radio/TCP_LOG 2>&1 

cp ~/ln/lte-nsc.out $RE_DIR/radio/
cp ~/ln/*.pcap $RE_DIR/radio/
cp ~/ln/DlRxPhyStats.txt $RE_DIR/radio/
cp ~/ln/UlRxPhyStats.txt $RE_DIR/radio/
cp ~/ln/DlTxPhyStats.txt $RE_DIR/radio/
cp ~/ln/UlTxPhyStats.txt $RE_DIR/radio/
cp ~/ln/*.txt $RE_DIR/radio/
cp ~/ln/lte-nsc-in.txt $RE_DIR/radio/
cp ~/ln/scratch/lte_nsc_full.cc $RE_DIR/radio
cp ~/ln/scratch/lte-nsc.out $RE_DIR/radio
rm ~/ln/*.pcap
mkdir $BACKUP_FOLDER
cp -rf $RE_DIR/radio/ $BACKUP_FOLDER 
#cp ~/ln/DlRxPhyStats.txt /var/tmp/ln_result/radio
cd $RE_DIR
./dr-radio-nsc-cade.sh $E_NAME



