

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


/**
 * NOTE: This code doesn't support MULTIPLE ENODEBs yet (problem might lay on the mobility model, co-location in position of EnodeBs).
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
#include "ns3/config-store.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include "ns3/flow-monitor-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/lte-global-pathloss-database.h"

//#include "ns3/gtk-config-store.h"

#include <map>

using namespace ns3;
/**
 * Theory Topology:                Ues (x numberOfNodes) ---------- EnodeB ---------- |
 *																																	|
 * 							 Ues (x numberOfNodes) ---------- EnodeB ----------	|
 * 							 																										|
 * 							 Ues (x numberOfNodes) ---------- EnodeB ----------	|	SPGW -------------------------- RH
 *
 *                              										n2                 n0                       n1
 */
 /*
  * Experiment topology:
  *     
  *             UE (n3) -------------------- ENB (n2) ==================== SPGW (n0) ==================== End-host (n1)
  *                                                 2:2                  0:3        0:2                 1:1
  *                             radio                  <1Gbps,15ms,1500>               <1Gbps,15ms,1500>
  */
/**
 * Sample simulation script for LTE+EPC. It instantiates several eNodeB,
 * attaches one UE per eNodeB starts a flow for each UE to  and from a remote host.
 * It also  starts yet another flow between each UE pair.
 */
NS_LOG_COMPONENT_DEFINE ("lte");

#define kilo 1000


double
CalculateAverageDelay(std::map <uint64_t, uint32_t> delayArray);


double simTime = 100;	//simulation time for EACH application
std::ofstream tcpThroughput;
std::ofstream tcpThroughput_ack;
Ptr<ns3::FlowMonitor> monitor;
FlowMonitorHelper flowHelper;
double samplingInterval = 0.01;    /*sample TCP thoughput for each 50ms*/
double t = 0.0;
uint16_t isTcp = 1;
//topology
uint16_t numberOfUeNodes = 1;
uint16_t numberOfEnodebs = 1;

//S1uLink
std::string s1uLinkDataRate = "1Gb/s";
double  s1uLinkDelay = 0.015;
uint16_t s1uLinkMtu = 1500;

//p2pLink
std::string p2pLinkDataRate = "1Gb/s";
double p2pLinkDelay = 0.015;
uint16_t p2pLinkMtu = 1500;

//Simulation
uint32_t numberOfPackets = 0;
uint32_t packetSize = 900;
double distance = 1000.0;    //With enbTxPower=5, Noise=37 and UeTxPower=50 (NEED TO BE THAT HIGH TO GUARANTEE UPLINK FOR TCP ACK FLOW), noise=9, we have roughly 1000Kb/s downlink bandwidth.
uint16_t radioUlBandwidth = 100;  //the radio link bandwidth among UEs and EnodeB (in Resource Blocks). This is the configuration on LteEnbDevice.
uint16_t radioDlBandwidth = 100;  //same as above, for downlink.
std::string dataRate = "100Mb/s";

uint16_t isAMRLC = 0;    


static double last_tx_time = 0;
static double last_rx_time = 0;
static double last_tx_bytes = 0;
static double last_rx_bytes = 0;
static double tcp_delay = 0;
static double last_delay_sum = 0;
static double last_rx_pkts = 0;

/**sending flowS stats***/
std::map<Ipv4Address, double> meanTxRate_send;
std::map<Ipv4Address, double> meanRxRate_send;
std::map<Ipv4Address, double> meanTcpDelay_send;
std::map<Ipv4Address, uint64_t> numOfLostPackets_send;
std::map<Ipv4Address, uint64_t> numOfTxPacket_send;
double last_lost = 0;
/***acking flowS stats***/
std::map<Ipv4Address, double> meanTxRate_ack;
std::map<Ipv4Address, double> meanRxRate_ack;
std::map<Ipv4Address, double> meanTcpDelay_ack;
std::map<Ipv4Address, uint64_t> numOfLostPackets_ack;
std::map<Ipv4Address, uint64_t> numOfTxPacket_ack;
double last_lost_ack = 0;
Ptr<ns3::Ipv4FlowClassifier> classifier;
std::map <FlowId, FlowMonitor::FlowStats> stats;


