#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <string>
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

class UEs_Info
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

  void GetUeSinr(uint16_t cellId, uint16_t rnti, double rsrp, double sinr, uint8_t componentCarrierId)
  {
    double now = Simulator::Now().GetSeconds();

    this->sinr = 10 * log10(sinr);

    if (now >= 10 && now < 11){
      *os1 <<now <<","<< imsi << "," << this->sinr  <<"," << position.x <<"," <<position.y<<","<< connected_eNB << endl;
    }
  }

  void CourseChange1(Ptr<const MobilityModel> mobility)
  {
    Vector pos = mobility->GetPosition(); // Get position
    position = pos;
  }

  void setConnectedENB(int c)
  {
    connected_eNB = c;
  }

  Vector get_Position()
  {
    return position;
  }

  void set_output(std::ofstream *os1)
  {
    this->os1 = os1;
  }

private:
  int imsi = -1;
  int connected_eNB;
  double sinr = 0.0;
  Vector position, enb_position;
  std::ofstream *os1;
};

int main(int argc, char *argv[])
{
  string TRACE_FILE = "scratch/test04.tcl";

  const int NODE_NUM = 300;
  const int BANDWIDTH = 100;
  const int ENB_NUM = 35;

  int SELECTED_ENB = 1;

  const double DURATION = 60;
  const double ENB_TX_POWER = 20;

  Ptr<LteUePhy> uephy;
  Ptr<LteHelper> lteHelper = CreateObject<LteHelper>();

  Ptr<MobilityModel> ueMobilityModel;

  AsciiTraceHelper asciiTraceHelper;
  std::ofstream outputfile1;

  outputfile1.open("test04.csv");
  outputfile1 << "Time_sec,IMSI,SINR,X,Y,Selected_eNB" << endl;

  LogComponentEnable("Ns2MobilityHelper", LOG_LEVEL_DEBUG);

  CommandLine cmd;
  cmd.AddValue("traceFile", "Ns2 movement trace file", TRACE_FILE);
  cmd.AddValue("NODE_NUM", "Number of nodes", NODE_NUM);
  cmd.AddValue("duration", "Duration of Simulation", DURATION);
  cmd.Parse(argc, argv);

  Config::SetDefault("ns3::LteEnbPhy::TxPower", DoubleValue(ENB_TX_POWER));
  Config::SetDefault("ns3::LteUePhy::TxPower", DoubleValue(20.0));
  Config::SetDefault("ns3::LteUePhy::EnableUplinkPowerControl", BooleanValue(true));
  Config::SetDefault("ns3::LteUePhy::EnableUplinkPowerControl", BooleanValue(true));

  Config::SetDefault("ns3::LteEnbMac::NumberOfRaPreambles", UintegerValue(10));
  Config::SetDefault("ns3::LteEnbRrc::DefaultTransmissionMode", UintegerValue(2));

  Config::SetDefault("ns3::LteHelper::PathlossModel", StringValue("ns3::OkumuraHataPropagationLossModel"));
  Config::SetDefault("ns3::OkumuraHataPropagationLossModel::Frequency", DoubleValue(2000000000));

  Config::SetDefault("ns3::LteUePowerControl::ClosedLoop", BooleanValue(true));
  Config::SetDefault("ns3::LteUePowerControl::AccumulationEnabled", BooleanValue(true));
  Config::SetDefault("ns3::LteUePowerControl::Alpha", DoubleValue(1.0));
  Config::SetDefault("ns3::LteEnbRrc::SrsPeriodicity", UintegerValue(320));

  lteHelper->SetEnbDeviceAttribute("DlBandwidth", UintegerValue(BANDWIDTH));
  lteHelper->SetEnbDeviceAttribute("UlBandwidth", UintegerValue(BANDWIDTH));
  lteHelper->SetEnbAntennaModelType("ns3::IsotropicAntennaModel");
  lteHelper->SetEnbAntennaModelAttribute("Gain", DoubleValue(1.0));

  UEs_Info *ues_info = (UEs_Info *)malloc(sizeof(UEs_Info) * NODE_NUM);
  Ns2MobilityHelper ns2 = Ns2MobilityHelper(TRACE_FILE);

  NodeContainer ueNode;
  ueNode.Create(NODE_NUM);

  ns2.Install();

  NodeContainer enbNode;
  enbNode.Create(ENB_NUM);

  Ptr<ListPositionAllocator> enbPositionAlloc = CreateObject<ListPositionAllocator>();
  enbPositionAlloc->Add(Vector(500.0, 8.00, 0));
  
  for (int j = 1; j < 6; j++)
  {
    for (int i = 1; i < 8; i++)
    {
      Vector enbPosition(700 + 500*i, 1900 + 500*j, 0);
      enbPositionAlloc->Add(enbPosition);
    }
  }

  MobilityHelper enbMobility;
  enbMobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  enbMobility.SetPositionAllocator(enbPositionAlloc);
  enbMobility.Install(enbNode);

  NetDeviceContainer enbDevs;
  NetDeviceContainer ueDevs;
  enbDevs = lteHelper->InstallEnbDevice(enbNode);
  ueDevs = lteHelper->InstallUeDevice(ueNode);

  Ptr<LtePhy> enbPhy = enbDevs.Get(SELECTED_ENB)->GetObject<LteEnbNetDevice>()->GetPhy()->GetObject<LtePhy>();

  InternetStackHelper internet1;
  internet1.Install(ueNode);

  Ptr<SpectrumValue> psd = enbPhy->CreateTxPowerSpectralDensity();
  (*psd) = 4.14e-21;
  Ptr<LteSpectrumPhy> enbDlSpectrumPhy = enbDevs.Get(SELECTED_ENB)->GetObject<LteEnbNetDevice>()->GetPhy()->GetDownlinkSpectrumPhy()->GetObject<LteSpectrumPhy>();
  enbDlSpectrumPhy->SetNoisePowerSpectralDensity(psd);

  for (int i = 0; i < NODE_NUM; i++)
  {
    lteHelper->Attach(ueDevs.Get(i), enbDevs.Get(SELECTED_ENB));
  }

  for (int i = 0; i < NODE_NUM; i++)
  {
    ueMobilityModel = ueNode.Get(i)->GetObject<MobilityModel>();
    ues_info[i].set_Position(ueMobilityModel->GetPosition());

    ues_info[i].setConnectedENB(SELECTED_ENB + 1);
    ues_info[i].set_output(&outputfile1);

    uephy = ueDevs.Get(i)->GetObject<LteUeNetDevice>()->GetPhy();
    ues_info[i].set_imsi(ueDevs.Get(i)->GetObject<LteUeNetDevice>()->GetImsi());

    uephy->TraceConnectWithoutContext("ReportCurrentCellRsrpSinr", MakeCallback(&UEs_Info::GetUeSinr, &ues_info[i]));
    ueMobilityModel->TraceConnectWithoutContext("CourseChange", MakeCallback(&UEs_Info::CourseChange1, &ues_info[i]));
  }

  enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
  EpsBearer bearer(q);
  lteHelper->ActivateDataRadioBearer(ueDevs, bearer);

  Simulator::Stop(Seconds(DURATION));
  Simulator::Run();
  Simulator::Destroy();
  return 0;
}