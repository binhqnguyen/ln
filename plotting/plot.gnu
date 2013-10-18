
##======CONST=======
#TCP="TcpTahoe"
#BUFF="50 pkts"
BUFFSIZE=50
#x1 = 0
#x2 = 100
SEGSIZE=536
PKTSIZE=578
#CONST_A=BUFFSIZE
#CONST_B=BUFFSIZE-1
#CONST_C=BUFFSIZE-2
#CONST_D=BUFFSIZE-3

CONST_A=1
CONST_B=2
CONST_C=3
CONST_D=4

#=============Functions=============
cnt=0
increment(cnt) = (cnt=cnt+1,cnt)

filter_ssthresh(x)=((x>=999999)?1/0:x)
filter(x)=((x<100||x>1000000)?1/0:x)

#======== d=700,v=3 ========
reset
set title "Rate: ".TCP.", Buffer ".BUFF
set key inside bottom right box
set xlabel "Time (s)"
set xtic 10
set xrange [x1:x2]
set x2range [x1:x2]
#set y2label "Congestion window (Bytes)"
#set yrange [0:2000]
#set xrange [0:100]
set ylabel "Rate (kbps)"
set output "rate.svg"
#set y2tics nomirror tc lt 6

set terminal svg


plot "udp-put.txt" using 1:3 title "Radio bandwidth" with lines,\
"tcp-put.txt" using 1:3 title "TCP throughput (Rx rate)" with lines,\
"RTO.txt" using 3:(0) title "Tcp Timeout",\
"3ACKs.txt" using 3:(10) title "3AKCs"
#"cwnd.txt" using 1:5 title "cwnd" with lines axis x2y2

#"tcp-put.txt" using 1:11 title "TCP Tx rate" with lines,\
#"tcp-put.txt" using 1:12 title "TCP delay" with lines axis x2y2,\
#"tcp-put.txt" using 1:13 title "TCP ack delay" with lines axis x2y2

#===========================================
#===========================================
reset
set title "Congestion window ".TCP.", ".BUFF
set key inside top right box
set xlabel "Time (s)"
#set xtic 10
set ylabel "value (packets)"
set y2label "delay (ms)"
set output "cwnd-detail-".x1."-".x2.".svg"
#set y2tics nomirror tc lt 2
#set y2range [0:150000]
#set yrange [0:1200]
set xrange [x1:x2]
set x2range [x1:x2]
set y2tics nomirror tc lt 2

set terminal svg


plot "cwnd.txt" using 1:($5/SEGSIZE) title "cwnd" pt 0 lc 1,\
"tcp-put.txt" using 1:12 title "TCP delay" with lines axis x2y2,\
"RTO.txt" using 3:(11) title "TCP timeout",\
"3ACKs.txt" using 3:(50) title "3ACKs",\
"queues.txt" using 2:($4/PKTSIZE) title "enb_radio_queue" pt 0 lc 2,\
"ssthreshold.txt" using 3:(filter_ssthresh($14)/SEGSIZE) title "ssthreshold" with lines
#"newack_partial.txt" using 3:15 title "newack in FR" pt 0,\
#"newack_fullack.txt" using 3:17 title "received full ack, leaving FR" pt 0,\
#"Dupack_FASTRECOVERY.txt" using 3:15 title "Dupack IN FR" pt 0
#===========================================
reset

set output "cwnd_rx_rate-".x1."-".x2.".svg"
set terminal svg size 700,700 fsize 14
set multiplot layout 2,1
set tmargin 3
set bmargin 3
set lmargin 10

set title "Congestion window ".TCP.", ".BUFF
set key inside top right box
set xlabel "Time (s)"
#set xtic 10
set ylabel "cwnd (segment)"
#set y2label ""
#set y2tics nomirror tc lt 2
#set yrange [0:1200]
set xrange [x1:x2]
set x2range [x1:x2]
set y2tics nomirror tc lt 6
#set log y2




