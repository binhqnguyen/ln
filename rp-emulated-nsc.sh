#!/bin/bash
E_NAME='CUBIC_noname'
PY_DIR='dir_of_python_scripts'
RE_DIR='/var/tmp/ln_result/'
if [ "$1" != "" && "$2" != "" ];
	then
	E_NAME=$1
	PY_DIR=$2
	else
		echo "usage rp-...sh <name of backup folder> <where are python scripts>"
		exit;
fi
echo $E_NAME 
echo $PY_DIR
echo $RE_DIR

echo "running emulated nsc UDP ..."
./waf --run "scratch/emulated_nsc --sim_time=100 --is_tcp=0" > /var/tmp/ln_result/emulated/UDP_LOG 2>&1 

echo "running emulated nsc TCP ..."
./waf --run "scratch/emulated_nsc --sim_time=100 --is_tcp=1" > /var/tmp/ln_result/emulated/TCP_LOG 2>&1 

mkdir $RE_DIR\backup/emulated/$E_NAME
cp -rf $RE_DIR\emulated/ $RE_DIR\backup/emulated/$E_NAME 

cd $RE_DIR
./dr-emulated-nsc.sh $E_NAME $PY_DIR


