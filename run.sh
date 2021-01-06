#!/bin/bash

cd sumo-learning
git pull
echo "cp ./ns3lte/lteudp_test1.cc ../scratch/"
cp ./ns3lte/lteudp_test1.cc ../scratch/
echo "cp ./test09/test09.tcl ../scratch/"
cp ./test09/test09.tcl ../scratch/
echo "cp ./test09/2.png ../scratch/"
cp ./test09/2.png ../scratch/
cd ..
cd ..
echo "./waf --run "scratch/lteudp_test1""
./waf --run "scratch/lteudp_test1"
