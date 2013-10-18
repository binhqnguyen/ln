#!/bin/bash

grep "TcpSocketBase" TCP_LOG > socket.timeout
grep "[node 2]" socket.timeout > socket_node_2.timeout
grep -C 5 "SendDataPacket Schedule ReTxTimeout at time" socket_node_2.timeout > TIMEOUT.timeout
grep -C 4 "retxing seq" socket.timeout > RETRANS.timeout
grep "2 RttEstimator" TCP_LOG > rtt_estimator_2.timeout
grep "RttEstimator:RetransmitTimeout(): RetransmitTimeout:  var" rtt_estimator_2.timeout > values.timeout
grep "Schedule ReTxTimeout at time" socket_node_2.timeout > schedule.timeout
grep "New ack of" socket_node_2.timeout > newack.timeout
grep "Dupack ack of" socket_node_2.timeout > dupack.timeout
grep "ReTxTimeout Expired at time" socket_node_2.timeout > RTO.txt
grep " updated to cwnd" TCP_LOG > ssthreshold.txt


