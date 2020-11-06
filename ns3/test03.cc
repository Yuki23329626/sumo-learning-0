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

using namespace ns3;
using namespace std;

class UEs_Info{
    public:

        void
        set_imsi(int imsi){
            if(imsi >= 0)
                this->imsi = imsi;
        }

        void
        set_Position(Vector position){
            this->position = position;
        }

        /////TraceFunction
        void
        GetUeSinr(uint16_t cellId, uint16_t rnti, double rsrp, double sinr, uint8_t componentCarrierId){
          double now = Simulator::Now().GetSeconds();
          //bool print = false ;
          this->sinr = 10*log10(sinr);
          //if ( counter %1000 == 0 ){

          if (now >= 223 && now < 224 )
            *os1 <<now <<","<< imsi << "," << this->sinr  <<"," << position.x <<"," <<position.y<<","<< connectenb << endl;
        }

        //When ue move change the ue position
        void
        CourseChange1 ( Ptr<const MobilityModel> mobility)
        {
           Vector pos = mobility->GetPosition (); // Get position
           position = pos ;
        }

        /*void setCounter(int c){
          counter = c;
        }*/
        void setConnectenb(int c){
          connectenb = c;
        }

        Vector get_Position(){
            return position;
        }

        void set_output(std::ofstream *os1){
          this->os1 = os1;
        }
       
    private:
        int imsi = -1;
        //int counter;
        int connectenb;
        //double distance = 0.0;
        double sinr = 0.0;
        double last_gt = 0.0 ;
        Vector position ,enb_position;
        std::ofstream *os1;
};


