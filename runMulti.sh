#!/bin/bash

trap "kill 0" INT

cd sumo-learning
git pull
cp ./ns3lte/lteudpMulti.cc ../scratch/
cp ./test10/test10.tcl ../scratch/
cd ..

logPrefixName = "log.lteudpMultiPart"

for ((i=0; i<2; i++))
do
    logName = "${logPrefixName}${i}"
    echo "logName = ${logName}"
    endUe = $(($i+5))
    echo "endUe = ${endUe}"
    (./waf --run "scratch/lteudpMulti --numberOfUes=10 --startUe=${i} --endUe=${endUe} --isSdnEnabled=true --isDownlink=true --isUplink=true" > $logName 2>&1) &
done

wait