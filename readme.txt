1. generate a bundle file from created scenario
sumo -c test05.sumocfg --fcd-output test05.xml

會產生 xml 檔

2. create mobility tract to import for NS3
python traceExporter.py --fcd-input test05.xml --ns2mobility-output test05.tcl

會產生 tcl 檔

3. ./waf --run "test05 --traceFile=scratch/test05.tcl --nodeNum=170 --duration=60 --selectedEnb=18"
產生結果

cd sumo-learning
git pull
cp test05/test05.cc ../scratch/
cp test05/test05.tcl ../scratch/
cd ..

./waf --run "test05 --traceFile=scratch/test05.tcl --nodeNum=170 --duration=60 --selectedEnb=1"