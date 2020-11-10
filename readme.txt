1. generate a bundle file from created scenario
sumo -c test04.sumocfg --fcd-output test04.xml

會產生 xml 檔

2. create mobility tract to import for NS3
python traceExporter.py --fcd-input test04.xml --ns2mobility-output test03.tcl

會產生 tcl 檔

3. ./waf --run "test04 --traceFile=scratch/test04.tcl --nodeNum=300 --duration=60"
產生結果