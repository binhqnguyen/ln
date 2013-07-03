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


echo "running lte nsc UDP ..."
./waf --run "scratch/lte_nsc --simTime=100 --isTcp=0" > /var/tmp/ln_result/radio/UDP_LOG 2>&1 

echo "running lte nsc TCP ..."
./waf --run "scratch/lte_nsc --simTime=100 --isTcp=1" > /var/tmp/ln_result/radio/TCP_LOG 2>&1 

mkdir /var/tmp/ln_result/backup/radio/$E_NAME
cp -rf /var/tmp/ln_result/radio/ /var/tmp/ln_result/backup/radio/$E_NAME 

cd $RE_DIR
./dr-radio-nsc.sh $E_NAME $PY_DIR



