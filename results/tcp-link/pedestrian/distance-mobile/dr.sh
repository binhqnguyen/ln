#!/bin/bash

cd /Users/binh/Desktop/ns3_play/result/tcp-link/pedestrian/distance-mobile
pwd
rm *.svg

gnuplot plot-ul-200m.gnu.txt
gnuplot plot-ul-300m.gnu.txt
gnuplot plot-ul-400m.gnu.txt
gnuplot plot-ul-500m.gnu.txt
gnuplot plot-ul-600m.gnu.txt
gnuplot plot-ul-700m.gnu.txt
gnuplot plot-ul-800m.gnu.txt
gnuplot plot-ul-900m.gnu.txt
gnuplot plot-ul-1000m.gnu.txt

gnuplot plot-3kmph.gnu.txt
gnuplot plot-2ue.gnu.txt


