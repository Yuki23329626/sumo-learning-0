1. generate a bundle file from created scenario
sumo -c test03.sumocfg --fcd-output test03.xml

2. create mobility tract to import for NS3
python traceExporter.py --fcd-input test03.xml --ns2mobility-output test03.tcl