plot "cong_avoid.txt" using 3:($12/SEGSIZE) title "congestion avoidance" pt 0 lc 1,\
"slow_start.txt" using 3:($12/SEGSIZE) title "slow start" pt 0 lc 2,\
"queues.txt" using 2:($4/PKTSIZE) title "enb_radio_queue" with lines lc 3,\
"RTO.txt" using 3:(5) title "Timeout event" pt 1 lc 4,\
"3ACKs.txt" using 3:(3) title "3AKC event" pt 1 lc 5,\
"enb_dev_queue_drop.txt" using 1:(1) title "enb queue droptail" pt 1 lc 6,\
"tcp-put.txt" using 1:12 title "TCP delay" with lines lc 7 axis x2y2

#"rwnd.txt" using 1:($2/536) title "remote side flow control window" with lines
#plot "cwnd.txt" using 1:5 title "cwnd" with lines,\
#"newack_partial.txt" using 3:15 title "newack in FR" pt 1,\
#"newack_fullack.txt" using 3:17 title "received full ack, #leaving FR" pt 1,\
#"Dupack_FASTRECOVERY.txt" using 3:15 title "Dupack IN FR" pt 1,\

#"tcp-put.txt" using 1:12 title "TCP delay" with lines axis x2y2,\
#"enqueued.txt" using 2:3 title "enqueued per 20ms" with lines axis x2y2
#"tcp-put.txt" using 1:12 title "TCP delay" with lines axis x2y2,\
#"tcp-put.txt" using 1:13 title "TCP ack delay" with lines axis x2y2

set title "Rate: ".TCP.", Buffer ".BUFF
set key inside bottom right box
#set xtic 10
set xrange [x1:x2]
set x2range [x1:x2]
#set y2label "Congestion window (Bytes)"
#set yrange [0:2000]
#set xrange [0:100]
set ylabel "Rate (kbps)"
#set y2tics nomirror tc lt 6


plot "udp-put.txt" using 1:3 title "Radio bandwidth" with lines lc 1,\
"tcp-put.txt" using 1:3 title "TCP throughput (Rx rate)" with lines lc 2

unset multiplot

#===========================================
reset
set title "Packet sequence ".TCP.", ".BUFF
set key inside bottom right box
set xlabel "Time (s)"
#set xtic 10
set ylabel "packet sequence #"
#set y2label "queue droptail"
set output "sequence.svg"
#set y2tics nomirror tc lt 2
set y2range [0:1]
#set yrange [0:1200]
set xrange [x1:x2]
#set x2range [x1:x2]
set y2tics nomirror tc lt 4
#set log y2

set terminal svg


plot "next_tx_seq.txt" using 1:(($3-1)/SEGSIZE) title "next tx sequence #" pt 1,\
"enb_dev_queue_drop.txt" using 1:(0) title "enb queue droptail" pt 2 axis x1y2,\
"retransmit.txt" using 3:(($16-1)/SEGSIZE) title "3acks sequence #" pt 1,\
"ack_seq.txt" using 1:(($12-1)/SEGSIZE) title "ack sequence #" pt 1,\
"RTO.txt" using 3:(11) title "TCP timeout" axis x1y2 pt 2 lt -1
#plot "highest_sent_seq.txt" using 1:3 title "highest sent sequence #" pt 1,\


#===========================================
reset
set title "TCP rtt estimator ".TCP.", ".BUFF
set key inside top right box
set xlabel "Time (s)"
#set xtic 10
set ylabel "time (ms)"
#set y2label "queue size (Packets)"
set output "rttestimator.svg"
#set y2tics nomirror tc lt 2
#set y2range [0:1200]
#set yrange [450:600]
set xrange [x1:x2]
#set x2range [x1:x2]
#set y2tics nomirror tc lt 8
#set log y2

set terminal svg


plot "rto_value.txt" using 1:($2*1000) title "retransmit time out value" with lines,\
"last_rtt_sample.txt" using 1:($2*1000) title "last rtt sample value" with lines,\
"RTO.txt" using 3:(11) title "TCP timeout",\
"3ACKs.txt" using 3:(20) title "3ACKs"
#"rtt_value.txt" using 1:($2*1000) title "rtt value" pt 1,\
#plot "rtt_estimator_rto.txt" using 1:($2*1000) title "retransmit timeout value" with lines,\
#"measured_rtt.txt" using 1:($2/1000000) title "measured rtt" pt 0 with lines,\




