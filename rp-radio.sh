#!/bin/bash
echo "cleaning old DlRxPhyStats.txt and others ..."
rm ~/Documents/workspace/lena/DlRxPhyStats.txt ~/Documents/workspace/lena/DlTxPhyStats.txt  ~/Documents/workspace/lena/UlRxPhyStats.txt ~/Documents/workspace/lena/UlRxPhyStats.txt 
rm ~/Documents/workspace/lena/results/tcp/data-scripts/radio/*.tmp
rm ~/Documents/workspace/lena/results/tcp/data-scripts/radio/*.dat

echo "running lte UDP ..."
./waf --run "scratch/lte --simTime=100 --isTcp=0" > ~/Documents/workspace/lena/results/tcp/data-scripts/radio/UDP_LOG 2>&1 

echo "cleaning DlRxPhyStats.txt and others ..."
rm ~/Documents/workspace/lena/DlRxPhyStats.txt ~/Documents/workspace/lena/DlTxPhyStats.txt  ~/Documents/workspace/lena/UlRxPhyStats.txt ~/Documents/workspace/lena/UlRxPhyStats.txt 

echo "running lte TCP ..."
./waf --run "scratch/lte --simTime=100 --isTcp=1" > ~/Documents/workspace/lena/results/tcp/data-scripts/radio/TCP_LOG 2>&1 

cp ~/Documents/workspace/lena/results/tcp/data-scripts/radio/UDP_LOG ~/Documents/workspace/lena/results/tcp/graphs/radio/raw-data
cp ~/Documents/workspace/lena/results/tcp/data-scripts/radio/TCP_LOG ~/Documents/workspace/lena/results/tcp/graphs/radio/raw-data
# cp ~/Documents/workspace/lena/results/tcp/data-scripts/radio/{cwnd.txt,rto_value_tmp.txt,last_rtt_sample_tmp.txt,highest_tx_seq.txt,next_tx_seq.txt,queues.txt, tcp-put, tcp-put-ack, udp-put, udp-put-ack} ~/Documents/workspace/lena/results/tcp/graphs/radio/raw-data
cd ~/Documents/workspace/lena/results/tcp/data-scripts/radio/
cp *.dat ~/Documents/workspace/lena/results/tcp/graphs/radio/plotting-data
cp plot-averaged.txt ~/Documents/workspace/lena/results/tcp/graphs/radio/plotting-data
# cp last_rtt_sample_tmp.txt ~/Documents/workspace/lena/results/tcp/graphs/radio/raw-data
# cp highest_tx_seq.txt ~/Documents/workspace/lena/results/tcp/graphs/radio/raw-data
# cp next_tx_seq.txt ~/Documents/workspace/lena/results/tcp/graphs/radio/raw-data
# cp queues.txt ~/Documents/workspace/lena/results/tcp/graphs/radio/raw-data
# cp tcp-put.txt ~/Documents/workspace/lena/results/tcp/graphs/radio/raw-data
# cp tcp-put-ack.txt ~/Documents/workspace/lena/results/tcp/graphs/radio/raw-data
# cp udp-put.txt ~/Documents/workspace/lena/results/tcp/graphs/radio/raw-data
# cp udp-put-ack.txt ~/Documents/workspace/lena/results/tcp/graphs/radio/raw-data

# cd ~/Documents/workspace/lena/

# echo "moving output files to ../data-scripts/radio/ ..."
# mv cwnd.txt rto_value_tmp.txt last_rtt_sample_tmp.txt highest_tx_seq.txt next_tx_seq.txt queues.txt ~/Documents/workspace/lena/results/tcp/data-scripts/radio/

echo "plotting ..."
cd ~/Documents/workspace/lena/results/tcp/
pwd
./dr-radio.sh


