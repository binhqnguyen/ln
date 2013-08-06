#!/bin/bash
cd ~/ln/
SIM_TIME=100
INC=100


for ((d=100; d<= 17000; d+=$INC))
do
	if [ $d -le 7000 ]; then
		INC=500
	fi
	if [ $d -gt 7000 -a $d -le 10000 ]; then
		INC=300
	fi
	if [ $d -gt 10000 -a $d -le 12000 ]; then
		INC=200
	fi
	if [ $d -gt 12000 -a $d -le 17000 ]; then
		INC=100
	fi
	echo "$d $INC"
	./rp-radio-nsc.sh PED_$d $SIM_TIME $d 1
	./rp-radio-nsc.sh VEH_$d $SIM_TIME $d 0
done





