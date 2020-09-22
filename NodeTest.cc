#include<string>
#include "NodeTest.h"

#include "inet/networklayer/ipv4/Ipv4RoutingTable.h"
#include "inet/networklayer/contract/ipv4/Ipv4Address.h"
#include "inet/networklayer/common/InterfaceTable.h"

Define_Module(NodeTest);


void NodeTest::initialize()
{
    int numApps = par("apps");
    int startTime = par("startTime");
    for(int i = 0; i < numApps ; i++){
        cMessage *mecAppCreationEvent = new cMessage();
        mecAppCreationEvent->setKind(i);
        scheduleAt(simTime() + startTime, mecAppCreationEvent);
    }
}

void NodeTest::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()){
        int numApp = msg->getKind();
        EV << "Creating MecApp: " << numApp <<endl;
        spawnAndConnectApp(numApp);
    }
}

void NodeTest::spawnAndConnectApp(int numApp)
{
//    cModule *app = moduleType->create("app", this->getParentModule()->getParentModule());
      std::string mecApp = "mecApp["+std::to_string(numApp)+"]";
      const char * c0 = mecApp.c_str();
      cModule *mecAppToSpawn = getModuleByPath(c0);
//    //app->par("numEthInterfaces") = 1;
      cModule *vSwitchMod = getModuleByPath("vSwitch1");
      cModule *vSwitchMadMod = getModuleByPath("vSwitch1.mad");
      EV << "Current app gates "<< vSwitchMod->gateSize("vEthInt")<< endl;
      vSwitchMod->setGateSize("vEthInt", vSwitchMod->gateSize("vEthInt") + 1);
      vSwitchMadMod->setGateSize("toApps", vSwitchMadMod->gateSize("toApps") +1);

      cGate *madGate_out= vSwitchMadMod->gate("toApps$o", vSwitchMadMod->gateSize("toApps")-1); //the last created gate
      cGate *madGate_in= vSwitchMadMod->gate("toApps$i", vSwitchMadMod->gateSize("toApps")-1); //the last created gate

      cGate *vSwitchGate_out =vSwitchMod->gate("vEthInt$o", vSwitchMod->gateSize("vEthInt")-1); //the last created gate
      cGate *vSwitchGate_in =vSwitchMod->gate("vEthInt$i", vSwitchMod->gateSize("vEthInt")-1); //the last created gate

      cGate *appGate_out = mecAppToSpawn->gate("vEth$o");
      cGate *appGate_in = mecAppToSpawn->gate("vEth$i");

      // internal vSwitch connections
      madGate_out->connectTo(vSwitchGate_out);
      vSwitchGate_in->connectTo(madGate_in);

      // connect MecApp to the created internal vSwitch gate
//      cDatarateChannel *datarateChannelUplink = cDatarateChannel::create("channel");
//      datarateChannelUplink->setDatarate(1e9);
//      datarateChannelUplink->setDelay(0.0);
//      cDatarateChannel *datarateChannelDownlink = cDatarateChannel::create("channel");
//      datarateChannelDownlink->setDatarate(1e9);
//      datarateChannelDownlink->setDelay(0.0);
//      appGate_out->connectTo(vSwitchGate_in, datarateChannelDownlink);
//      vSwitchGate_out->connectTo(appGate_in, datarateChannelUplink);

      appGate_out->connectTo(vSwitchGate_in);
      vSwitchGate_out->connectTo(appGate_in);

      // update routing table
      //cModule *mecAppToSpawn = getModuleByPath("mecApp[0].vm.ethg[0]");
      std::string mecAppRT = "mecApp["+std::to_string(numApp)+"].vm.ipv4.routingTable";
      const char * c1 = mecAppRT.c_str();
      inet::Ipv4RoutingTable *vm_routing_table = check_and_cast<inet::Ipv4RoutingTable*>(getModuleByPath(c1));

      std::vector<inet::Ipv4Address> vm_external_addr  = std::vector<inet::Ipv4Address>();
      for (auto addr : vm_routing_table->gatherAddresses())
          if (! (addr.getAddressCategory() == inet::Ipv4Address::AddressCategory::LOOPBACK))
              vm_external_addr.push_back(addr);

      std::string mecAppIT = "mecApp["+std::to_string(numApp)+"].vm.interfaceTable";
      const char * c2 = mecAppIT.c_str();

      inet::InterfaceEntry *vm_eth_interface = check_and_cast<inet::InterfaceTable*>(getModuleByPath(c2))->findInterfaceByName("ppp0");
      inet::Ipv4Route *vm_tableEntry = new inet::Ipv4Route();
      vm_tableEntry->setInterface(vm_eth_interface);
      EV<<"OKKKK"<<endl;
      vm_routing_table->addRoute(vm_tableEntry);


      inet::Ipv4RoutingTable *router_routing_table = check_and_cast<inet::Ipv4RoutingTable*>(getModuleByPath("vSwitch1.vrouter.ipv4.routingTable"));

      inet::InterfaceTable *router_interface_table = check_and_cast<inet::InterfaceTable*>(getModuleByPath("vSwitch1.vrouter.interfaceTable"));
      inet::InterfaceEntry *interface = router_interface_table->findInterfaceByName("ppp1");

      inet::Ipv4Route* tableEntry = new inet::Ipv4Route();
      tableEntry->setDestination(vm_external_addr[0]);
      tableEntry->setNetmask(inet::Ipv4Address("255.255.255.255"));
      tableEntry->setInterface(interface);

      router_routing_table->addRoute(tableEntry);
      router_routing_table->printRoutingTable();


}


