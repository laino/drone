#!/bin/sh

# Compiles and transfers the binaries to the rPI, stopping the old
# library and starting the new one.
#
# It is recommended to copy your ssh public key to the rPI first,
# to avoid having to give your password every time.
# 
# Usage:
# 
# ./transfer.sh 192.168.10.2
#

IP=$1

(cd build/root/; tar -Pc .) | 
    ssh root@$IP "cd /; tar -px"
