#!/bin/bash

grep "TcpSocketBase" TCP_LOG > socket.timeout
grep "[node 2]" socket.timeout > socket_node_2.timeout
grep -C 5 "SendDataPacket Schedule ReTxTimeout at time" > TIMEOUT.timeout
grep -C 4 "retxing seq" socket.timeout > RETRANS.timeout
