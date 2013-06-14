#!/bin/bash

cd /Users/binh/Desktop/ns3_play/result/tcp-link/pedestrian/distance-mobile/const_vel
pwd
rm *.svg

cat < tcp-100-3-ul-700m | grep ID1 > tcp-100-3-ul-700m-buffer.txt
cat < udp-100-3-ul-700m | grep ID1 > udp-100-3-ul-700m-buffer.txt
cat < tcp-100-3-ul-700m-nofading | grep ID1 > tcp-100-3-ul-700m-buffer-nofading.txt
cat < udp-100-3-ul-700m-nofading | grep ID1 > udp-100-3-ul-700m-buffer-nofading.txt
cat < tcp-100-3-ul-700m-nofading | grep 7.0.0.2 > tcp-put-ul-send-100-3-700m-nofading.txt
cat < udp-100-3-ul-700m-nofading | grep 7.0.0.2 > udp-put-ul-send-100-3-700m-nofading.txt




gnuplot plot-buffer.txt
gnuplot plot-aggregate.txt
gnuplot plot-small-interval.txt

gnuplot plot-ul-200m.gnu.txt
gnuplot plot-ul-300m.gnu.txt
gnuplot plot-ul-400m.gnu.txt
gnuplot plot-ul-500m.gnu.txt
gnuplot plot-ul-600m.gnu.txt
gnuplot plot-ul-700m.gnu.txt
gnuplot plot-ul-800m.gnu.txt
gnuplot plot-ul-900m.gnu.txt
gnuplot plot-ul-1000m.gnu.txt
gnuplot plot-ul-1100m.gnu.txt
gnuplot plot-ul-1200m.gnu.txt
gnuplot plot-ul-1300m.gnu.txt

gnuplot plot-3kmph.gnu.txt
gnuplot plot-2ue.gnu.txt


