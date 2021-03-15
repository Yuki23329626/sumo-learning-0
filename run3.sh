#!/bin/bash

cd sumo-learning
git pull
cp ./ns3lte/lteudp_test3.cc ../scratch/
cp ./test10/test10.tcl ../scratch/
cd ..
./waf --run "scratch/lteudp_test3 --isSdnEnabled=true --isDownlink=true --isUplink=true --interPacketInterval=1000" > log.lteudp_test3_sdn 2>&1
