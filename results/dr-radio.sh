	#!/bin/bash

	cd /Users/binh/Documents/workspace/lena/results/tcp
	pwd

	TCP='TcpTahoe'
	cd graphs
	rm *.svg
	cd ..

	cd data-scripts/radio
	pwd

######### queues logs from log file ###########
	cat < TCP_LOG | grep "Queue:" > queue.raw
	grep "0 Queue:GetTotalReceivedBytes():" queue.raw > spgw_dev_total_received.tmp
	grep "1 Queue:Dequeue(): m_traceDequeue" queue.raw > endhost_dev_dequeue_tmp.tmp
    grep "2 Queue:Enqueue(): m_traceEnqueue" queue.raw > enb_dev_enqueue_tmp.tmp	
    grep "2 Queue:Dequeue(): m_traceDequeue" queue.raw > enb_dev_dequeue_tmp.tmp	
    grep "2 Queue:Drop(): m_traceDrop" queue.raw > enb_dev_queue_drop_tmp.tmp
    grep "Queue:Drop(): m_traceDrop" queue.raw > all_p2p_netdev_queue_drop_tmp.tmp
###############################################



################ TCP version specified ###########
	cat < TCP_LOG | grep "$TCP:DupAck" | grep -v '10.1.3.1' | grep "Triple Dup Ack\| Triple dupack" | grep retransmit > 3ACKs.dat
	cat < TCP_LOG | grep "$TCP:DupAck" | grep -v '10.1.3.1' | grep "Dupack in fast recovery mode" > Dupack_FASTRECOVERY.dat
	cat < TCP_LOG | grep "$TCP:NewAck" | grep 'Partial ACK' > newack_partial.dat
	cat < TCP_LOG | grep "$TCP:NewAck" | grep 'Received full ACK' > newack_fullack.dat
	cat < TCP_LOG | grep "$TCP:NewAck" | grep 'In SlowStart' > slow_start.dat
	cat < TCP_LOG | grep "$TCP:NewAck" | grep 'In CongAvoid' > cong_avoid.dat
	cat < TCP_LOG | grep "$TCP:DupAck" | grep 'retransmit missing segment' > retransmit.dat
	grep "$TCP:NewAck():" TCP_LOG | grep "$TCP receieved ACK" > ack_seq.dat
	grep "$TCP:NewAck():" TCP_LOG | grep "$TCP receieved ACK" > ssthreshold.dat
	cat < TCP_LOG | grep "$TCP:Retransmit" | grep 'RTO\|ReTxTimeout' > RTO.dat
##################################################


################ RLC UM log ###########
cat TCP_LOG | grep "2 LteRlcUm:" > enb_rlc_um.raw
cat enb_rlc_um.raw | grep "TxBuffer is full. RLC SDU discarded" > enb_rlcum_txqueue_drop_tmp.tmp
cat enb_rlc_um.raw | grep "txBufferSize = " > enb_rlcum_txqueue_size_tmp.tmp



############# post processing: *_tmp files processing ########
	./check_radio_error.py
	./rlc_um_processing.py
	./get_queue_time.py

######plot and move graph files######
	gnuplot plot-averaged.txt

	cp *.svg ../../graphs/radio
	cp *.dat ~/Documents/workspace/lena/results/tcp/graphs/radio/plotting-data

