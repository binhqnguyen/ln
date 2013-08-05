#!/bin/bash
E_NAME="CUBIC_noname"
PY_DIR="/var/tmp/ln_result/radio/"
RE_DIR="/var/tmp/ln_result"
SIM_TIME="50"
IS_PEDESTRIAN="1"
IS_FADING="1"

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
rm /home/binhn/ln/lte-nsc.out

if [ "$1" != "" ];
	then
	E_NAME=$1
	else
		echo "usage rp-..sh <name of backup folder>"
		exit;
fi
echo $E_NAME 
echo $PY_DIR
echo $RE_DIR

BACKUP_FOLDER="$RE_DIR/from_cade/$E_NAME"
echo "backup folder: $BACKUP_FOLDER"

echo "running lte nsc UDP ..."
./waf --run "scratch/lte_nsc_full_ues --simTime=$SIM_TIME --isTcp=0 --isPedestrian=$IS_PEDESTRIAN --isFading=$IS_FADING" > /var/tmp/ln_result/radio/UDP_LOG 2>&1 

echo "running lte nsc TCP ..."
./waf --run "scratch/lte_nsc_full_ues --simTime=$SIM_TIME --isTcp=1 --isPedestrian=$IS_PEDESTRIAN --isFading=$IS_FADING" > /var/tmp/ln_result/radio/TCP_LOG 2>&1 

cp ~/ln/lte_nsc_full_ues.out $RE_DIR/radio/
cp ~/ln/*.pcap $RE_DIR/radio/
cp ~/ln/{debugger.dat,course_change.dat} $RE_DIR/radio/
cp ~/ln/DlRxPhyStats.txt $RE_DIR/radio/
cp ~/ln/UlRxPhyStats.txt $RE_DIR/radio/
cp ~/ln/DlTxPhyStats.txt $RE_DIR/radio/
cp ~/ln/UlTxPhyStats.txt $RE_DIR/radio/
cp ~/ln/lte-nsc-full-in.txt $RE_DIR/radio/
cp ~/ln/scratch/lte_nsc.cc $RE_DIR/radio
cp ~/ln/scratch/lte-nsc.out $RE_DIR/radio
rm ~/ln/*.pcap

cd $RE_DIR
./dr-radio-nsc-cade.sh $E_NAME

mkdir $BACKUP_FOLDER
cp -rf $RE_DIR/radio/ $BACKUP_FOLDER 
#cp ~/ln/DlRxPhyStats.txt /var/tmp/ln_result/radio


