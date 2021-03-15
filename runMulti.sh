#!/bin/bash

cd sumo-learning
git pull
cp ./ns3lte/lteudpMulti.cc ../scratch/
cp ./test10/test10.tcl ../scratch/
cd ..
(./waf --run "scratch/lteudpMulti --numberOfUes=10 --startUe=0 --endUe=5 --isSdnEnabled=true --isDownlink=true --isUplink=true" > log.lteudpMulti 2>&1) &
(./waf --run "scratch/lteudpMulti --numberOfUes=10 --startUe=5 --endUe=10 --isSdnEnabled=true --isDownlink=true --isUplink=true" > log.lteudpMulti 2>&1) &
