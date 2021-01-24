  /* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
  /*
    * Copyright (c) 2013 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
    *
    * This program is free software; you can redistribute it and/or modify
    * it under the terms of the GNU General Public License version 2 as
    * published by the Free Software Foundation;
    *
    * This program is distributed in the hope that it will be useful,
    * but WITHOUT ANY WARRANTY; without even the implied warranty of
    * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    * GNU General Public License for more details.
    *
    * You should have received a copy of the GNU General Public License
    * along with this program; if not, write to the Free Software
    * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
    *
    * Author: Manuel Requena <manuel.requena@cttc.es>
    */
  
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/lte-module.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/config-store-module.h"

#include <iostream>
#include "ns3/lte-helper.h"
#include "ns3/epc-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/config-store.h"
#include <ns3/buildings-module.h>
#include "ns3/netanim-module.h"

#include "ns3/flow-monitor-module.h"
#include "ns3/log.h"
#include <sys/timeb.h>
#include <ns3/internet-trace-helper.h>
#include <ns3/spectrum-module.h>
#include <ns3/log.h>
#include <ns3/string.h>
#include <fstream>
#include <string>
#include <chrono>
#include <limits>
  
  using namespace ns3;
  using namespace std;
  
  NS_LOG_COMPONENT_DEFINE ("LenaX2HandoverMeasures");
  
  void
  NotifyConnectionEstablishedUe (std::string context,
                                  uint64_t imsi,
                                  uint16_t cellid,
                                  uint16_t rnti)
  {
    double now = Simulator::Now().GetSeconds();
    std::cout << "now: " << now << "\t";
    std::cout << context
              << " UE IMSI " << imsi
              << ": connected to CellId " << cellid
              << " with RNTI " << rnti
              << std::endl;
  }
  
  void
  NotifyHandoverStartUe (std::string context,
                          uint64_t imsi,
                          uint16_t cellid,
                          uint16_t rnti,
                          uint16_t targetCellId)
  {
    double now = Simulator::Now().GetSeconds();
    std::cout << "now: " << now << "\t";
    std::cout << context
              << " UE IMSI " << imsi
              << ": previously connected to CellId " << cellid
              << " with RNTI " << rnti
              << ", doing handover to CellId " << targetCellId
              << std::endl;
  }
  
  void
  NotifyHandoverEndOkUe (std::string context,
                          uint64_t imsi,
                          uint16_t cellid,
                          uint16_t rnti)
  {
    double now = Simulator::Now().GetSeconds();
    std::cout << "now: " << now << "\t";
    std::cout << context
              << " UE IMSI " << imsi
              << ": successful handover to CellId " << cellid
              << " with RNTI " << rnti
              << std::endl;
  }
  
  void
  NotifyConnectionEstablishedEnb (std::string context,
                                  uint64_t imsi,
                                  uint16_t cellid,
                                  uint16_t rnti)
  {
    double now = Simulator::Now().GetSeconds();
    std::cout << "now: " << now << "\t";
    std::cout << context
              << " eNB CellId " << cellid
              << ": successful connection of UE with IMSI " << imsi
              << " RNTI " << rnti
              << std::endl;
  }
  
  void
  NotifyHandoverStartEnb (std::string context,
                          uint64_t imsi,
                          uint16_t cellid,
                          uint16_t rnti,
                          uint16_t targetCellId)
  {
    double now = Simulator::Now().GetSeconds();
    std::cout << "now: " << now << "\t";
    std::cout << context
              << " eNB CellId " << cellid
              << ": start handover of UE with IMSI " << imsi
              << " RNTI " << rnti
              << " to CellId " << targetCellId
              << std::endl;
  }
  
  void
  NotifyHandoverEndOkEnb (std::string context,
                          uint64_t imsi,
                          uint16_t cellid,
                          uint16_t rnti)
  {
    double now = Simulator::Now().GetSeconds();
    std::cout << "now: " << now << "\t";
    std::cout << context
              << " eNB CellId " << cellid
              << ": completed handover of UE with IMSI " << imsi
              << " RNTI " << rnti
              << std::endl;
  }

