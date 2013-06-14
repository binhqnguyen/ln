	#!/bin/bash

	cd /Users/binh/Documents/workspace/lena/results/tcp
	pwd

	TCP='TcpTahoe'
	cd graphs
	rm *.svg

	cd ..

	cd data-scripts/emulated
	pwd

	cat < TCP | grep "$TCP" > tcp-component
	cat < TCP | grep "Queue:" > queue
	cat < TCP | grep "RttEstimator:" > rttestimator
	cat < TCP | grep "TcpSocketBase" > TcpSocketBase

	cat < TCP | grep 10.1.3.1 | grep -v "TcpSocketBase" | grep -v enb | grep -v cwnd > tcp-put.txt
	cat < TCP | grep cwnd_from | grep -v enb | grep -v '10.1.3.1' > cwnd.txt
	cat < tcp-component | grep "$TCP:DupAck" | grep -v '10.1.3.1' | grep "Triple Dup Ack\| Triple dupack" | grep retransmit > 3ACKs.txt
	cat < tcp-component | grep "$TCP:DupAck" | grep -v '10.1.3.1' | grep "Dupack in fast recovery mode" > Dupack_FASTRECOVERY.txt
	cat < tcp-component | grep "$TCP:NewAck" | grep 'Partial ACK' > newack_partial.txt
	cat < tcp-component | grep "$TCP:NewAck" | grep 'Received full ACK' > newack_fullack.txt
	cat < tcp-component | grep "$TCP:NewAck" | grep 'In SlowStart' > slow_start.txt
	cat < tcp-component | grep "$TCP:NewAck" | grep 'In CongAvoid' > cong_avoid.txt
	cat < tcp-component | grep "$TCP:DupAck" | grep 'retransmit missing segment' > retransmit.txt

	#TCP parameters
	grep "TcpSocketBase:AvailableWindow():" TcpSocketBase > rwnd_tmp.txt
	grep "RTO_value" TCP > rto_value_tmp.txt
	#grep "2 RttEstimator:RetransmitTimeout(): RetransmitTimeout:  return" rttestimator > rtt_estimator_rto_tmp.txt
	#cat rttestimator | grep "2 RttEstimator:Measurement" | grep -v "m_variance" > measured_rtt_tmp.txt
	grep "awnd_value" TCP > awnd_value.txt
	grep "last_rtt_sample" TCP > last_rtt_sample_tmp.txt
	grep "highest_sent_seq" TCP > highest_sent_seq.txt
	grep "$TCP:NewAck():" tcp | grep "$TCP receieved ACK" > ack_seq.txt
	grep "next_tx_seq" TCP > next_tx_seq.txt
	grep "0 Queue:GetTotalReceivedBytes():" queue > ue_dev_total_received.txt
	grep "2 Queue:Dequeue(): m_traceDequeue" queue > endhost_dev_dequeue_tmp.txt
    grep "1 Queue:Enqueue(): m_traceEnqueue" queue > enb_dev_enqueue_tmp.txt	
    grep "1 Queue:Dequeue(): m_traceDequeue" queue > enb_dev_dequeue_tmp.txt	
    grep "1 Queue:Drop(): m_traceDrop" queue > enb_dev_queue_drop_tmp.txt
    #grep "2 RttEstimator:AckSeq(): RttEstimator::AckSeq Value" TCP > rtt_value_tmp.txt

	grep "$TCP:NewAck():" TCP | grep "$TCP receieved ACK" > ssthreshold.txt

	./get_queue_time.py



	cat < tcp | grep "$TCP:Retransmit" | grep 'RTO\|ReTxTimeout' > RTO.txt

	cat < TCP | grep "QQQQ:" > queues.txt
	cat < TCP | grep "ENQUEUED:" > enqueued.txt
	cat < UDP | grep 10.1.3.1 | grep -v ID1 | grep -v enb > udp-put.txt


	gnuplot plot-averaged.txt

	cp *.svg ../../graphs/emulated
	rm *.svg