const uint32_t ONEBIL = 1000000000;

/********* Ascii output files name *********/
static std::string DIR = "/var/tmp/ln_result/radio/";
static std::string cwnd = DIR+"cwnd.dat";
static std::string rto = DIR+"rto_value_tmp.tmp";
static std::string rwnd = DIR+"rwnd_tmp.tmp";
static std::string rtt = DIR+"last_rtt_sample_tmp.tmp";
static std::string highesttxseq = DIR+"highest_tx_seq.dat";
static std::string nexttxseq = DIR+"next_tx_seq.dat";
// static std::string queues = DIR+"queues.dat";
static std::string macro = DIR+"macro_output.dat";
static std::string put_send;
static std::string put_ack;
static std::string debugger = "debugger.dat";

/********wrappers**********/
static AsciiTraceHelper asciiTraceHelper;
Ptr<OutputStreamWrapper> cwnd_wp;
Ptr<OutputStreamWrapper> rto_wp;
Ptr<OutputStreamWrapper> rwnd_wp;
Ptr<OutputStreamWrapper> highest_tx_seq_wp;
Ptr<OutputStreamWrapper> next_tx_seq_wp;
Ptr<OutputStreamWrapper> rtt_wp;
// Ptr<OutputStreamWrapper> dev_queues_wp;
Ptr<OutputStreamWrapper> put_send_wp;
Ptr<OutputStreamWrapper> put_ack_wp;
Ptr<OutputStreamWrapper> macro_wp;
Ptr<OutputStreamWrapper> debugger_wp;

/**************** Functions **************/

static void getTcpPut();
static void init_wrappers();
/*****************NSC*********************/
//static std::string nsc_stack="liblinux2.6.26.so";
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

