

/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 The university of Utah
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
 * Author: Binh Nguyen <binh@cs.utah.edu>
 */

/*
 *  Brief: simplest lte experiment, constant radio link bandwidth and delay, no error, no HARQ.
 *  Topology:
 *  
 *              UE-------------------EnodeB---------------------SPWG-------------------Endhost
 *                    link_0                    link_1 (S1uLink)         link_2 (P2pLink)
 *  link_0: 5ms, 1Mbps, no error, no HARQ.
 *  link_1, link_2: 30ms, 1Gbps, no error.
 */

#include "ns3/lte-helper.h"
#include "ns3/epc-helper.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/lte-module.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/point-to-point-module.h"
#include "ns3/config-store.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include "ns3/flow-monitor-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/lte-global-pathloss-database.h"


#include <map>

using namespace ns3;
NS_LOG_COMPONENT_DEFINE ("lte-dl-simplest");

/************* typedef ****************/
#define kilo 1000
#define KILO 1024
#define ONEBIL kilo*kilo*kilo
#define PUT_SAMPLING_INTERVAL 0.010    /*sample TCP thoughput for each 50ms*/


/******** Nodes and devs***********/
static Ptr<Node> remote_host_node;
static Ptr<Node> p_gateway_node;
static NodeContainer ueNodes;
static NodeContainer enbNodes;
static NetDeviceContainer p2p_net_devs;
static Ptr<PointToPointNetDevice> SPGW_dev;
static Ptr<PointToPointNetDevice> remote_host_dev;
static NetDeviceContainer enbLteDevs;
static NetDeviceContainer ueLteDevs;
static Ipv4Address remoteHostAddr;
static Ipv4Address SPGW_address;



/**************** Helpers, Global variables **************/
static Ptr<ns3::FlowMonitor> monitor;
static Ptr<ns3::Ipv4FlowClassifier> classifier;
static FlowMonitorHelper flowHelper;
static std::map <FlowId, FlowMonitor::FlowStats> stats;
static InternetStackHelper internet_helper;
static PointToPointHelper p2ph;
static Ptr<LteHelper> lteHelper;
static Ptr<EpcHelper>  epcHelper;
static Ipv4StaticRoutingHelper ipv4RoutingHelper;
static AsciiTraceHelper asciiTraceHelper;
static Ipv4AddressHelper ipv4h;  //Ipv4AddressHelper is used to assign Ip Address for a typical node.
static Ipv4InterfaceContainer ue_ip_ifaces;

static ApplicationContainer clientApps;
static ApplicationContainer serverApps;
    

static CommandLine cmd;
static ConfigStore inputConfig;
static ConfigStore outputConfig;
// static double client_start = 0.6;
static uint16_t dlPort = 2000;
static double last_tx_time = 0;
static double last_rx_time = 0;
static double last_tx_bytes = 0;
static double last_rx_bytes = 0;
static double last_rx_pkts = 0;
static double tcp_delay = 0;
static double last_delay_sum = 0;


/**sending flowS stats***/
static std::map<Ipv4Address, double> meanTxRate_send;
static std::map<Ipv4Address, double> meanRxRate_send;
static std::map<Ipv4Address, double> meanTcpDelay_send;
static std::map<Ipv4Address, uint64_t> numOfLostPackets_send;
static std::map<Ipv4Address, uint64_t> numOfTxPacket_send;
/**acking flowS stats**/
static std::map<Ipv4Address, double> meanTxRate_ack;
static std::map<Ipv4Address, double> meanRxRate_ack;
static std::map<Ipv4Address, double> meanTcpDelay_ack;
static std::map<Ipv4Address, uint64_t> numOfLostPackets_ack;
static std::map<Ipv4Address, uint64_t> numOfTxPacket_ack;

// /******* Topology specs ************/
static uint16_t numberOfUeNodes = 1;
static uint16_t numberOfEnodebs = 1;
//S1uLink
static std::string s1uLinkDataRate = "1Gb/s";
static double  s1uLinkDelay = 0.015;
static uint16_t s1uLinkMtu = 1500;
//p2pLink
static std::string p2pLinkDataRate = "1Gb/s";
static double p2pLinkDelay = 0.015;
static uint16_t p2pLinkMtu = 1500;
/**********************************/

/*********Simulation paras**********/
static double simTime = 100;  //simulation time for EACH application
static std::string pcapName = "lte-dl-simplest";

