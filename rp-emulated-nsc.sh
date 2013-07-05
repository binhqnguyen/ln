#!/bin/bash
E_NAME='CUBIC_noname'
RE_DIR='/var/tmp/ln_result/'
if [ "$1" != "" && "$2" != "" ];
	then
	E_NAME=$1
	else
		echo "usage rp-...sh <name of backup folder>"
		exit;
fi
echo $E_NAME 
echo $RE_DIR

BACKUP_FOLDER="$RE_DIR\backup/from_cade/$E_NAME"

echo "running emulated nsc UDP ..."
./waf --run "scratch/emulated_nsc --sim_time=100 --is_tcp=0" > /var/tmp/ln_result/emulated/UDP_LOG 2>&1 

echo "running emulated nsc TCP ..."
./waf --run "scratch/emulated_nsc --sim_time=100 --is_tcp=1" > /var/tmp/ln_result/emulated/TCP_LOG 2>&1 

mkdir $BACKUP_FOLDER
cp -rf $RE_DIR\emulated/ $BACKUP_FOLDER

cd $RE_DIR
./dr-emulated-nsc.sh $E_NAME