AnimationInterface * pAnim = 0;
  
// class UEs_Info{
//   public:

//     void
//     set_imsi(int imsi){
//       if(imsi >= 0)
//         this->imsi = imsi;
//     }

//     void
//     set_Position(Vector position){
//       this->position = position;
//     }

//     /////TraceFunction
//     void
//     GetUeSinr(uint16_t cellId, uint16_t rnti, double rsrp, double sinr, uint8_t componentCarrierId){
//       double now = Simulator::Now().GetSeconds();
//       this->sinr = 10*log10(sinr);

//       cout << "GetUeSinr: " << now << "," << imsi << "," << this->sinr  << "," << position.x << "," <<position.y<< "," << connectenb << endl;
//     }

//     //When ue move change the ue position
//     void CourseChange1 ( Ptr<const MobilityModel> mobility)
//     {
//         Vector pos = mobility->GetPosition (); // Get position
//         std::cout << Simulator::Now ().GetSeconds() << ", pos=" << position << ", x=" << pos.x << ", y=" << pos.y << ", z=" << pos.z << std::endl;
//         position = pos ;
//     }

//     void setConnectenb(int c){
//       connectenb = c;
//     }

//     Vector get_Position(){
//       return position;
//     }

//     void set_output(std::ofstream *os){
//       this->os = os;
//     }
      
//   private:
//     int imsi = -1;
//     //int counter;
//     int connectenb;
//     //double distance = 0.0;
//     double sinr = 0.0;
//     double last_gt = 0.0 ;
//     Vector position ,enb_position;
//     std::ofstream *os;
// };

// static void 
// CourseChange (std::string context, Ptr<const MobilityModel> position)
// {
//   Vector pos = position->GetPosition ();
//   std::cout << Simulator::Now ().GetSeconds() << ", pos=" << position << ", x=" << pos.x << ", y=" << pos.y
//             << ", z=" << pos.z << std::endl;
// }

Ptr<LteHelper> lteHelper;
int *last_index;
void attachToClosestEnb(NodeContainer* ueNodes, NetDeviceContainer* ueLteDevs, NodeContainer* enbNodes, NetDeviceContainer* enbLteDevs, uint16_t numberOfUes, uint16_t numberOfEnbs){
  for(int i=0; i<numberOfUes; i++){
    Ptr<const MobilityModel> ueMobilityModel = ueNodes->Get(i)->GetObject<MobilityModel>();
    Vector pos_ue = ueMobilityModel->GetPosition ();
    std::cout << Simulator::Now ().GetSeconds() << ", ue_x=" << pos_ue.x << ", ue_y=" << pos_ue.y << std::endl;
    int index = -1;
    unsigned long long int min_distance = std::numeric_limits<int>::max();
    for(int j=0; j<numberOfEnbs; j++){
      Ptr<const MobilityModel> enbMobilityModel = enbNodes->Get(j)->GetObject<MobilityModel>();
      Vector pos_enb = enbMobilityModel->GetPosition ();
      // std::cout << Simulator::Now ().GetSeconds() << ", enb_x=" << pos_enb.x << ", enb_y=" << pos_enb.y << std::endl;
      if((pos_ue.x-pos_enb.x)*(pos_ue.x-pos_enb.x)+(pos_ue.y-pos_enb.y)*(pos_ue.y-pos_enb.y)<min_distance){
        min_distance = (pos_ue.x-pos_enb.x)*(pos_ue.x-pos_enb.x)+(pos_ue.y-pos_enb.y)*(pos_ue.y-pos_enb.y);
        index = j;
        // std::cout << "enb:" << index << ", distance: " << min_distance << endl << endl;
      }
    }
    if(last_index[i] != index){
      // uint16_t ueCellId = ueLteDevs->Get(i)->GetObject<LteUeNetDevice>()->GetRrc()->GetCellId ();
      // uint16_t enbCellId = enbLteDevs->Get(index)->GetObject<LteEnbNetDevice>()->GetCellId ();
      // cout << "\n\n====================\nsec: " << Simulator::Now ().GetSeconds() << ", ueCellId: " << ueCellId << ", last_index: " << enbCellId << ", index: " << index << endl;
      lteHelper->HandoverRequest(Seconds(0), ueLteDevs->Get(i), enbLteDevs->Get(last_index[i]), enbLteDevs->Get(index));
      last_index[i] = index;
    }
  }
}
  