/***************LTE specifications********/
static uint16_t isAMRLC = 0;    
static double distance = 100.0;
static uint16_t radioUlBandwidth = 100;  //the radio link bandwidth among UEs and EnodeB (in Resource Blocks). This is the configuration on LteEnbDevice.
static uint16_t radioDlBandwidth = 100;  //same as above, for downlink.
    

/***************Application specs*********/
static uint32_t numberOfPackets = 0;
static uint32_t packetSize = 900;
static std::string dataRate = "100Mb/s";
static uint16_t isTcp = 1;


/********* Ascii output files name *********/
static std::string DIR = "/Users/binh/Documents/workspace/lena/results/tcp/data-scripts/radio/";
static std::string cwnd = DIR+"cwnd.txt";
static std::string rto = DIR+"rto_value_tmp.txt";
static std::string rwnd = DIR+"rwnd_tmp.txt";
static std::string rtt = DIR+"last_rtt_sample_tmp.txt";
static std::string highesttxseq = DIR+"highest_tx_seq.txt";
static std::string nexttxseq = DIR+"next_tx_seq.txt";
static std::string queues = DIR+"queues.txt";
static std::string macro = DIR+"macro_output.txt";
static std::string put_send;
static std::string put_ack;
static std::string debugger = "debugger.txt";

/********wrappers**********/
Ptr<OutputStreamWrapper> cwnd_wp;
Ptr<OutputStreamWrapper> rto_wp;
Ptr<OutputStreamWrapper> rwnd_wp;
Ptr<OutputStreamWrapper> highest_tx_seq_wp;
Ptr<OutputStreamWrapper> next_tx_seq_wp;
Ptr<OutputStreamWrapper> rtt_wp;
Ptr<OutputStreamWrapper> dev_queues_wp;
Ptr<OutputStreamWrapper> put_send_wp;
Ptr<OutputStreamWrapper> put_ack_wp;
Ptr<OutputStreamWrapper> macro_wp;
Ptr<OutputStreamWrapper> debugger_wp;


double
CalculateAverageDelay(std::map <uint64_t, uint32_t> delayArray){
  double sum = 0;
  uint64_t counter = 0;
  for (std::map<uint64_t, uint32_t>::iterator ii = delayArray.begin(); ii != delayArray.end(); ++ii){
    sum += (*ii).second;
    counter++;
  }
  return (sum/counter);
}

static void 
CwndTracer (Ptr<OutputStreamWrapper> cwnd_wp, uint32_t oldval, uint32_t newval)
{
  // NS_LOG_UNCOND (Simulator::Now().GetSeconds() << " cwnd_from " << oldval << " to " << newval);
  *cwnd_wp->GetStream() << Simulator::Now().GetSeconds() << " cwnd_from " << oldval << " to " << newval << std::endl;
}

static void 
RTOTracer (Ptr<OutputStreamWrapper> rto_wp, ns3::Time oldval, ns3::Time newval)
{
  // NS_LOG_UNCOND (Simulator::Now().GetSeconds() << " \t RTO_value \t " << newval.GetSeconds());
  *rto_wp->GetStream() << Simulator::Now().GetSeconds() << " \t RTO_value \t " << newval.GetSeconds() << std::endl;
}

static void 
LastRttTracer (Ptr<OutputStreamWrapper> rtt_wp, ns3::Time oldval, ns3::Time newval)
{
  // NS_LOG_UNCOND (Simulator::Now().GetSeconds() << " \t last_rtt_sample \t " << newval.GetSeconds());
  *rtt_wp->GetStream() << Simulator::Now().GetSeconds() << " \t last_rtt_sample \t " << newval.GetSeconds() << std::endl;

}


static void 
HighestSentSeqTracer (Ptr<OutputStreamWrapper> highest_tx_seq_wp, ns3::SequenceNumber32 oldval, ns3::SequenceNumber32 newval)
{
  // NS_LOG_UNCOND (Simulator::Now().GetSeconds() << " \t highest_sent_seq \t " << newval);
  *highest_tx_seq_wp->GetStream() << Simulator::Now().GetSeconds() << " \t highest_sent_seq \t " << newval << std::endl;
}


static void 
NextTxSeqTracer (Ptr<OutputStreamWrapper> next_tx_seq_wp, ns3::SequenceNumber32 oldval, ns3::SequenceNumber32 newval)
{
  // NS_LOG_UNCOND (Simulator::Now().GetSeconds() << " \t next_tx_seq \t " << newval);
  *next_tx_seq_wp->GetStream() << Simulator::Now().GetSeconds() << " \t next_tx_seq \t " << newval << std::endl;
}



