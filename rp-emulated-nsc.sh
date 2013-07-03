#!/bin/bash
E_NAME='CUBIC_20'


echo "running emulated nsc UDP ..."
./waf --run "scratch/emulated_nsc --sim_time=100 --is_tcp=0" > /var/tmp/ln_result/emulated/UDP_LOG 2>&1 

echo "running emulated nsc TCP ..."
./waf --run "scratch/emulated_nsc --sim_time=100 --is_tcp=1" > /var/tmp/ln_result/emulated/TCP_LOG 2>&1 

mkdir /var/tmp/ln_result/backup/emulated/$E_NAME
cp -rf /var/tmp/ln_result/emulated/ /var/tmp/ln_result/backup/emulated/$E_NAME 

cd /var/tmp/ln_result/
./dr-emulated-nsc.sh $E_NAME