static void enable_tcp_socket_traces(Ptr<Application> app)
{
    Ptr<OnOffApplication> on_off_app = app->GetObject<OnOffApplication>();
    if (on_off_app != NULL)
    {
        *debugger_wp->GetStream() << "enabling tracers....\n";
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
	//*************Enable logs********************/
    //To enable all components inside the LTE module.
//      lteHelper->EnableLogComponents();
    
    //	LogComponentEnable("UdpEchoClientApplication",LOG_LEVEL_INFO);
    //	LogComponentEnable("UdpEchoClientApplication",LOG_PREFIX_ALL);
    	// LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);
    	// LogComponentEnable("UdpClient",LOG_LEVEL_INFO);
    	 // LogComponentEnable("UdpServer", LOG_LEVEL_INFO);
//		LogComponentEnable("OnOffApplication",LOG_LEVEL_INFO);
//		LogComponentEnable("PacketSink",LOG_LEVEL_INFO);
    LogLevel level = (LogLevel) (LOG_LEVEL_ALL | LOG_PREFIX_TIME | LOG_PREFIX_NODE | LOG_PREFIX_FUNC);
   // LogComponentEnable("TcpTahoe", level);
   // LogComponentEnable("RttEstimator",level);
   // LogComponentEnable("TcpSocketBase",level);
    LogComponentEnable ("LteRlcUm", level);
    LogComponentEnable ("LteRlcAm", level);
    LogComponentEnable ("NscTcpSocketImpl",LOG_LEVEL_DEBUG);
    		// LogComponentEnable("OnOffApplication",level);
//     		LogComponentEnable("PacketSink",level);
    //   LogComponentEnable ("LteHelper",level);
//       LogComponentEnable ("LteUeMac", level);
    //   LogComponentEnable ("LteEnbMac", level);
//       LogComponentEnable ("LtePdcp", level);
//       LogComponentEnable ("LtePhy", level);
   
    
    // Command line arguments
    CommandLine cmd;
    cmd.AddValue("numberOfUeNodes", "Number of UeNodes", numberOfUeNodes);
    cmd.AddValue("numberOfEnodebs", "Number of eNodebs", numberOfEnodebs);
    cmd.AddValue("simTime", "Total duration of the simulation [s])", simTime);
    cmd.AddValue("distance", "Distance between eNBs [m]", distance);
    cmd.AddValue("numberOfPackets", "Number of packets to send", numberOfPackets);
    cmd.AddValue("packetSize", "Size of each packet", packetSize);
    cmd.AddValue("s1uLinkDataRate", "S1u Link Data Rate", s1uLinkDataRate);
    cmd.AddValue("s1uLinkDelay", "S1u Link Delay", s1uLinkDelay);
    cmd.AddValue("s1uLinkMtu", "S1u Link Mtu", s1uLinkMtu);
    cmd.AddValue("p2pLinkDataRate", "p2p Link Data Rate", p2pLinkDataRate);
    cmd.AddValue("p2pLinkDelay", "p2p Link Delay", p2pLinkDelay);
    cmd.AddValue("p2pLinkMtu", "p2p Link Mtu", p2pLinkMtu);
    cmd.AddValue("radioUlBandwidth", "Uplink radio bandwidth [RBs] (6,15,25,50,75,100)", radioUlBandwidth);
    cmd.AddValue("radioDlBandwidth", "Downlink radio bandwidth [RBs] (6,15,25,50,75,100)", radioDlBandwidth);
    cmd.AddValue("isAMRLC", "Whether using AM RLC (UM RLC if false)", isAMRLC);
    cmd.AddValue("dataRate", "TCP application data rate", dataRate);
    cmd.AddValue("isTcp", "TCP application if true, Udp if false", isTcp);




    /**ConfigStore setting*/
    Config::SetDefault("ns3::ConfigStore::Filename", StringValue("lte-nonsc-new-in.txt"));
    Config::SetDefault("ns3::ConfigStore::FileFormat", StringValue("RawText"));
    Config::SetDefault("ns3::ConfigStore::Mode", StringValue("Load"));
    ConfigStore inputConfig;
    inputConfig.ConfigureDefaults();
    inputConfig.ConfigureAttributes();
    
    cmd.Parse(argc, argv);
    //*************************************************/

    init_wrappers();

    //************lteHeper, epcHelper**************//
    Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();
    Ptr<EpcHelper>  epcHelper = CreateObject<EpcHelper> ();
    lteHelper->SetEpcHelper (epcHelper);
    lteHelper->SetSchedulerType("ns3::PfFfMacScheduler");
    

    //*********************Use epcHelper to get the PGW node********************//
    Ptr<Node> pgw = epcHelper->GetPgwNode ();
    epcHelper->SetAttribute("S1uLinkDataRate", DataRateValue (DataRate (s1uLinkDataRate)));
    epcHelper->SetAttribute("S1uLinkDelay", TimeValue (Seconds (s1uLinkDelay)));
    epcHelper->SetAttribute("S1uLinkMtu", UintegerValue (s1uLinkMtu));
    
    
    
    //***********Create a single RemoteHost, install the Internet stack on it*************//
    NodeContainer remoteHostContainer;
    remoteHostContainer.Create (1);
    Ptr<Node> remoteHost = remoteHostContainer.Get (0);
    //Install Internet stack on the remoteHost.
    InternetStackHelper internet;
    //internet.SetTcp("ns3::NscTcpL4Protocol", "Library", StringValue(nsc_stack));
    internet.Install (remoteHost);
    //Config::Set ("/NodeList/*/$ns3::Ns3NscStack<linux2.6.26>/net.ipv4.tcp_sack", StringValue ("0"));
    //Config::Set ("/NodeList/*/$ns3::Ns3NscStack<linux2.6.26>/net.ipv4.tcp_timestamps", StringValue ("0"));
    //Config::Set ("/NodeList/*/$ns3::Ns3NscStack<linux2.6.26>/net.ipv4.tcp_window_scaling", StringValue ("0"));
    
    //***************Create and install a point to point connection between the SPGW and the remoteHost*****************//
    PointToPointHelper p2ph;
    p2ph.SetDeviceAttribute ("DataRate", DataRateValue (DataRate (p2pLinkDataRate)));
    p2ph.SetDeviceAttribute ("Mtu", UintegerValue (p2pLinkMtu));
    p2ph.SetChannelAttribute ("Delay", TimeValue (Seconds (p2pLinkDelay)));
    NetDeviceContainer internetDevices = p2ph.Install (pgw, remoteHost);		//The interfaces between the SPGW and remoteHost were saved in internetDevices.
    
    // Create the Internet
    Ipv4AddressHelper ipv4h;	//Ipv4AddressHelper is used to assign Ip Address for a typical node.
    ipv4h.SetBase ("1.0.0.0", "255.0.0.0");
    Ipv4InterfaceContainer internetIpIfaces = ipv4h.Assign (internetDevices);		//assign IP addresses in starting at "1.0.0.0" to the SPGW and remoteHost.
    // interface 0 is localhost, 1 is the p2p device
    //Ipv4Address remoteHostAddr = internetIpIfaces.GetAddress (1);
    
    
    //***************************Let's the remoteHost know how to route to UE "7.0.0.0"**************************//
    Ipv4StaticRoutingHelper ipv4RoutingHelper;
    //get the static routing method to the remoteHost. The parameter for GetStaticRouting() is the Ptr<Ipv4> of the destination.
    Ptr<Ipv4StaticRouting> remoteHostStaticRouting = ipv4RoutingHelper.GetStaticRouting (remoteHost->GetObject<Ipv4> ());	//remoteHostStaticRouting now knows how to route to the remoteHost.
    remoteHostStaticRouting->AddNetworkRouteTo (Ipv4Address ("7.0.0.0"), Ipv4Mask ("255.0.0.0"), 1);	//Add the routing entry to the remoteHostStaticRouting table.
    //"1" means interface #1 of the remoteHost will route to "7.0.0.0/24" (which is default Ipv4 range for UEs??)
    
    //**********************************Create Ue nodes, EnodeBs*******************************//
    NodeContainer ueNodes;
    NodeContainer enbNodes;
    enbNodes.Create(numberOfEnodebs);
    ueNodes.Create(numberOfUeNodes);
    
    
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
    NetDeviceContainer enbLteDevs = lteHelper->InstallEnbDevice (enbNodes);


    
    NetDeviceContainer::Iterator enbLteDevIt = enbLteDevs.Begin ();
    Vector enbPosition = (*enbLteDevIt)->GetNode ()->GetObject<MobilityModel> ()->GetPosition ();
    MobilityHelper ueMobility;
    ueMobility.SetPositionAllocator ("ns3::UniformDiscPositionAllocator", //nodes are put randomly inside a circle with the central point is (x,y).
                                     "X", DoubleValue (enbPosition.x),
                                     "Y", DoubleValue (enbPosition.y),
                                     "rho", DoubleValue (distance));  //radius of the circle.

    ueMobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
    ueMobility.Install (ueNodes);
    NetDeviceContainer ueLteDevs = lteHelper->InstallUeDevice (ueNodes); 


   
    //**********************Assign Ipv4 addresses for UEs. Install the IP stack on the UEs******************//
    internet.Install (ueNodes);	//internet (InternetStackHelper) again be used to install an Internet stack for a node.

    // Assign IP address to UEs, and install applications
    Ipv4InterfaceContainer ueIpIface;
    ueIpIface = epcHelper->AssignUeIpv4Address (NetDeviceContainer (ueLteDevs));

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

    //*****************************Install and start applications on UEs and remote host****************************//
    uint16_t dlPort = 2000;
    ApplicationContainer clientApps;
    ApplicationContainer serverApps;
    for (uint32_t u = 0; u < ueNodes.GetN (); ++u)
    {
        ++dlPort;				//each Ue will contact with the remoteHost by a different dlPort (the remoteHost needs this).
        

        if (isTcp == 1){
					/*********TCP Application********/
					//Create a packet sink to receive packet on remoteHost
                    LogComponentEnable("Queue",level);    //Only enable Queue monitoring for TCP to accelerate experiment speed.
					PacketSinkHelper sink("ns3::TcpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), dlPort));
					serverApps.Add(sink.Install(ueNodes.Get(u)));

					OnOffHelper onOffHelper("ns3::TcpSocketFactory", Address ( InetSocketAddress(ueIpIface.GetAddress(u) , dlPort) ));
					onOffHelper.SetConstantRate( DataRate(dataRate), packetSize );
					if (numberOfPackets != 0)
						onOffHelper.SetAttribute("MaxBytes",UintegerValue(packetSize*numberOfPackets));
					clientApps.Add(onOffHelper.Install(remoteHost));
        }
        else{
					/*********UDP Application********/
					//Create a packet sink to receive packet on remoteHost
					PacketSinkHelper sink("ns3::UdpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), dlPort));
					serverApps.Add(sink.Install(ueNodes.Get(u)));

					OnOffHelper onOffHelper("ns3::UdpSocketFactory", Address ( InetSocketAddress(ueIpIface.GetAddress(u), dlPort) ));
					onOffHelper.SetConstantRate( DataRate(dataRate), packetSize );
					if (numberOfPackets != 0)
						onOffHelper.SetAttribute("MaxBytes",UintegerValue(packetSize*numberOfPackets));
					clientApps.Add(onOffHelper.Install(remoteHost));
        }

    }




    /*******************Start client and server apps***************/
    serverApps.Start (Seconds (0.01));		//All server start at 0.01s.
    clientApps.Start(Seconds(0.5));
    
    
    /*********Tracing settings***************/
    lteHelper->EnableTraces ();
    lteHelper->GetPdcpStats()->SetAttribute("EpochDuration", TimeValue( Seconds (0.010)) );		//set collection interval for PDCP.
    lteHelper->GetRlcStats()->SetAttribute("EpochDuration", TimeValue ( Seconds (0.010)))	;		//same for RLC
    // Uncomment to enable PCAP tracing
    p2ph.EnablePcapAll("lte-simplest");


    monitor = flowHelper.Install(ueNodes);
    monitor = flowHelper.Install(remoteHost);
    monitor = flowHelper.GetMonitor();


    /****ConfigStore setting****/
    Config::SetDefault("ns3::ConfigStore::Filename", StringValue("lte-nonnsc-new.out"));
    Config::SetDefault("ns3::ConfigStore::FileFormat", StringValue("RawText"));
    Config::SetDefault("ns3::ConfigStore::Mode", StringValue("Save"));
    ConfigStore outputConfig;
    outputConfig.ConfigureDefaults();
    outputConfig.ConfigureAttributes();

  
    /*=============schedule to get TCP throughput============*/
    //Time t = Seconds(0.0);
    Simulator::ScheduleWithContext (0 ,Seconds (0.0), &getTcpPut);
    Simulator::Schedule(Seconds(0.6) + NanoSeconds(1.0), &enable_tcp_socket_traces, remoteHostContainer.Get(0)->GetApplication(0));///*Note: enable_cwnd_trace must be scheduled after the OnOffApplication starts (OnOffApplication's socket is created after the application starts) 



    /*********Start the simulation*****/
    Simulator::Stop(Seconds(simTime));
    Simulator::Run();
    

    /**************Simulation stops here. Start printing out information (if needed)***********/
    
  
  monitor->CheckForLostPackets();
  Ptr<ns3::Ipv4FlowClassifier> classifier = DynamicCast<ns3::Ipv4FlowClassifier> (flowHelper.GetClassifier());
  std::map <FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats();

  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator iter = stats.begin(); iter != stats.end(); ++iter){
    ns3::Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(iter->first);

    /*sending flows, from endhost (1.0.0.2:49153) to Ues (7.0.0.2:200x)*/
    if (t.destinationPort >= 2001 && t.destinationPort <= 3000) {
      if (iter->second.rxPackets > 1){
        meanTxRate_send[t.sourceAddress] = 8*iter->second.txBytes/(iter->second.timeLastTxPacket.GetDouble()-iter->second.timeFirstTxPacket.GetDouble())*ONEBIL/(1024);
        meanRxRate_send[t.sourceAddress] = 8*iter->second.rxBytes/(iter->second.timeLastRxPacket.GetDouble()-iter->second.timeFirstRxPacket.GetDouble())*ONEBIL/(1024);
        meanTcpDelay_send[t.sourceAddress] = iter->second.delaySum.GetDouble()/iter->second.rxPackets/1000000;
      }
      numOfLostPackets_send[t.sourceAddress] = iter->second.lostPackets; 
     numOfTxPacket_send[t.sourceAddress] = iter->second.txPackets;
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

    *macro_wp->GetStream() << "***Flow ID: " << iter->first << " Src Addr " << t.sourceAddress << " Dst Addr " << t.destinationAddress << std::endl
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

  Simulator::Destroy();
  return 0;
    
}

/***Calculate average of a map**/
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

// static void enable_cwnd_trace(Ptr<Application> app)
// {
    
//     Ptr<OnOffApplication> on_off_app = app->GetObject<OnOffApplication>();
//     if (on_off_app != NULL)
//     {
//         Ptr<Socket> socket = on_off_app->GetSocket();
//         socket->TraceConnectWithoutContext("CongestionWindow", MakeCallback(&CwndTracer));//, stream));
//     }
// }



static void
getTcpPut(){
    monitor->CheckForLostPackets();
    classifier = DynamicCast<ns3::Ipv4FlowClassifier> (flowHelper.GetClassifier());
    stats = monitor->GetFlowStats();

    /*==============Get flows information============*/
  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator iter = stats.begin(); iter != stats.end(); ++iter){
    ns3::Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(iter->first);

    /*sending flows, from endhost (1.0.0.2:49153) to Ues (7.0.0.2:200x)*/
    if (t.destinationPort >= 2001 && t.destinationPort <= 3000) {
      if (iter->second.rxPackets > 5 && iter->second.txPackets > 5){
        meanTxRate_send[t.destinationAddress] = 8*(iter->second.txBytes-last_tx_bytes)/(iter->second.timeLastTxPacket.GetDouble()-last_tx_time)*ONEBIL/kilo;
        meanRxRate_send[t.destinationAddress] = 8*(iter->second.rxBytes-last_rx_bytes)/(iter->second.timeLastRxPacket.GetDouble()-last_rx_time)*ONEBIL/kilo;
        last_tx_time = iter->second.timeLastTxPacket.GetDouble();
        last_tx_bytes = iter->second.txBytes;
        last_rx_time = iter->second.timeLastRxPacket.GetDouble();
        last_rx_bytes = iter->second.rxBytes;      
        meanTcpDelay_send[t.destinationAddress] = iter->second.delaySum.GetDouble()/iter->second.rxPackets/1000000;
      }
      numOfLostPackets_send[t.destinationAddress] = iter->second.lostPackets;
	if (iter->second.rxPackets > last_rx_pkts){
    		    tcp_delay = (iter->second.delaySum.GetDouble() - last_delay_sum) / (iter->second.rxPackets - last_rx_pkts)/(kilo*kilo);
    		    last_delay_sum = iter->second.delaySum.GetDouble();
    		    last_rx_pkts = iter->second.rxPackets;
    	 }

      if (iter->second.lostPackets > last_lost){
	     NS_LOG_UNCOND(Simulator::Now().GetSeconds() << " Tcp lost= " << iter->second.lostPackets - last_lost);
	     last_lost = iter->second.lostPackets;
      }
 
      numOfTxPacket_send[t.destinationAddress] = iter->second.txPackets;
    }

    /*ack flow, from Ues (7.0.0.2:200x) to endhost (1.0.0.2:49153)*/
    if (t.destinationPort >= 49153){
      if (iter->second.rxPackets > 1 && iter->second.txPackets > 1){
        meanTxRate_ack[t.sourceAddress] = 8*iter->second.txBytes/(iter->second.timeLastTxPacket.GetDouble()-iter->second.timeFirstTxPacket.GetDouble())*ONEBIL/(1024);
        meanRxRate_ack[t.sourceAddress] = 8*iter->second.rxBytes/(iter->second.timeLastRxPacket.GetDouble()-iter->second.timeFirstRxPacket.GetDouble())*ONEBIL/(1024);
        meanTcpDelay_ack[t.sourceAddress] = iter->second.delaySum.GetDouble()/iter->second.rxPackets/1000000;
      }
      numOfLostPackets_ack[t.sourceAddress] = iter->second.lostPackets;
      if (iter->second.lostPackets > last_lost_ack){
	NS_LOG_UNCOND(Simulator::Now().GetSeconds() << " Tcp_ack lost= " << iter->second.lostPackets - last_lost_ack);
	last_lost_ack = iter->second.lostPackets;	
      }
      numOfTxPacket_ack[t.sourceAddress] = iter->second.txPackets;
    }
  }


    std::map<Ipv4Address,double>::iterator it1 = meanRxRate_send.begin();
    std::map<Ipv4Address,double>::iterator it2 = meanTcpDelay_send.begin();
    std::map<Ipv4Address,uint64_t>::iterator it3 = numOfLostPackets_send.begin();
    std::map<Ipv4Address,uint64_t>::iterator it4 = numOfTxPacket_send.begin();
    std::map<Ipv4Address,double>::iterator it5 = meanTxRate_send.begin();

    for (;it1 != meanRxRate_send.end(); ){
      *put_send_wp->GetStream() << Simulator::Now().GetSeconds() << "\t\t"
                  << (*it1).first << "\t\t"
                  << (*it1).second << "\t\t"
                  << (*it2).second << "\t\t"
                  << (*it3).second << "\t\t"
                  << (*it4).second << "\t\t"
                  << "x" << "\t\t"
                  << "x" << "\t\t"
                  << "x" << "\t\t"
                  << "x" << "\t"
		  << (*it5).second << "\t"
		  << tcp_delay <<  "\n";
                  ++it1;
                  ++it2;
                  ++it3;
                  ++it4;
           	  ++it5;
    }

    while (t < simTime){
        t += samplingInterval;
        Simulator::Schedule(Seconds(t),&getTcpPut);
    }
}

static void 
init_wrappers(){
    /* create files for wrappers */
    cwnd_wp = asciiTraceHelper.CreateFileStream(cwnd);
    rto_wp = asciiTraceHelper.CreateFileStream(rto);
    rtt_wp = asciiTraceHelper.CreateFileStream(rtt);
    rtt_wp = asciiTraceHelper.CreateFileStream(rtt);
    highest_tx_seq_wp = asciiTraceHelper.CreateFileStream(highesttxseq);
    next_tx_seq_wp = asciiTraceHelper.CreateFileStream(nexttxseq);
    // dev_queues_wp = asciiTraceHelper.CreateFileStream(queues);
    macro_wp = asciiTraceHelper.CreateFileStream(macro);
    debugger_wp = asciiTraceHelper.CreateFileStream(debugger);

    //********************Initialize wrappers*********************/
    if (isTcp==1){
      put_send = DIR + "tcp-put.dat";
      put_ack = DIR + "tcp-put-ack.dat";
    } else{
      put_send = DIR + "udp-put.dat";
      put_ack = DIR + "udp-put-ack.dat";
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