void
getTcpPut(){
  monitor->CheckForLostPackets();
  classifier = DynamicCast<ns3::Ipv4FlowClassifier> (flowHelper.GetClassifier());
  stats = monitor->GetFlowStats();

    /*==============Get flows information============*/
  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator iter = stats.begin(); iter != stats.end(); ++iter){
    ns3::Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(iter->first);

    /*sending flows, from endhost (1.0.0.2:49153) to Ues (7.0.0.2:200x)*/
    if (t.destinationPort >= 2000 && t.destinationPort <= 3000) {
        if (iter->second.rxPackets > 1){
          if (last_tx_time < iter->second.timeLastTxPacket.GetDouble()){
              meanTxRate_send[t.destinationAddress] = 8*(iter->second.txBytes-last_tx_bytes)/(iter->second.timeLastTxPacket.GetDouble()-last_tx_time)*ONEBIL/kilo;
              meanRxRate_send[t.destinationAddress] = 8*(iter->second.rxBytes-last_rx_bytes)/(iter->second.timeLastRxPacket.GetDouble()-last_rx_time)*ONEBIL/kilo;
              last_tx_time = iter->second.timeLastTxPacket.GetDouble();
              last_tx_bytes = iter->second.txBytes;
              last_rx_time = iter->second.timeLastRxPacket.GetDouble();
              last_rx_bytes = iter->second.rxBytes;
          }
          if (iter->second.rxPackets > last_rx_pkts){
              meanTcpDelay_send[t.destinationAddress] = iter->second.delaySum.GetDouble()/iter->second.rxPackets/1000000;
              tcp_delay = (iter->second.delaySum.GetDouble() - last_delay_sum) / (iter->second.rxPackets - last_rx_pkts)/(kilo*kilo);
              last_delay_sum = iter->second.delaySum.GetDouble();
              last_rx_pkts = iter->second.rxPackets;
          }
        }
    }

    /*ack flow, from Ues (7.0.0.2:200x) to endhost (1.0.0.2:49153)*/
    if (t.destinationPort >= 49153){
      if (iter->second.rxPackets > 1){
        meanTxRate_ack[t.sourceAddress] = 8*iter->second.txBytes/(iter->second.timeLastTxPacket.GetDouble()-iter->second.timeFirstTxPacket.GetDouble())*ONEBIL/(1024);
        meanRxRate_ack[t.sourceAddress] = 8*iter->second.rxBytes/(iter->second.timeLastRxPacket.GetDouble()-iter->second.timeFirstRxPacket.GetDouble())*ONEBIL/(1024);
        meanTcpDelay_ack[t.sourceAddress] = iter->second.delaySum.GetDouble()/iter->second.rxPackets/1000000;
      }
      numOfLostPackets_ack[t.sourceAddress] = iter->second.lostPackets;
      numOfTxPacket_ack[t.sourceAddress] = iter->second.txPackets;
     }
    }

  
    std::map<Ipv4Address,double>::iterator it1 = meanRxRate_send.begin();
    std::map<Ipv4Address,double>::iterator it2 = meanTcpDelay_send.begin();
    std::map<Ipv4Address,uint64_t>::iterator it3 = numOfLostPackets_send.begin();
    std::map<Ipv4Address,uint64_t>::iterator it4 = numOfTxPacket_send.begin();
    std::map<Ipv4Address,double>::iterator it5 = meanTxRate_send.begin();

    std::cout << "xxx\n";

    for (;it1 != meanRxRate_send.end(); ){
      *put_send_wp->GetStream() << Simulator::Now().GetMilliSeconds() << "\t\t"
      << (*it1).first << "\t\t"
      << (*it1).second << "\t\t"
      << (*it2).second << "\t\t"
      << (*it3).second << "\t\t"
      << (*it4).second << "\t\t"
      << (*it5).second << "\n";
      ++it1;
      ++it2;
      ++it3;
      ++it4;
      ++it5;
      std::cout << "yyyyyyyy\n";
    }

    // while (client_start < simTime){
    //     std::cout << "client_start=" << client_start << "PUT_SAMPLING_INTERVAL=" << PUT_SAMPLING_INTERVAL << std::endl;
    //     client_start += PUT_SAMPLING_INTERVAL;
    //     Simulator::Schedule(Seconds(client_start),&getTcpPut);
    // }
}

