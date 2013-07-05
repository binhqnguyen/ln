#!/bin/bash
E_NAME='CUBIC_noname'
PY_DIR='/var/tmp/ln_result/radio/'
RE_DIR='/var/tmp/ln_result/'
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
BACKUP_FOLDER="$RE_DIR\from_cade/$E_NAME"
echo "backup folder: $BACKUP_FOLDER"

echo "running lte nsc UDP ..."
./waf --run "scratch/lte_nsc --simTime=100 --isTcp=0 --distance=5000" > /var/tmp/ln_result/radio/UDP_LOG 2>&1 

echo "running lte nsc TCP ..."
./waf --run "scratch/lte_nsc --simTime=100 --isTcp=1 --distance=5000" > /var/tmp/ln_result/radio/TCP_LOG 2>&1 

mkdir $BACKUP_FOLDER
cp -rf $RE_DIR\radio/ $BACKUP_FOLDER 
cp ~/ln/DlRxPhyStats.txt $BACKUP_FOLDER
#cp ~/ln/DlRxPhyStats.txt /var/tmp/ln_result/radio
cd $RE_DIR
./dr-radio-nsc.sh $E_NAME



