#!/bin/bash

cd /Users/binh/Desktop/ns3_play/result/tcp-link/pedestrian/distance-mobile/const_vel/2ue
pwd
rm *.svg
cat tcp-put-ul-send-100-3-200m.txt | grep 7.0.0.2 > ue1/tcp-put-send-ue1-100-3-200m.txt
cat tcp-put-ul-send-100-3-300m.txt | grep 7.0.0.2 > ue1/tcp-put-send-ue1-100-3-300m.txt
cat tcp-put-ul-send-100-3-400m.txt | grep 7.0.0.2 > ue1/tcp-put-send-ue1-100-3-400m.txt
cat tcp-put-ul-send-100-3-500m.txt | grep 7.0.0.2 > ue1/tcp-put-send-ue1-100-3-500m.txt
cat tcp-put-ul-send-100-3-600m.txt | grep 7.0.0.2 > ue1/tcp-put-send-ue1-100-3-600m.txt
cat tcp-put-ul-send-100-3-700m.txt | grep 7.0.0.2 > ue1/tcp-put-send-ue1-100-3-700m.txt
cat tcp-put-ul-send-100-3-800m.txt | grep 7.0.0.2 > ue1/tcp-put-send-ue1-100-3-800m.txt
cat tcp-put-ul-send-100-3-900m.txt | grep 7.0.0.2 > ue1/tcp-put-send-ue1-100-3-900m.txt
cat tcp-put-ul-send-100-3-1000m.txt | grep 7.0.0.2 > ue1/tcp-put-send-ue1-100-3-1000m.txt
cat tcp-put-ul-send-100-3-1100m.txt | grep 7.0.0.2 > ue1/tcp-put-send-ue1-100-3-1100m.txt
cat tcp-put-ul-send-100-3-1200m.txt | grep 7.0.0.2 > ue1/tcp-put-send-ue1-100-3-1200m.txt
cat tcp-put-ul-send-100-3-1300m.txt | grep 7.0.0.2 > ue1/tcp-put-send-ue1-100-3-1300m.txt


cat tcp-put-ul-send-100-3-200m.txt | grep 7.0.0.3 > ue2/tcp-put-send-ue2-100-3-200m.txt
cat tcp-put-ul-send-100-3-300m.txt | grep 7.0.0.3 > ue2/tcp-put-send-ue2-100-3-300m.txt
cat tcp-put-ul-send-100-3-400m.txt | grep 7.0.0.3 > ue2/tcp-put-send-ue2-100-3-400m.txt
cat tcp-put-ul-send-100-3-500m.txt | grep 7.0.0.3 > ue2/tcp-put-send-ue2-100-3-500m.txt
cat tcp-put-ul-send-100-3-600m.txt | grep 7.0.0.3 > ue2/tcp-put-send-ue2-100-3-600m.txt
cat tcp-put-ul-send-100-3-700m.txt | grep 7.0.0.3 > ue2/tcp-put-send-ue2-100-3-700m.txt
cat tcp-put-ul-send-100-3-800m.txt | grep 7.0.0.3 > ue2/tcp-put-send-ue2-100-3-800m.txt
cat tcp-put-ul-send-100-3-900m.txt | grep 7.0.0.3 > ue2/tcp-put-send-ue2-100-3-900m.txt
cat tcp-put-ul-send-100-3-1000m.txt | grep 7.0.0.3 > ue2/tcp-put-send-ue2-100-3-1000m.txt
cat tcp-put-ul-send-100-3-1100m.txt | grep 7.0.0.3 > ue2/tcp-put-send-ue2-100-3-1100m.txt
cat tcp-put-ul-send-100-3-1200m.txt | grep 7.0.0.3 > ue2/tcp-put-send-ue2-100-3-1200m.txt
cat tcp-put-ul-send-100-3-1300m.txt | grep 7.0.0.3 > ue2/tcp-put-send-ue2-100-3-1300m.txt


gnuplot plot-3kmph.gnu.txt