void init_log_and_cmd(int argc, char *argv[]){
  //*************Enable logs********************/
  // LogLevel level = (LogLevel) (LOG_LEVEL_ALL | LOG_PREFIX_TIME | LOG_PREFIX_NODE | LOG_PREFIX_FUNC);

  //To enable all components inside the LTE module.
  //      lteHelper->EnableLogComponents();
   //  LogComponentEnable("UdpEchoClientApplication",LOG_LEVEL_INFO);
  //  LogComponentEnable("UdpEchoClientApplication",LOG_PREFIX_ALL);
  // LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);
  // LogComponentEnable("UdpClient",LOG_LEVEL_INFO);
  // LogComponentEnable("UdpServer", LOG_LEVEL_INFO);
     // LogComponentEnable("OnOffApplication",level);
  //    LogComponentEnable("PacketSink",LOG_LEVEL_INFO);
   // LogComponentEnable("TcpTahoe", level);
   // LogComponentEnable("TcpTahoe",level);
   // LogComponentEnable("RttEstimator",level);
   // LogComponentEnable("TcpSocketBase",level);

  //        LogComponentEnable("OnOffApplication",level);
  //        LogComponentEnable("PacketSink",level);
  //       LogComponentEnable ("LteRlcUm", level);
    // LogComponentEnable ("LteHelper",level);
  //       LogComponentEnable ("LteUeMac", level);
  //   LogComponentEnable ("LteEnbMac", level);
  //       LogComponentEnable ("LtePdcp", level);
  //       LogComponentEnable ("LtePhy", level);

     
    
    // Command line arguments
    cmd.AddValue("numberOfUeNodes", "Number of UeNodes", numberOfUeNodes);
    cmd.AddValue("numberOfEnodebs", "Number of eNodebs", numberOfEnodebs);
    cmd.AddValue("s1uLinkDataRate", "S1u Link Data Rate", s1uLinkDataRate);
    cmd.AddValue("s1uLinkDelay", "S1u Link Delay", s1uLinkDelay);
    cmd.AddValue("s1uLinkMtu", "S1u Link Mtu", s1uLinkMtu);
    cmd.AddValue("p2pLinkDataRate", "p2p Link Data Rate", p2pLinkDataRate);
    cmd.AddValue("p2pLinkDelay", "p2p Link Delay", p2pLinkDelay);
    cmd.AddValue("p2pLinkMtu", "p2p Link Mtu", p2pLinkMtu);

    cmd.AddValue("simTime", "Total duration of the simulation [s])", simTime);
    cmd.AddValue("numberOfPackets", "Number of packets to send", numberOfPackets);
    cmd.AddValue("packetSize", "Size of each packet", packetSize);
    cmd.AddValue("isAMRLC", "Whether using AM RLC (UM RLC if false)", isAMRLC);
    cmd.AddValue("dataRate", "TCP application data rate", dataRate);
    cmd.AddValue("isTcp", "TCP application if true, Udp if false", isTcp);
    
    
    cmd.AddValue("radioUlBandwidth", "Uplink radio bandwidth [RBs] (6,15,25,50,75,100)", radioUlBandwidth);
    cmd.AddValue("distance", "Distance between eNBs [m]", distance);
    cmd.AddValue("radioDlBandwidth", "Downlink radio bandwidth [RBs] (6,15,25,50,75,100)", radioDlBandwidth);

    cmd.AddValue("pcapName", "Prefix of pcap files", pcapName);



    /**ConfigStore setting*/
    Config::SetDefault("ns3::ConfigStore::Filename", StringValue("lte-dl-simplest-config-in.txt"));
    Config::SetDefault("ns3::ConfigStore::FileFormat", StringValue("RawText"));
    Config::SetDefault("ns3::ConfigStore::Mode", StringValue("Load"));
    inputConfig.ConfigureDefaults();
    inputConfig.ConfigureAttributes();

    cmd.Parse(argc, argv);
}


