#!/bin/bash

cd /Users/binh/Documents/workspace/lena/results/tcp-link/pedestrian/const_vel/interval_avg
pwd
rm *.svg

cat < tcp-700m | grep ID1 > tcp-700m-queue
cat < udp-700m | grep ID1 > udp-700m-queue
cat < tcp-700m | grep 7.0.0.2 > tcp-700m-put
cat < udp-700m | grep 7.0.0.2 > udp-700m-put

cat < tcp-1000m | grep ID1 > tcp-1000m-queue
cat < udp-1000m | grep ID1 > udp-1000m-queue
cat < tcp-1000m | grep 7.0.0.2 > tcp-1000m-put
cat < udp-1000m | grep 7.0.0.2 > udp-1000m-put


gnuplot plot.txt
gnuplot plot-put.txt


