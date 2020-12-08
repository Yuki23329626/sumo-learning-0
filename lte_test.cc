/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 */

#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/mobility-module.h>
#include <ns3/lte-module.h>

// Default Network Topology
//
//   Wifi 10.1.3.0
//                 AP
//  *    *    *    *
//  |    |    |    |    10.1.1.0
// n5   n6   n7   n0 -------------- n1   n2   n3   n4
//                   point-to-point  |    |    |    |
//                                   ================
//                                     LAN 10.1.2.0
 
using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("ThirdScriptExample");

int main (int argc, char *argv[])
{
    Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();

    NodeContainer enbNodes;
    enbNodes.Create (1);
    NodeContainer ueNodes;
    ueNodes.Create (2);

    MobilityHelper mobility;
    mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
    mobility.Install (enbNodes);
    mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
    mobility.Install (ueNodes);

    NetDeviceContainer enbDevs;
    enbDevs = lteHelper->InstallEnbDevice (enbNodes);
    NetDeviceContainer ueDevs;
    ueDevs = lteHelper->InstallUeDevice (ueNodes);

    lteHelper->Attach (ueDevs, enbDevs.Get (0));

    enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
    EpsBearer bearer (q);
    lteHelper->ActivateDataRadioBearer (ueDevs, bearer);

    Simulator::Stop (Seconds (0.005));

    Simulator::Run ();
    Simulator::Destroy ();
    return 0;
}