void 
p2p_setup(){
    /**********************************1.Create nodes*******************************/
    enbNodes.Create(numberOfEnodebs);
    ueNodes.Create(numberOfUeNodes);
    NodeContainer remoteHostContainer;
    remoteHostContainer.Create (1);
    remote_host_node = remoteHostContainer.Get (0);


    //************lteHeper, epcHelper**************//
    lteHelper = CreateObject<LteHelper> ();
    epcHelper = CreateObject<EpcHelper> ();
    lteHelper->SetEpcHelper (epcHelper);
    lteHelper->SetSchedulerType("ns3::PfFfMacScheduler");    
    p_gateway_node = epcHelper->GetPgwNode ();


    //*********************Set S1u link parameters (link between EnodeB and SPGW)********************//
    epcHelper->SetAttribute("S1uLinkDataRate", DataRateValue (DataRate (s1uLinkDataRate)));
    epcHelper->SetAttribute("S1uLinkDelay", TimeValue (Seconds (s1uLinkDelay)));
    epcHelper->SetAttribute("S1uLinkMtu", UintegerValue (s1uLinkMtu));
    
    
    //***************Set parameters for link between SPGW and remote_host_node*****************//
    p2ph.SetDeviceAttribute ("DataRate", DataRateValue (DataRate (p2pLinkDataRate)));
    p2ph.SetDeviceAttribute ("Mtu", UintegerValue (p2pLinkMtu));
    p2ph.SetChannelAttribute ("Delay", TimeValue (Seconds (p2pLinkDelay)));
    p2p_net_devs = p2ph.Install (p_gateway_node, remote_host_node);   //The interfaces between the SPGW and remoteHost were saved in p2p_net_devs.
    SPGW_dev = p2p_net_devs.Get(0)->GetObject<ns3::PointToPointNetDevice>();
    remote_host_dev = p2p_net_devs.Get(1)->GetObject<ns3::PointToPointNetDevice>();
    

    //***********Install Internet stack on edge nodes*************//
    internet_helper.Install (remote_host_node);
    
    
    // Create the Internet
    ipv4h.SetBase ("1.0.0.0", "255.0.0.0");
    Ipv4InterfaceContainer internetIpIfaces = ipv4h.Assign (p2p_net_devs);    //assign IP addresses in starting at "1.0.0.0" to the SPGW and remoteHost.
    remoteHostAddr = internetIpIfaces.GetAddress (1);
    SPGW_address = internetIpIfaces.GetAddress(0);
    *debugger_wp->GetStream() << "remoteHostAddr=" << remoteHostAddr << "SPGW_address=" << SPGW_address << std::endl;
    
    
    //***************************Let's the remoteHost know how to route to UE "7.0.0.0"**************************//
    //get the static routing method to the remoteHost. The parameter for GetStaticRouting() is the Ptr<Ipv4> of the destination.
    Ptr<Ipv4StaticRouting> remoteHostStaticRouting = ipv4RoutingHelper.GetStaticRouting (remote_host_node->GetObject<Ipv4> ()); //remoteHostStaticRouting now knows how to route to the remoteHost.
    //Add the routing entry to the remoteHostStaticRouting table.
    //UEs subnet is "7.0.0.0"
    remoteHostStaticRouting->AddNetworkRouteTo (Ipv4Address ("7.0.0.0"), Ipv4Mask ("255.0.0.0"), 1);  
    
}

void
ran_setup(){
    //=============================Install mobility model for UE nodes and EnodeB nodes=================//
    MobilityHelper enbMobility;
    enbMobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
    enbMobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                      "MinX", DoubleValue (0.0),  //zero point
                                      "MinY", DoubleValue (0.0),  //zero point
                                      "DeltaX", DoubleValue (10000.0),  //distance among ENB nodes
                                      "DeltaY", DoubleValue (10000.0),
                                      "GridWidth", UintegerValue (3), //number of nodes on a line
                                      "LayoutType", StringValue ("RowFirst"));
    enbMobility.Install (enbNodes); /*===ENB #1 placed at (0.0)====*/
    enbLteDevs = lteHelper->InstallEnbDevice (enbNodes);


    
    NetDeviceContainer::Iterator enbLteDevIt = enbLteDevs.Begin ();
    Vector enbPosition = (*enbLteDevIt)->GetNode ()->GetObject<MobilityModel> ()->GetPosition ();
    MobilityHelper ueMobility;
    ueMobility.SetPositionAllocator ("ns3::UniformDiscPositionAllocator", //nodes are put randomly inside a circle with the central point is (x,y).
                                     "X", DoubleValue (enbPosition.x),
                                     "Y", DoubleValue (enbPosition.y),
                                     "rho", DoubleValue (distance));  //radius of the circle.

    ueMobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
    ueMobility.Install (ueNodes);
    ueLteDevs = lteHelper->InstallUeDevice (ueNodes); 

    //**********************Assign Ipv4 addresses for UEs. Install the IP stack on the UEs******************//
    // Assign IP address to UEs, and install applications
    internet_helper.Install (ueNodes);  //internet (InternetStackHelper) again be used to install an Internet stack for a node.
    ue_ip_ifaces = epcHelper->AssignUeIpv4Address (NetDeviceContainer (ueLteDevs));
    // Add routing entries to routing table.
    for (uint32_t u = 0; u < ueNodes.GetN (); ++u)
    {
        Ptr<Node> ueNode = ueNodes.Get (u);
        // Set the default gateway for the UE
        Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting (ueNode->GetObject<Ipv4> ());
        ueStaticRouting->SetDefaultRoute (epcHelper->GetUeDefaultGatewayAddress (), 1);
    }

    //**********************Attach all UEs to eNodeB**********************//
    for (uint16_t i = 0; i < numberOfEnodebs; i++)
    {
        for (uint16_t t = 0; t < numberOfUeNodes; ++t){
            lteHelper->Attach (ueLteDevs.Get(t), enbLteDevs.Get(i));    //Attach function takes Interfaces as parameters.
        }
    }
    *debugger_wp->GetStream() << "UEaddress=" << ue_ip_ifaces.GetAddress(0) << std::endl;
}

