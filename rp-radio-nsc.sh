#!/bin/bash
E_NAME='CUBIC_noname'
PY_DIR='/var/tmp/ln_result/radio/'
RE_DIR='/var/tmp/ln_result'

echo "removing old files....."
rm $RE_DIR/radio/*.dat
rm $RE_DIR/radio/*.tmp
rm $RE_DIR/radio/*.raw
rm $RE_DIR/radio/TCP_LOG
rm $RE_DIR/radio/UDP_LOG
rm $RE_DIR/radio/*.txt
rm $RE_DIR/radio/*.svg

rm ~/ln/DlRxPhyStats.txt
rm ~/ln/DlTxPhyStats.txt
rm ~/ln/UlRxPhyStats.txt
rm ~/ln/DlTxPhyStats.txt

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
./waf --run "scratch/lte_nsc --simTime=100 --isTcp=0" > /var/tmp/ln_result/radio/UDP_LOG 2>&1 

echo "running lte nsc TCP ..."
./waf --run "scratch/lte_nsc --simTime=100 --isTcp=1" > /var/tmp/ln_result/radio/TCP_LOG 2>&1 

cp ~/ln/DlRxPhyStats.txt $RE_DIR/radio/
cp ~/ln/UlRxPhyStats.txt $RE_DIR/radio/
cp ~/ln/DlTxPhyStats.txt $RE_DIR/radio/
cp ~/ln/UlTxPhyStats.txt $RE_DIR/radio/
cp ~/ln/lte-nsc-in.txt $RE_DIR/radio/
cp ~/ln/scratch/lte_nsc.cc $RE_DIR/radio
cp ~/ln/scratch/lte-nsc.out $RE_DIR/radio
mkdir $BACKUP_FOLDER
cp -rf $RE_DIR/radio/ $BACKUP_FOLDER 
#cp ~/ln/DlRxPhyStats.txt /var/tmp/ln_result/radio
cd $RE_DIR
./dr-radio-nsc.sh $E_NAME