int main (int argc, char *argv[])
{
  auto t1 = chrono::high_resolution_clock::now();
  // LogLevel logLevel = (LogLevel)(LOG_PREFIX_ALL | LOG_LEVEL_ALL);

  // LogComponentEnable ("LteHelper", logLevel);
  // LogComponentEnable ("EpcHelper", logLevel);
  // LogComponentEnable ("EpcEnbApplication", logLevel);
  // LogComponentEnable ("EpcMmeApplication", logLevel);
  // LogComponentEnable ("EpcPgwApplication", logLevel);
  // LogComponentEnable ("EpcSgwApplication", logLevel);
  // LogComponentEnable ("EpcX2", logLevel);

  // LogComponentEnable ("LteEnbRrc", logLevel);
  // LogComponentEnable ("LteEnbNetDevice", logLevel);
  // LogComponentEnable ("LteUeRrc", logLevel);
  // LogComponentEnable ("LteUeNetDevice", logLevel);
  // LogComponentEnable ("A2A4RsrqHandoverAlgorithm", logLevel);
  // LogComponentEnable ("A3RsrpHandoverAlgorithm", logLevel);

  uint16_t numberOfUes = 1;
  uint16_t numberOfEnbs = 3;
  double distance = 500.0; // m
  double speed = 20;       // m/s
  double simTime = 60; // 1500 m / 20 m/s = 75 secs
  double enbTxPowerDbm = 46.0;
  double interPacketInterval = 1000.0;
  // Ptr<LteUePhy> uephy;
  // Ptr<MobilityModel> ueMobilityModel;
  // UEs_Info * ues_info = (UEs_Info *)malloc(sizeof(UEs_Info)*numberOfUes);

  last_index = (int*)malloc(sizeof(int)*numberOfUes);
  for(int i=0; i<numberOfUes; i++){
    last_index[i] = -1;
  }

  std::string animFile = "lte_udp_test6.xml";
  // string TRACE_FILE = "scratch/test10.tcl";
  string TRACE_FILE = "scratch/oneUE.tcl";
  Ns2MobilityHelper ns2 = Ns2MobilityHelper(TRACE_FILE);

  AsciiTraceHelper ascii;
  MobilityHelper::EnableAsciiAll (ascii.CreateFileStream ("mobility-trace-example.mob"));

  // change some default attributes so that they are reasonable for
  // this scenario, but do this before processing command line
  // arguments, so that the user is allowed to override these settings
  Config::SetDefault ("ns3::UdpClient::Interval", TimeValue (MilliSeconds (10)));
  Config::SetDefault ("ns3::UdpClient::MaxPackets", UintegerValue (1000000));
  Config::SetDefault ("ns3::LteHelper::UseIdealRrc", BooleanValue (true));
  Config::SetDefault ("ns3::LteEnbMac::NumberOfRaPreambles", UintegerValue (20));
  Config::SetDefault ("ns3::LteEnbRrc::SrsPeriodicity", UintegerValue (320));

  // Command line arguments
  CommandLine cmd (__FILE__);
  cmd.AddValue ("simTime", "Total duration of the simulation (in seconds)", simTime);
  cmd.AddValue ("speed", "Speed of the UE (default = 20 m/s)", speed);
  cmd.AddValue ("enbTxPowerDbm", "TX power [dBm] used by HeNBs (default = 46.0)", enbTxPowerDbm);
  
  cmd.AddValue("numberOfNodes", "Number of eNodeBs + UE pairs", numberOfEnbs);
  cmd.AddValue("simTime", "Total duration of the simulation [s])", simTime);
  cmd.AddValue("distance", "Distance between eNBs [m]", distance);
  cmd.AddValue("interPacketInterval", "Inter packet interval [ms])", interPacketInterval);
  cmd.AddValue ("animFile",  "File Name for Animation Output", animFile);

  cmd.Parse (argc, argv);


  lteHelper = CreateObject<LteHelper> ();
  Ptr<PointToPointEpcHelper> epcHelper = CreateObject<PointToPointEpcHelper> ();
  
  lteHelper->SetEpcHelper (epcHelper);
  lteHelper->SetSchedulerType ("ns3::RrFfMacScheduler");

  lteHelper->SetHandoverAlgorithmType ("ns3::A2A4RsrqHandoverAlgorithm");
  lteHelper->SetHandoverAlgorithmAttribute ("ServingCellThreshold",
                                            UintegerValue (30));
  lteHelper->SetHandoverAlgorithmAttribute ("NeighbourCellOffset",
                                            UintegerValue (1));

  //  lteHelper->SetHandoverAlgorithmType ("ns3::A3RsrpHandoverAlgorithm");
  //  lteHelper->SetHandoverAlgorithmAttribute ("Hysteresis",
  //                                            DoubleValue (3.0));
  //  lteHelper->SetHandoverAlgorithmAttribute ("TimeToTrigger",
  //                                            TimeValue (MilliSeconds (256)));

  Ptr<Node> pgw = epcHelper->GetPgwNode ();

  // Create a single RemoteHost
  NodeContainer remoteHostContainer;
  remoteHostContainer.Create (1);
  Ptr<Node> remoteHost = remoteHostContainer.Get (0);
  InternetStackHelper internet;
  internet.Install (remoteHostContainer);

  // Create the Internet
  PointToPointHelper p2ph;
  p2ph.SetDeviceAttribute ("DataRate", DataRateValue (DataRate ("100Gb/s")));
  p2ph.SetDeviceAttribute ("Mtu", UintegerValue (1500));
  p2ph.SetChannelAttribute ("Delay", TimeValue (Seconds (0.010)));
  NetDeviceContainer internetDevices = p2ph.Install (pgw, remoteHost);
  Ipv4AddressHelper ipv4h;
  ipv4h.SetBase ("1.0.0.0", "255.0.0.0");
  Ipv4InterfaceContainer internetIpIfaces = ipv4h.Assign (internetDevices);
  Ipv4Address remoteHostAddr = internetIpIfaces.GetAddress (1);


  // Routing of the Internet Host (towards the LTE network)
  Ipv4StaticRoutingHelper ipv4RoutingHelper;
  Ptr<Ipv4StaticRouting> remoteHostStaticRouting = ipv4RoutingHelper.GetStaticRouting (remoteHost->GetObject<Ipv4> ());
  // interface 0 is localhost, 1 is the p2p device
  remoteHostStaticRouting->AddNetworkRouteTo (Ipv4Address ("7.0.0.0"), Ipv4Mask ("255.0.0.0"), 1);

  /*
    * Network topology:
    *
    *      |     + --------------------------------------------------------->
    *      |     UE
    *      |
    *      |               d                   d                   d
    *    y |     |-------------------x-------------------x-------------------
    *      |     |                 eNodeB              eNodeB
    *      |   d |
    *      |     |
    *      |     |                                             d = distance
    *            o (0, 0, 0)                                   y = yForUe
    */

  NodeContainer ueNodes;
  NodeContainer enbNodes;
  enbNodes.Create (numberOfEnbs);
  ueNodes.Create (numberOfUes);
  ns2.Install(ueNodes.Begin(), ueNodes.End());

  // Install Mobility Model in UE
  // MobilityHelper ueMobility;
  // ueMobility.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");
  // ueMobility.Install (ueNodes);
  // ueNodes.Get (0)->GetObject<MobilityModel> ()->SetPosition (Vector (583, 365, 0));
  // ueNodes.Get (0)->GetObject<ConstantVelocityMobilityModel> ()->SetVelocity (Vector (734/60, 585/60, 0));

  // Install Mobility Model in eNB
  Ptr<ListPositionAllocator> enbPositionAlloc = CreateObject<ListPositionAllocator> ();
  //  for (uint16_t i = 0; i < numberOfEnbs; i++)
  //    {
  //      Vector enbPosition (distance * (i + 1), distance, 0);
  //      enbPositionAlloc->Add (enbPosition);
  //    }
  enbPositionAlloc->Add(Vector(583, 365, 0));
  // enbPositionAlloc->Add(Vector(885, 338, 0));
  // enbPositionAlloc->Add(Vector(1187, 328, 0));
  // enbPositionAlloc->Add(Vector(1305, 322, 0));
  // enbPositionAlloc->Add(Vector(596, 703, 0));
  enbPositionAlloc->Add(Vector(895, 690, 0));
  // enbPositionAlloc->Add(Vector(1100, 682, 0));
  // enbPositionAlloc->Add(Vector(1317, 679, 0));
  // enbPositionAlloc->Add(Vector(602, 972, 0));
  // enbPositionAlloc->Add(Vector(908, 955, 0));
  // enbPositionAlloc->Add(Vector(1107, 953, 0));
  enbPositionAlloc->Add(Vector(1317, 950, 0));
  
  MobilityHelper enbMobility;
  enbMobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  enbMobility.SetPositionAllocator (enbPositionAlloc);
  enbMobility.Install (enbNodes);

  // Install LTE Devices in eNB and UEs
  Config::SetDefault ("ns3::LteEnbPhy::TxPower", DoubleValue (enbTxPowerDbm));
  NetDeviceContainer enbLteDevs = lteHelper->InstallEnbDevice (enbNodes);
  NetDeviceContainer ueLteDevs = lteHelper->InstallUeDevice (ueNodes);

  // Install the IP stack on the UEs
  internet.Install (ueNodes);
  Ipv4InterfaceContainer ueIpIfaces;
  ueIpIfaces = epcHelper->AssignUeIpv4Address (NetDeviceContainer (ueLteDevs));

  // Attach all UEs to the first eNodeB
  // for (uint16_t i = 0; i < numberOfUes; i++)
  //   {
  //     lteHelper->Attach (ueLteDevs.Get (i), enbLteDevs.Get (0));
  //   }

  uint16_t j = 0;
  for (uint16_t i = 0; i < numberOfUes; i++){  
    if (j < numberOfEnbs){
        lteHelper->Attach (ueLteDevs.Get(i), enbLteDevs.Get(j));
        last_index[i] = j;
        j++;
    }
    else{
        j = 0;
        lteHelper->Attach (ueLteDevs.Get(i), enbLteDevs.Get(j));
        last_index[i] = j;
    }   
  }

  // for(int i=0; i<numberOfUes; i++){
  //   Ptr<const MobilityModel> ueMobilityModel = ueNodes.Get(i)->GetObject<MobilityModel>();
  //   Vector pos_ue = ueMobilityModel->GetPosition ();
  //   int index = -1;
  //   unsigned long long int min_distance = std::numeric_limits<int>::max();
  //   for(int j=0; j<numberOfEnbs; j++){
  //     Ptr<const MobilityModel> enbMobilityModel = enbNodes.Get(j)->GetObject<MobilityModel>();
  //     Vector pos_enb = enbMobilityModel->GetPosition ();
  //     if((pos_ue.x-pos_enb.x)*(pos_ue.x-pos_enb.x)+(pos_ue.y-pos_enb.y)*(pos_ue.y-pos_enb.y)<min_distance){
  //       min_distance = (pos_ue.x-pos_enb.x)*(pos_ue.x-pos_enb.x)+(pos_ue.y-pos_enb.y)*(pos_ue.y-pos_enb.y);
  //       index = j;
  //       std::cout << "enb:" << index << ", distance: " << min_distance << std::endl;
  //     }
  //   }
  //   lteHelper->AttachToClosestEnb (ueLteDevs.Get(i), enbLteDevs.Get(index));
  // }


  NS_LOG_LOGIC ("setting up applications");

  // randomize a bit start times to avoid simulation artifacts
  // (e.g., buffer overflows due to packet transmissions happening
  // exactly at the same time)
  Ptr<UniformRandomVariable> startTimeSeconds = CreateObject<UniformRandomVariable> ();
  startTimeSeconds->SetAttribute ("Min", DoubleValue (0));
  startTimeSeconds->SetAttribute ("Max", DoubleValue (0.010));

  for (uint32_t u = 0; u < numberOfUes; ++u)
  {
      Ptr<Node> ue = ueNodes.Get (u);
      // Set the default gateway for the UE
      Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting (ue->GetObject<Ipv4> ());
      ueStaticRouting->SetDefaultRoute (epcHelper->GetUeDefaultGatewayAddress (), 1);
  }

  
//enter radio range support that carries data between UE and EnodeB
Ptr<EpcTft> tft = Create<EpcTft> ();
EpcTft::PacketFilter pf;
pf.localPortStart = 1234;
pf.localPortEnd = 1234;
tft->Add (pf);
lteHelper->ActivateDedicatedEpsBearer (ueLteDevs, EpsBearer (EpsBearer::NGBR_VIDEO_TCP_DEFAULT), tft);

uint16_t dlPort = 1234;
uint16_t ulPort = 2000;
  uint16_t otherPort = 3000;
  ApplicationContainer clientApps;
  ApplicationContainer serverApps;

// generate traffic request to remote server
  for (uint32_t u = 0; u < ueNodes.GetN (); ++u){
      ++ulPort;
      ++otherPort;
      PacketSinkHelper dlPacketSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), dlPort));
      PacketSinkHelper ulPacketSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), ulPort));
      PacketSinkHelper packetSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), otherPort));
      serverApps.Add (dlPacketSinkHelper.Install (ueNodes.Get(u)));
      serverApps.Add (ulPacketSinkHelper.Install (remoteHost));
      serverApps.Add (packetSinkHelper.Install (ueNodes.Get(u)));

      UdpClientHelper dlClient (ueIpIfaces.GetAddress (u), dlPort);
      dlClient.SetAttribute ("Interval", TimeValue (MilliSeconds(interPacketInterval)));
      dlClient.SetAttribute ("MaxPackets", UintegerValue(1000000));

      UdpClientHelper ulClient (remoteHostAddr, ulPort);
      ulClient.SetAttribute ("Interval", TimeValue (MilliSeconds(interPacketInterval)));
      ulClient.SetAttribute ("MaxPackets", UintegerValue(1000000));

      UdpClientHelper client (ueIpIfaces.GetAddress (u), otherPort);
      client.SetAttribute ("Interval", TimeValue (MilliSeconds(interPacketInterval)));
      client.SetAttribute ("MaxPackets", UintegerValue(1000000));

      clientApps.Add (dlClient.Install (remoteHost));
      clientApps.Add (ulClient.Install (ueNodes.Get(u)));
      if (u+1 < ueNodes.GetN ()){
          clientApps.Add (client.Install (ueNodes.Get(u+1)));
      }
      else
          {
          clientApps.Add (client.Install (ueNodes.Get(0)));
          }
}
  // Install and start applications on UEs and remote host
