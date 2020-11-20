1. generate a bundle file from created scenario
sumo -c test06.sumocfg --fcd-output test06.xml --step-length 1

會產生 xml 檔

2. create mobility tract to import for NS3
python traceExporter.py --fcd-input test06.xml --ns2mobility-output test06.tcl

會產生 tcl 檔

3. ./waf --run "test06 --traceFile=scratch/test06.tcl --nodeNum=100 --duration=60 --selectedEnb=0 --outputDir=test06-1"
產生結果

cd sumo-learning
git pull
cp test06/test06.cc ../scratch/
cp test06/test06.tcl ../scratch/
cd ..
./waf --run "test06-delay --traceFile=scratch/test06.tcl --nodeNum=100 --duration=1000 --selectedEnb=0 --outputDir=test06-2"


./waf clean
./waf configure --build-profile=optimized --enable-examples --enable-tests

./waf clean
./waf configure --build-profile=debug --enable-examples --enable-tests

./waf --run "test05 --traceFile=scratch/test05.tcl --nodeNum=100 --duration=60 --selectedEnb=3"
CXXFLAGS="-std=c++0x" ./waf configure
./waf clean

top -n 1 | grep nxshen | cut -c 1-15