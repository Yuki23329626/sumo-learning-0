1. generate a bundle file from created scenario
sumo -c test08.sumocfg --fcd-output test08.xml --step-length 1

會產生 xml 檔

2. create mobility tract to import for NS3
python traceExporter.py --fcd-input test08.xml --ns2mobility-output test08.tcl

會產生 tcl 檔

3. ./waf --run "test08 --traceFile=scratch/test08.tcl --nodeNum=100 --duration=60 --selectedEnb=0 --outputDir=test08-1"
產生結果

cd sumo-learning
git pull
cp test08/test08.cc ../scratch/
cp test08/test08.tcl ../scratch/
cd ..
./waf --run "test08 --traceFile=scratch/test08.tcl --nodeNum=100 --duration=1000 --selectedEnb=0 --outputDir=test08-3"

./waf --run "test08-delay --traceFile=scratch/test08.tcl --nodeNum=100 --duration=1000 --selectedEnb=0 --outputDir=test08-1"

```bash

cd sumo-learning
git pull
cp test.cc ../scratch/
cd ..
# ./waf
# export NS_LOG=FirstScriptExample=info
./waf --run "scratch/test"

```


cd sumo-learning
git pull
cp test07/test07.cc ../scratch/
cp test07/test07-delay.cc ../scratch/
cp test07/test07.tcl ../scratch/
cd ..
./waf --run test07-delay


./waf clean
./waf configure --build-profile=optimized --enable-examples --enable-tests

./waf clean
./waf configure --build-profile=debug --enable-examples --enable-tests

./waf --run "test05 --traceFile=scratch/test05.tcl --nodeNum=100 --duration=60 --selectedEnb=3"
CXXFLAGS="-std=c++0x" ./waf configure
./waf clean

top -n 1 | grep nxshen | cut -c 1-15