serverApps.Start (Seconds (1));
clientApps.Start (Seconds (1));



  // Add X2 interface
  lteHelper->AddX2Interface (enbNodes);

  // X2-based Handover
  //lteHelper->HandoverRequest (Seconds (0.100), ueLteDevs.Get (0), enbLteDevs.Get (0), enbLteDevs.Get (1));

  // Uncomment to enable PCAP tracing
  // p2ph.EnablePcapAll("lena-x2-handover-measures");
  p2ph.EnableAsciiAll (ascii.CreateFileStream ("serverpgw_trace.tr"));
  p2ph.EnablePcapAll("pgw-hostudp");

  lteHelper->EnablePhyTraces ();
  lteHelper->EnableMacTraces ();
  lteHelper->EnableRlcTraces ();
  lteHelper->EnablePdcpTraces ();
  Ptr<RadioBearerStatsCalculator> rlcStats = lteHelper->GetRlcStats ();
  rlcStats->SetAttribute ("EpochDuration", TimeValue (Seconds (1.0)));
  Ptr<RadioBearerStatsCalculator> pdcpStats = lteHelper->GetPdcpStats ();
  pdcpStats->SetAttribute ("EpochDuration", TimeValue (Seconds (1.0)));

  // connect custom trace sinks for RRC connection establishment and handover notification
  Config::Connect ("/NodeList/*/DeviceList/*/LteEnbRrc/ConnectionEstablished",
                    MakeCallback (&NotifyConnectionEstablishedEnb));
  Config::Connect ("/NodeList/*/DeviceList/*/LteUeRrc/ConnectionEstablished",
                    MakeCallback (&NotifyConnectionEstablishedUe));
  Config::Connect ("/NodeList/*/DeviceList/*/LteEnbRrc/HandoverStart",
                    MakeCallback (&NotifyHandoverStartEnb));
  Config::Connect ("/NodeList/*/DeviceList/*/LteUeRrc/HandoverStart",
                    MakeCallback (&NotifyHandoverStartUe));
  Config::Connect ("/NodeList/*/DeviceList/*/LteEnbRrc/HandoverEndOk",
                    MakeCallback (&NotifyHandoverEndOkEnb));
  Config::Connect ("/NodeList/*/DeviceList/*/LteUeRrc/HandoverEndOk",
                    MakeCallback (&NotifyHandoverEndOkUe));
  // Config::Connect ("/NodeList/*/$ns3::MobilityModel/CourseChange",
  //                 MakeCallback (&CourseChange));

  // Create the animation object and configure for specific output
  pAnim = new AnimationInterface (animFile.c_str ());
  // Provide the absolute path to the resource
  pAnim->SetMaxPktsPerTraceFile(99999999999999);

  // for(int i = 0; i < numberOfUes; i++){
  //   // uephy = ueLteDevs.Get (i)->GetObject<LteUeNetDevice> ()->GetPhy ();
  //   // ues_info[i].set_imsi(ueLteDevs.Get (i)->GetObject<LteUeNetDevice> ()->GetImsi ());

  //   ueMobilityModel = ueNodes.Get(i)->GetObject<MobilityModel>();
  //   ues_info[i].set_Position(ueMobilityModel->GetPosition());

  //   // uephy -> TraceConnectWithoutContext("ReportCurrentCellRsrpSinr", MakeCallback (&UEs_Info::GetUeSinr, &ues_info[i]));
  //   ueMobilityModel -> TraceConnectWithoutContext("CourseChange", MakeCallback (&UEs_Info::CourseChange1, &ues_info[i]));
  // }

  for(int i=0; i<simTime; i++){
    Simulator::Schedule (Seconds (i), attachToClosestEnb, &ueNodes, &ueLteDevs, &enbNodes, &enbLteDevs, numberOfUes, numberOfEnbs);
  }
  Simulator::Stop (Seconds (simTime));
  Simulator::Run ();

  // GtkConfigStore config;
  // config.ConfigureAttributes ();

  Simulator::Destroy ();
  delete pAnim;

  auto t2 = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<std::chrono::seconds>( t2 - t1 ).count();
  cout << "time spent: " << duration / 3600 << ":" << duration / 60 % 60 << ":" << duration % 60 <<endl;

  return 0;

}