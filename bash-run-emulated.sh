#!/bin/bash
cd ~/ln/

./rp-emulated.sh  emutated_tahoe_5pkts

perl -pi -e 's/DropTailQueue::MaxPackets \"5\"/DropTailQueue::MaxPackets \"10\"/g' emulated-in.txt
./rp-emulated.sh  emulated_tahoe_10pkts

perl -pi -e 's/DropTailQueue::MaxPackets \"10\"/DropTailQueue::MaxPackets \"20\"/g' emulated-in.txt
./rp-emulated.sh  emulated_tahoe_20pkts

perl -pi -e 's/DropTailQueue::MaxPackets \"20\"/DropTailQueue::MaxPackets \"40\"/g' emulated-in.txt
./rp-emulated.sh  emulated_tahoe_40pkts

perl -pi -e 's/DropTailQueue::MaxPackets \"40\"/DropTailQueue::MaxPackets \"80\"/g' emulated-in.txt
./rp-emulated.sh  emulated_tahoe_80pkts

perl -pi -e 's/DropTailQueue::MaxPackets \"80\"/DropTailQueue::MaxPackets \"160\"/g' emulated-in.txt
./rp-emulated.sh  emulated_tahoe_160pkts

perl -pi -e 's/DropTailQueue::MaxPackets \"160\"/DropTailQueue::MaxPackets \"320\"/g' emulated-in.txt
./rp-emulated.sh  emulated_tahoe_320pkts



##########NewReno#######
perl -pi -e 's/::SocketType \"ns3::TcpTahoe\"/::SocketType \"ns3::TcpNewReno\"/g' emulated-in.txt
perl -pi -e 's/DropTailQueue::MaxPackets \"320\"/DropTailQueue::MaxPackets \"5\"/g' emulated-in.txt
./rp-emulated.sh  emutated_newreno_5pkts

perl -pi -e 's/DropTailQueue::MaxPackets \"5\"/DropTailQueue::MaxPackets \"10\"/g' emulated-in.txt
./rp-emulated.sh  emulated_newreno_10pkts

perl -pi -e 's/DropTailQueue::MaxPackets \"10\"/DropTailQueue::MaxPackets \"20\"/g' emulated-in.txt
./rp-emulated.sh  emulated_newreno_20pkts

perl -pi -e 's/DropTailQueue::MaxPackets \"20\"/DropTailQueue::MaxPackets \"40\"/g' emulated-in.txt
./rp-emulated.sh  emulated_newreno_40pkts

perl -pi -e 's/DropTailQueue::MaxPackets \"40\"/DropTailQueue::MaxPackets \"80\"/g' emulated-in.txt
./rp-emulated.sh  emulated_newreno_80pkts

perl -pi -e 's/DropTailQueue::MaxPackets \"80\"/DropTailQueue::MaxPackets \"160\"/g' emulated-in.txt
./rp-emulated.sh  emulated_newreno_160pkts

perl -pi -e 's/DropTailQueue::MaxPackets \"160\"/DropTailQueue::MaxPackets \"320\"/g' emulated-in.txt
./rp-emulated.sh  emulated_newreno_320pkts
