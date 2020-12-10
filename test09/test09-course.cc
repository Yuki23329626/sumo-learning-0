#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include "ns3/core-module.h"
#include "ns3/mobility-module.h"
#include "ns3/ns2-mobility-helper.h"
#include "ns3/lte-module.h"
#include "ns3/network-module.h"
#include "ns3/config-store.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/spectrum-module.h"
#include "ns3/animation-interface.h"

using namespace ns3;
using namespace std;

/*
目前要做甚麼
找一個城市的地圖，目前暫定台北市，1km*1km 的大小，兩百公尺設置一個 eNB，約 40 個好了
UE 約 300 台
有辦法找到，在某個特定時間點，哪個 UE 會選擇哪個 eNB
目前有兩個 callback function，針對 SINR 跟 Mobility
目前應該是先考慮 Mobility，不過不確定這是不是位置移動就會 callback ，待確認
網路節點需要：blockchain node，IPFS，ORACLE 之類的節點，互相之間的傳輸延遲
考慮上傳跟下載所需的時間，怎麼算
確認 SDN 換手需要哪些資訊，要跟那些節點溝通，抽象化計算，比較傳統 hndover 跟 SDN handover 的時間
handover 考慮 prepare time 跟 completion time，也許可以比較
*/

/*

         ENB                                   Core network
  *    *    *    *                    SW   SW   SW  SDNC IPFS ORACLE
  |    |    |    |   Point-To-Point    |    |    |    |    |    |
 n3   n2   n1   n0 -----------------  n13  n14  n15  n16  n17  n18
                          LTE          |    |    |    |    |    | 
                                      ============================
                                              LAN 10.1.2.0
*/



int main(int argc, char *argv[])
{
  string traceFile = "test09.tcl";
  int nodeNum = 100;
  int duration = 1001;
  int selectedEnb = 0;
  string outputDir = "test09-course1"
  string outputFileName = ""

  CommandLine cmd;
  cmd.AddValue("traceFile", "Ns2 movement trace file", traceFile);
  cmd.AddValue("nodeNum", "Number of nodes", nodeNum);
  cmd.AddValue("duration", "Duration of Simulation", duration);
  cmd.AddValue("selectedEnb", "Select eNB ID", selectedEnb);
  cmd.AddValue("outputDir", "Output directory", outputDir);
  cmd.Parse(argc, argv);

  // Enable logging from the ns2 helper
  LogComponentEnable("Ns2MobilityHelper", LOG_LEVEL_DEBUG);

  char CHAR_OUTPUT_DIR[outputDir.length()+1];
  strcpy(CHAR_OUTPUT_DIR, outputDir.c_str());
  mkdir(CHAR_OUTPUT_DIR, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  output_file = OUTPUT_DIR + "/" + to_string(SELECTED_ENB) + "_" + OUTPUT_FILE;

  Simulator::Stop(Seconds(duration));
  Simulator::Run();
  Simulator::Destroy();
  return 0;
}