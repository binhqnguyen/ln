#!/bin/bash
E_NAME=CUBIC_20

echo "running lte nsc UDP ..."
./waf --run "scratch/lte_nsc --simTime=100 --isTcp=0" > /var/tmp/ln_result/radio/UDP_LOG 2>&1 

echo "running lte nsc TCP ..."
./waf --run "scratch/lte_nsc --simTime=100 --isTcp=1" > /var/tmp/ln_result/radio/TCP_LOG 2>&1 

mkdir /var/tmp/ln_result/backup/radio/$E_NAME
cp -rf /var/tmp/ln_result/radio/ /var/tmp/ln_result/backup/radio/$E_NAME 



