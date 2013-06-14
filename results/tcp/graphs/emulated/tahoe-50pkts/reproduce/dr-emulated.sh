	#!/bin/bash

	cd /Users/binh/Documents/workspace/lena/results/tcp
	pwd

	TCP='TcpTahoe'
	cd graphs
	rm *.svg

	cd ..

	cd data-scripts/emulated
	pwd

	cat < TCP | grep 10.1.3.1 | grep -v enb | grep -v cwnd > tcp-put.txt
	cat < TCP | grep cwnd_from | grep -v enb | grep -v '10.1.3.1' > cwnd.txt
	cat < TCP | grep "$TCP:DupAck" | grep -v '10.1.3.1' | grep "Triple Dup Ack\| Triple dupack" | grep retransmit > 3ACKs.txt
	cat < TCP | grep "$TCP:DupAck" | grep -v '10.1.3.1' | grep "Dupack in fast recovery mode" > Dupack_FASTRECOVERY.txt
	cat < TCP | grep "$TCP:NewAck" | grep 'Partial ACK' > newack_partial.txt
	cat < TCP | grep "$TCP:NewAck" | grep 'Received full ACK' > newack_fullack.txt
	cat < TCP | grep "$TCP:NewAck" | grep 'In SlowStart' > slow_start.txt
	cat < TCP | grep "$TCP:NewAck" | grep 'In CongAvoid' > cong_avoid.txt
	cat < TCP | grep "$TCP:DupAck" | grep 'retransmit missing segment' > retransmit.txt

	#TCP parameters
	grep "RTO_value" TCP > rto_value.txt
	grep "awnd_value" TCP > awnd_value.txt
	grep "last_rtt_sample" TCP > last_rtt_sample.txt
	grep "highest_sent_seq" TCP > highest_sent_seq.txt
	grep "$TCP:NewAck():" TCP | grep "$TCP receieved ACK" > ack_seq.txt
	grep "next_tx_seq" TCP > next_tx_seq.txt
	grep "0 Queue:GetTotalReceivedBytes():" TCP > ue_dev_total_received.txt
	grep "2 Queue:Dequeue(): m_traceDequeue" TCP > endhost_dev_dequeue_tmp.txt
    grep "1 Queue:Enqueue(): m_traceEnqueue" TCP > enb_dev_enqueue_tmp.txt	
    grep "1 Queue:Dequeue(): m_traceDequeue" TCP > enb_dev_dequeue_tmp.txt	

	grep "$TCP:NewAck():" TCP | grep "$TCP receieved ACK" > ssthreshold.txt

	./get_queue_time.py



	cat < TCP | grep "$TCP:Retransmit" | grep 'RTO\|ReTxTimeout' > RTO.txt

	cat < TCP | grep "QQQQ:" > queues.txt
	cat < TCP | grep "ENQUEUED:" > enqueued.txt
	cat < UDP | grep 10.1.3.1 | grep -v ID1 | grep -v enb > udp-put.txt


	gnuplot plot-averaged.txt

	cp *.svg ../../graphs/emulated
	rm *.svg

