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
 n3   n2   n1   n0 -----------------  n12  n13  n14  n15  n16  n17
                                       |    |    |    |    |    | 
                                      ============================
                                              LAN 10.1.1.0
*/

class UE_Info
{
public:
  void set_imsi(int imsi)
  {
    if (imsi >= 0)
      this->imsi = imsi;
  }

  void set_Position(Vector position)
  {
    this->position = position;
  }

  void CourseChange(Ptr<const MobilityModel> mobility)
  {
    Vector pos = mobility->GetPosition(); // Get position
    position = pos;
    double now = Simulator::Now().GetSeconds();
    *ofstream1 << now << "," << imsi  << "," << position.x << "," << position.y << endl;
  }

  // 拿到 UE 當前座標 position.x, position.y
  Vector get_Position()
  {
    return position;
  }

  // 使用 object 的 output stream
  void set_output(std::ofstream *ofstream1)
  {
      this->ofstream1 = ofstream1;
  }

private:
    int imsi = -1;
    Vector position;
    std::ofstream *ofstream1;
};

int main(int argc, char *argv[])
{
  string traceFile = "test09.tcl";
  int nCsma = 3;
  int nNode = 100;
  int duration = 1001;
  string outputDir = "test09-course-1"
  string outputFileName = "test09-course.csv"

  CommandLine cmd;
  cmd.AddValue("traceFile", "Ns2 movement trace file", traceFile);
  cmd.AddValue("nNode", "Number of nodes", nNode);
  cmd.AddValue("duration", "Duration of Simulation", duration);
  cmd.AddValue("outputDir", "Output directory", outputDir);
  cmd.Parse(argc, argv);

  // Enable logging from the ns2 helper
  LogComponentEnable("Ns2MobilityHelper", LOG_LEVEL_DEBUG);

  char CHAR_OUTPUT_DIR[outputDir.length()+1];
  strcpy(CHAR_OUTPUT_DIR, outputDir.c_str());
  mkdir(CHAR_OUTPUT_DIR, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  outputFileName = OUTPUT_DIR + "/" + to_string(SELECTED_ENB) + "_" + outputFileName;
  
  std::ofstream ofstream1;
  ofstream1.open(outputFileName);
  ofstream1 << "time(s),IMSI,X,Y" << endl;

  UE_Info *ue_info = (UE_Info *)malloc(sizeof(UE_Info) * nNode);
  Ns2MobilityHelper ns2 = Ns2MobilityHelper(traceFile);
  ns2.Install();

  NodeContainer p2pNodes;
  p2pNodes.Create (15);

  PointToPointHelper pointToPoint;
  NetDeviceContainer p2pDevices;
  p2pDevices = pointToPoint.Install (p2pNodes);

  NodeContainer csmaNodes;
  csmaNodes.Add (p2pNodes.Get (12));
  csmaNodes.Add (p2pNodes.Get (13));
  csmaNodes.Add (p2pNodes.Get (14));
  csmaNodes.Create(nCsma);

  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
  csma.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));

  NodeContainer ueNodes;
  ueNodes.Create(nNode);

  NodeContainer enbNodes;
  enbNodes.Add p2pNodes.Get (0);
  enbNodes.Add p2pNodes.Get (1);
  enbNodes.Add p2pNodes.Get (2);
  enbNodes.Add p2pNodes.Get (3);
  enbNodes.Add p2pNodes.Get (4);
  enbNodes.Add p2pNodes.Get (5);
  enbNodes.Add p2pNodes.Get (6);
  enbNodes.Add p2pNodes.Get (7);
  enbNodes.Add p2pNodes.Get (8);
  enbNodes.Add p2pNodes.Get (9);
  enbNodes.Add p2pNodes.Get (10);
  enbNodes.Add p2pNodes.Get (11);

  NetDeviceContainer csmaDevices;
  csmaDevices = csma.Install (csmaNodes);

  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  phy.SetChannel (channel.Create ());

  WifiHelper wifi;
  wifi.SetRemoteStationManager ("ns3::AarfWifiManager");

  WifiMacHelper mac;
  Ssid ssid = Ssid ("ns-3-ssid");
  mac.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid),
               "ActiveProbing", BooleanValue (false));

  // Install netdevice
  NetDeviceContainer ueDevices;
  ueDevices = wifi.Install (phy, mac, ueNodes);
  
  NetDeviceContainer enbDevices;
  mac.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid));
  enbDevices = wifi.Install (phy, mac, enbNodes);

  Ptr<ListPositionAllocator> enbPositionAlloc = CreateObject<ListPositionAllocator>();

  enbPositionAlloc->Add(Vector(583, 365, 0));
  enbPositionAlloc->Add(Vector(885, 338, 0));
  enbPositionAlloc->Add(Vector(1187, 328, 0));
  enbPositionAlloc->Add(Vector(1305, 322, 0));
  enbPositionAlloc->Add(Vector(596, 703, 0));
  enbPositionAlloc->Add(Vector(895, 690, 0));
  enbPositionAlloc->Add(Vector(1100, 682, 0));
  enbPositionAlloc->Add(Vector(1317, 679, 0));
  enbPositionAlloc->Add(Vector(602, 972, 0));
  enbPositionAlloc->Add(Vector(908, 955, 0));
  enbPositionAlloc->Add(Vector(1107, 953, 0));
  enbPositionAlloc->Add(Vector(1317, 950, 0));

  MobilityHelper enbMobility;
  enbMobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  enbMobility.SetPositionAllocator(enbPositionAlloc);
  enbMobility.Install(csmaNodes);
  
  Ptr<MobilityModel> ueMobilityModel;
  Ptr<LteUePhy> uephy;

  for (int i = 0; i < nNode; i++)
  {
    ueMobilityModel = ueNodes.Get(i)->GetObject<MobilityModel>();
    ue_info[i].set_Position(ueMobilityModel->GetPosition());
    ue_info[i].set_output(&ofstream1);

    uephy = ueDevices.Get(i)->GetObject<LteUeNetDevice>()->GetPhy();
    ue_info[i].set_imsi(ueDevices.Get(i)->GetObject<LteUeNetDevice>()->GetImsi());

    // uephy->TraceConnectWithoutContext("ReportCurrentCellRsrpSinr", MakeCallback(&UEs_Info::GetUeSinr, &ues_info[i]));
    ueMobilityModel->TraceConnectWithoutContext("CourseChange", MakeCallback(&UEs_Info::CourseChange, &ue_info[i]));
  }

  InternetStackHelper stack;
  stack.Install (csmaNodes);
  stack.Install (ueNodes);
  stack.Install (enbNodes);

  Ipv4AddressHelper address;

  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces;
  csmaInterfaces = address.Assign (csmaDevices);

  address.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces = address.Assign (p2pDevices);

  address.SetBase ("10.1.3.0", "255.255.255.0");
  address.Assign (staDevices);
  address.Assign (apDevices);

  // udp server
  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (csmaNodes.Get (2));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  // udp client
  UdpEchoClientHelper echoClient (csmaInterfaces.GetAddress (2), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = 
  echoClient.Install (ueNodes.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  Simulator::Stop (Seconds (10.0));

  Simulator::Stop(Seconds(duration));
  Simulator::Run();
  Simulator::Destroy();
  return 0;
}