void
install_apps_on_nodes(){
    //*****************************Install and start applications on UEs and remote host****************************//
    for (uint32_t u = 0; u < ueNodes.GetN (); ++u)
    {
        ++dlPort;       //each Ue will contact with the remoteHost by a different dlPort (the remoteHost needs this)        

        if (isTcp == 1){
          /*********TCP Application********/
          //Create a packet sink to receive packet on remoteHost
          PacketSinkHelper sink("ns3::TcpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), dlPort));
          serverApps.Add(sink.Install(ueNodes.Get(u)));

          OnOffHelper onOffHelper("ns3::TcpSocketFactory", Address ( InetSocketAddress(ue_ip_ifaces.GetAddress(u) , dlPort) ));
          onOffHelper.SetConstantRate( DataRate(dataRate), packetSize );
          if (numberOfPackets != 0)
            onOffHelper.SetAttribute("MaxBytes",UintegerValue(packetSize*numberOfPackets));
          clientApps.Add(onOffHelper.Install(remote_host_node));
        }
        else{
          /*********UDP Application********/
          //Create a packet sink to receive packet on remoteHost
          PacketSinkHelper sink("ns3::UdpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), dlPort));
          serverApps.Add(sink.Install(ueNodes.Get(u)));

          OnOffHelper onOffHelper("ns3::UdpSocketFactory", Address ( InetSocketAddress(ue_ip_ifaces.GetAddress(u), dlPort) ));
          onOffHelper.SetConstantRate( DataRate(dataRate), packetSize );
          if (numberOfPackets != 0)
            onOffHelper.SetAttribute("MaxBytes",UintegerValue(packetSize*numberOfPackets));
          clientApps.Add(onOffHelper.Install(remote_host_node));
        }
    }
}

void 
init_wrappers(){

    /* create files for wrappers */
    cwnd_wp = asciiTraceHelper.CreateFileStream(cwnd);
    rto_wp = asciiTraceHelper.CreateFileStream(rto);
    rtt_wp = asciiTraceHelper.CreateFileStream(rtt);
    rtt_wp = asciiTraceHelper.CreateFileStream(rtt);
    highest_tx_seq_wp = asciiTraceHelper.CreateFileStream(highesttxseq);
    next_tx_seq_wp = asciiTraceHelper.CreateFileStream(nexttxseq);
    dev_queues_wp = asciiTraceHelper.CreateFileStream(queues);
    macro_wp = asciiTraceHelper.CreateFileStream(macro);
    debugger_wp = asciiTraceHelper.CreateFileStream(debugger);

    //********************Initialize wrappers*********************/
    if (isTcp==1){
      put_send = "TCP_PUT.txt";
      put_ack = "TCP_PUT_ACK.txt";
    } else{
      put_send = "UDP_PUT.txt";
      put_ack = "UDP_PUT_ACK.txt";
    }
    put_send_wp = asciiTraceHelper.CreateFileStream(put_send);
    put_ack_wp = asciiTraceHelper.CreateFileStream(put_ack);

    *put_send_wp->GetStream() << "#DestinationIp\t"
                  << "Time\t"
                  << "Send Tcp throughput\t"
                  << "Send Tcp delay\t"
                  << "Number of Lost Pkts\t"
                  << "Number of Tx Pkts\t"
                  << "ErrorUlTx\t"
                  << "ErrorDlTx\t"
                  << "HarqUlTx\t"
                  << "HarqDlTx\n";

    *put_ack_wp->GetStream() << "#ScrAdd\t"
                  << "Time\t"
                  << "Ack Tcp throughput\t"
                  << "Ack Tcp delay\t"
                  << "Number of Lost Pkts\t"
                  << "Number of Tx Pkts\t"
                  << "ErrorUlTx\t"
                  << "ErrorDlTx\t"
                  << "HarqUlTx\t"
                  << "HarqDlTx\n";

}

void 
print_final_outputs(){
    
  *macro_wp->GetStream() << "*************DL experiment**************"
  << "\nNumberofUeNodes = " << numberOfUeNodes
  << "\nNumberOfEnodeBs = " << numberOfEnodebs
  << "\nDistance = " << distance
  << "\nPacket size = " << packetSize
  << "\nRate = " << dataRate
  << "\nNumber of Packets = " << numberOfPackets
  << "\nS1uLink " << s1uLinkDataRate << " " << s1uLinkDelay
  << "\np2pLink " << p2pLinkDataRate << " " << p2pLinkDelay 
  << std::endl;
    
  monitor->CheckForLostPackets();
  classifier = DynamicCast<ns3::Ipv4FlowClassifier> (flowHelper.GetClassifier());
  stats = monitor->GetFlowStats();

  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator iter = stats.begin(); iter != stats.end(); ++iter){
    ns3::Ipv4FlowClassifier::FiveTuple tuple = classifier->FindFlow(iter->first);

    /*sending flows, from endhost (1.0.0.2:49153) to Ues (7.0.0.2:200x)*/
    if (tuple.destinationPort >= 2001 && tuple.destinationPort <= 3000) {
      if (iter->second.rxPackets > 1){
        meanTxRate_send[tuple.sourceAddress] = 8*iter->second.txBytes/(iter->second.timeLastTxPacket.GetDouble()-iter->second.timeFirstTxPacket.GetDouble())*ONEBIL/(1024);
        meanRxRate_send[tuple.sourceAddress] = 8*iter->second.rxBytes/(iter->second.timeLastRxPacket.GetDouble()-iter->second.timeFirstRxPacket.GetDouble())*ONEBIL/(1024);
        meanTcpDelay_send[tuple.sourceAddress] = iter->second.delaySum.GetDouble()/iter->second.rxPackets/1000000;
      }
      numOfLostPackets_send[tuple.sourceAddress] = iter->second.lostPackets; 
      numOfTxPacket_send[tuple.sourceAddress] = iter->second.txPackets;
    }

    /*ack flow, from Ues (7.0.0.2:200x) to endhost (1.0.0.2:49153)*/
    if (tuple.destinationPort >= 49153){
      if (iter->second.rxPackets > 1){
        meanTxRate_ack[tuple.sourceAddress] = 8*iter->second.txBytes/(iter->second.timeLastTxPacket.GetDouble()-iter->second.timeFirstTxPacket.GetDouble())*ONEBIL/(1024);
        meanRxRate_ack[tuple.sourceAddress] = 8*iter->second.rxBytes/(iter->second.timeLastRxPacket.GetDouble()-iter->second.timeFirstRxPacket.GetDouble())*ONEBIL/(1024);
        meanTcpDelay_ack[tuple.sourceAddress] = iter->second.delaySum.GetDouble()/iter->second.rxPackets/1000000;
      }
      numOfLostPackets_ack[tuple.sourceAddress] = iter->second.lostPackets; 
      numOfTxPacket_ack[tuple.sourceAddress] = iter->second.txPackets;
    }

    *macro_wp->GetStream() << "***Flow ID: " << iter->first 
    << " Src Addr " << tuple.sourceAddress << std::endl
    << " Dst Addr " << tuple.destinationAddress << std::endl
    << "Tx Packets " << iter->second.txPackets << std::endl
    << "Rx Packets " << iter->second.rxPackets << std::endl
    << "Lost packets " << iter->second.lostPackets << std::endl
    << "Lost ratio " << double (iter->second.lostPackets)/(iter->second.lostPackets+iter->second.rxPackets) << std::endl;

    if (iter->second.rxPackets > 1){
        *macro_wp->GetStream() << "Average delay received " << iter->second.delaySum/iter->second.rxPackets/1000000 << std::endl
        << "Mean received bitrate " << 8*iter->second.rxBytes/(iter->second.timeLastRxPacket-iter->second.timeFirstRxPacket)*ONEBIL/(1024) << std::endl
        << "Mean transmitted bitrate " << 8*iter->second.txBytes/(iter->second.timeLastTxPacket-iter->second.timeFirstTxPacket)*ONEBIL/(1024) << std::endl;
    }
  }
}

// static void 
// CwndTracer (uint32_t oldval, uint32_t newval)
// {
//   NS_LOG_UNCOND (Simulator::Now().GetMilliSeconds() << " cwnd_from " << oldval << " to " << newval);
// }

// static void 
// enable_cwnd_trace(Ptr<Application> app)
// {
    
//     Ptr<OnOffApplication> on_off_app = app->GetObject<OnOffApplication>();
//     if (on_off_app != NULL)
//     {
//         Ptr<Socket> socket = on_off_app->GetSocket();
//         socket->TraceConnectWithoutContext("CongestionWindow", MakeCallback(&CwndTracer));//, stream));
//     }
// }

static void enable_tcp_socket_traces(Ptr<Application> app)
{
    Ptr<OnOffApplication> on_off_app = app->GetObject<OnOffApplication>();
    if (on_off_app != NULL)
    {
        Ptr<Socket> socket = on_off_app->GetSocket();
        socket->TraceConnectWithoutContext("CongestionWindow", MakeBoundCallback(&CwndTracer, cwnd_wp));
        socket->TraceConnectWithoutContext("RTO", MakeBoundCallback(&RTOTracer, rto_wp));
        socket->TraceConnectWithoutContext("RTT", MakeBoundCallback(&LastRttTracer, rtt_wp));
        socket->TraceConnectWithoutContext("HighestSequence", MakeBoundCallback(&HighestSentSeqTracer,highest_tx_seq_wp));
        socket->TraceConnectWithoutContext("NextTxSequence", MakeBoundCallback(&NextTxSeqTracer,next_tx_seq_wp));
    }
}



int
main (int argc, char *argv[])
{
    /*** commands parsing and default configuration setting up*****/
    init_log_and_cmd(argc, argv);

    /****init wrappers****/
    init_wrappers();

    /**************p2p and core network setting up*****************/
    p2p_setup();

    /**************RAN setting up**************/
    ran_setup();
    
    /************** install apps on Ues and remote host *********/
    install_apps_on_nodes();

    monitor = flowHelper.Install(ueNodes);
    monitor = flowHelper.Install(remote_host_node);
    monitor = flowHelper.GetMonitor();

    /*********Tracing settings***************/
    lteHelper->EnableTraces ();
    lteHelper->GetPdcpStats()->SetAttribute("EpochDuration", TimeValue( Seconds (0.010)) );   //set collection interval for PDCP.
    lteHelper->GetRlcStats()->SetAttribute("EpochDuration", TimeValue ( Seconds (0.010))) ;   //same for RLC
    // Uncomment to enable PCAP tracing
    p2ph.EnablePcapAll(pcapName);


    /*******************Start client and server apps***************/
    serverApps.Start (Seconds (0.01));		//All server start at 0.01s.
    clientApps.Start(Seconds(0.5));

    

    /**Pathloss**/
    // DownlinkLteGlobalPathlossDatabase dlPathlossDb;
    // UplinkLteGlobalPathlossDatabase ulPathlossDb;

    /*=============schedule to get TCP throughput============*/
    // Simulator::Schedule(Seconds (0.0), &getTcpPut);
    // getTcpPut();
    // double client_start = 0;
    // while (client_start < simTime){
    //     // std::cout << "client_start=" << client_start << "PUT_SAMPLING_INTERVAL=" << PUT_SAMPLING_INTERVAL << std::endl;
    //     client_start += PUT_SAMPLING_INTERVAL;
    //     Simulator::Schedule(Seconds(client_start),&getTcpPut);
    // }

    Simulator::Schedule(Seconds(0.6) + NanoSeconds(1.0), &enable_tcp_socket_traces, remote_host_node->GetApplication(0));///*Note: enable_cwnd_trace must be scheduled after the OnOffApplication starts (OnOffApplication's socket is created after the application starts) 

    /****Output settings****/
    Config::SetDefault("ns3::ConfigStore::Filename", StringValue("lte-dl-simplest-config-out.txt"));
    Config::SetDefault("ns3::ConfigStore::FileFormat", StringValue("RawText"));
    Config::SetDefault("ns3::ConfigStore::Mode", StringValue("Save"));
    outputConfig.ConfigureDefaults();
    outputConfig.ConfigureAttributes();


    /*********Start the simulation*****/
    Simulator::Stop(Seconds(simTime));
    Simulator::Run();
    

    /**************Simulation stops here. Start printing out information (if needed)***********/
    print_final_outputs();

    Simulator::Destroy();
    return 0;
}
