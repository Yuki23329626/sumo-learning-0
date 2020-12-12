1. generate a bundle file from created scenario
sumo -c test09.sumocfg --fcd-output test09.xml --step-length 1

會產生 xml 檔

2. create mobility tract to import for NS3
python traceExporter.py --fcd-input test09.xml --ns2mobility-output test09.tcl
python D:\SUMO\tools\traceExporter.py --fcd-input D:\github\sumo-learning\test09\test09.xml --ns2mobility-output D:\github\sumo-learning\test09\test09.tcl

3. 產生 excel 黨
python D:\SUMO\tools\traceExporter.py traceExporter.py --fcd-input D:\github\sumo-learning\test09\test09.xml --gpsdat-output D:\github\sumo-learning\test09\test09.csv

D:
cd git

會產生 tcl 檔

3. ./waf --run "test09 --traceFile=scratch/test09.tcl --nodeNum=100 --duration=60 --selectedEnb=0 --outputDir=test09-4"
產生結果

cd sumo-learning
git pull
cp test09/test09.cc ../scratch/
cp test09/test09.tcl ../scratch/
cd ..
./waf --run "test09 --traceFile=scratch/test09.tcl --nodeNum=100 --duration=1000 --selectedEnb=0 --outputDir=test09-4"

./waf --run "test09-delay --traceFile=scratch/test09.tcl --nodeNum=100 --duration=1000 --selectedEnb=0 --outputDir=test09-4"

```bash

cd sumo-learning
git pull
cp test08/test08.cc ../scratch/
cp test08/test08.tcl ../scratch/
cd ..
# ./waf
# export NS_LOG=FirstScriptExample=info
./waf --run "scratch/test08"


```

```bash

cd sumo-learning
git pull
cp test09/test09-course.cc ../scratch/
cp test09/test09.tcl ../scratch/
cd ..
# ./waf
# export NS_LOG=FirstScriptExample=info
./waf --run "scratch/test09-course"

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