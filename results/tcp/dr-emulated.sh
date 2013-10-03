	#!/bin/bash

	cd /Users/binh/Documents/workspace/lena/results/tcp
	pwd

	TCP='TcpTahoe'
	cd graphs
	rm *.svg
	cd ..

	cd data-scripts/emulated
	pwd

######### queues logs from log file ###########
	cat < TCP_LOG | grep "Queue:" > queue
	grep "0 Queue:GetTotalReceivedBytes():" queue > ue_dev_total_received.txt
	grep "2 Queue:Dequeue(): m_traceDequeue" queue > endhost_dev_dequeue_tmp.txt
    grep "1 Queue:Enqueue(): m_traceEnqueue" queue > enb_dev_enqueue_tmp.txt	
    grep "1 Queue:Dequeue(): m_traceDequeue" queue > enb_dev_dequeue_tmp.txt	
    grep "1 Queue:Drop(): m_traceDrop" queue > enb_dev_queue_drop_tmp.txt
###############################################



################ TCP version specified ###########
	cat < TCP_LOG | grep "$TCP:DupAck" | grep -v '10.1.3.1' | grep "Triple Dup Ack\| Triple dupack" | grep retransmit > 3ACKs.txt
	cat < TCP_LOG | grep "$TCP:DupAck" | grep -v '10.1.3.1' | grep "Dupack in fast recovery mode" > Dupack_FASTRECOVERY.txt
	cat < TCP_LOG | grep "$TCP:NewAck" | grep 'Partial ACK' > newack_partial.txt
	cat < TCP_LOG | grep "$TCP:NewAck" | grep 'Received full ACK' > newack_fullack.txt
	cat < TCP_LOG | grep "$TCP:NewAck" | grep 'In SlowStart' > slow_start.txt
	cat < TCP_LOG | grep "$TCP:NewAck" | grep 'In CongAvoid' > cong_avoid.txt
	cat < TCP_LOG | grep "$TCP:DupAck" | grep 'retransmit missing segment' > retransmit.txt
	grep "$TCP:NewAck():" TCP_LOG | grep "$TCP receieved ACK" > ack_seq.txt
	grep "$TCP:NewAck():" TCP_LOG | grep "$TCP receieved ACK" > ssthreshold.txt
	cat < TCP_LOG | grep "$TCP:Retransmit" | grep 'RTO\|ReTxTimeout' > RTO.txt
##################################################


############# post processing: *_tmp files processing ########
	./get_queue_time.py

######plot and move graph files######
	gnuplot plot.gnu

	cp *.svg ../../graphs/emulated
	rm *.svg