int main (int argc, char *argv[])
{
  string traceFile = "scratch/test03.tcl";
 
  int    nodeNum = 300;
  int    bandwidth = 100;       //num of RB ,10MHz
  int    enbNum = 5;
  int    connected = 4;

  double duration = 269;         //50 seconds
  double eNbTxPower = 20 ;      //Transimission power in doubleBm

  Ptr<LteUePhy> uephy;
  Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();

  Ptr<MobilityModel> ueMobilityModel;

  AsciiTraceHelper asciiTraceHelper;
  std::ofstream outputfile1;
  std::ofstream outputfile2;
  std::ofstream outputfile3;
  std::ofstream outputfile4;

  //std::ofstream outputfile5;

  outputfile1.open("enb5_266s.csv");
  //outputfile5.open("enb2_306s.csv");

  outputfile1<<"Time_s,Imsi,Sinr,x,y,Cellid"<<endl;
  //outputfile5<<"Time_s,Imsi,Sinr,x,y,Cellid"<<endl;

  // Enable logging from the ns2 helper
  LogComponentEnable ("Ns2MobilityHelper",LOG_LEVEL_DEBUG);

  CommandLine cmd;
  cmd.AddValue ("traceFile", "Ns2 movement trace file", traceFile);
  cmd.AddValue ("nodeNum", "Number of nodes", nodeNum);
  cmd.AddValue ("duration", "Duration of Simulation", duration);
  cmd.Parse (argc,argv);

  // Set the default Configure
  Config::SetDefault ("ns3::LteEnbPhy::TxPower", DoubleValue (eNbTxPower));
  Config::SetDefault ("ns3::LteUePhy::TxPower", DoubleValue (20.0));                        //Transimt power in dbm
  Config::SetDefault ("ns3::LteUePhy::EnableUplinkPowerControl", BooleanValue (true));
  Config::SetDefault ("ns3::LteUePhy::EnableUplinkPowerControl", BooleanValue (true));
 
  Config::SetDefault ("ns3::LteEnbMac::NumberOfRaPreambles", UintegerValue(10));            //when too much ue will 
  Config::SetDefault ("ns3::LteEnbRrc::DefaultTransmissionMode",UintegerValue(2));          //MIMO Tx diversity

  //path loss model
  Config::SetDefault ("ns3::LteHelper::PathlossModel",StringValue("ns3::OkumuraHataPropagationLossModel"));
  Config::SetDefault ("ns3::OkumuraHataPropagationLossModel::Frequency", DoubleValue(2000000000)); 

  Config::SetDefault ("ns3::LteUePowerControl::ClosedLoop", BooleanValue (true));
  Config::SetDefault ("ns3::LteUePowerControl::AccumulationEnabled", BooleanValue (true));
  Config::SetDefault ("ns3::LteUePowerControl::Alpha", DoubleValue (1.0));
  Config::SetDefault ("ns3::LteEnbRrc::SrsPeriodicity", UintegerValue (320));              //he SRS periodicity in num TTIs

  lteHelper->SetEnbDeviceAttribute("DlBandwidth",UintegerValue(bandwidth));
  lteHelper->SetEnbDeviceAttribute("UlBandwidth",UintegerValue(bandwidth)); 
  //Set antenna type
  lteHelper->SetEnbAntennaModelType("ns3::IsotropicAntennaModel");
  lteHelper->SetEnbAntennaModelAttribute("Gain",DoubleValue(1.0));

  UEs_Info * ues_info = (UEs_Info *)malloc(sizeof(UEs_Info)*nodeNum);
  Ns2MobilityHelper ns2 = Ns2MobilityHelper (traceFile);

  // Create UE nodes.
  NodeContainer ueNode;
  ueNode.Create (nodeNum);

  //configure movements for each node, while reading trace file
  ns2.Install ();

  //Create enbNode
  NodeContainer enbNode;
  enbNode.Create (5);

  //Set enb position
  Ptr <ListPositionAllocator> enbPositionAlloc = CreateObject<ListPositionAllocator> ();
  enbPositionAlloc->Add (Vector(500.0,8.00,0));
  
  for (int i = 1 ; i < enbNum ; i++){
    Vector enbPosition ( 1000 * i +500 , 8, 0);
    enbPositionAlloc-> Add (enbPosition);
  }

  // Install mobility
  MobilityHelper enbMobility;
  enbMobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  enbMobility.SetPositionAllocator (enbPositionAlloc);
  enbMobility.Install (enbNode);

  //Install netdevice
  NetDeviceContainer enbDevs;
  NetDeviceContainer ueDevs;
  enbDevs = lteHelper->InstallEnbDevice (enbNode);
  ueDevs = lteHelper->InstallUeDevice(ueNode);

  Ptr<LtePhy> enbPhy= enbDevs.Get(connected)->GetObject<LteEnbNetDevice>()->GetPhy()->GetObject<LtePhy>();
  //enbPhy->CreateTxPowerSpectralDensity();

  // we install the IP stack on the UEs
  InternetStackHelper internet1;
  internet1.Install (ueNode);
 /* Ipv4InterfaceContainer ueIpIfaces;
  ueIpIfaces = epcHelper-> AssignUeIpv4Address(NetDeviceContainer (ueDevs));*/

  //Set the power density
  Ptr <SpectrumValue> psd = enbPhy->CreateTxPowerSpectralDensity();
  (*psd) = 4.14e-21 ;
  Ptr<LteSpectrumPhy> enbDlSpectrumPhy = enbDevs.Get(connected)->GetObject<LteEnbNetDevice> ()->GetPhy ()->GetDownlinkSpectrumPhy ()->GetObject<LteSpectrumPhy> ();
  enbDlSpectrumPhy -> SetNoisePowerSpectralDensity(psd);


  //*******change !!!!
  for (int i = 0; i < nodeNum ; i++){
    lteHelper->Attach(ueDevs.Get(i),enbDevs.Get(connected));
  }
  
  for(int i = 0; i < nodeNum; i++){
      // set position
      ueMobilityModel = ueNode.Get(i)->GetObject<MobilityModel>();
      ues_info[i].set_Position(ueMobilityModel->GetPosition());
      //ues_info[i].setCounter(0);
      ////*******change
      ues_info[i].setConnectenb(connected+1);
      ues_info[i].set_output(&outputfile1);

      // set imsi
      uephy = ueDevs.Get (i)->GetObject<LteUeNetDevice> ()->GetPhy ();
      //ueRrc = ueDevs.Get (i)->GetObject<LteUeNetDevice> ()->GetRrc ();
      ues_info[i].set_imsi(ueDevs.Get (i)->GetObject<LteUeNetDevice> ()->GetImsi());

      // Connect Sinr Report to class member fucntion.
      uephy -> TraceConnectWithoutContext("ReportCurrentCellRsrpSinr", MakeCallback (&UEs_Info::GetUeSinr, &ues_info[i]));
      ueMobilityModel -> TraceConnectWithoutContext("CourseChange", MakeCallback (&UEs_Info::CourseChange1, &ues_info[i]));
      
    }
  enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
  EpsBearer bearer (q);
  lteHelper->ActivateDataRadioBearer (ueDevs, bearer);
  
  Simulator::Stop (Seconds (duration));

  //lteHelper->EnableTraces ();
  Simulator::Run ();

  Simulator::Destroy ();
  return 0;
}