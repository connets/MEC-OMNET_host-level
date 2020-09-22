#include<string>
#include "SpawnApp.h"
#include "MecAppDispatcher.h"

#include "inet/networklayer/ipv4/Ipv4RoutingTable.h"
#include "inet/networklayer/contract/ipv4/Ipv4Address.h"
#include "inet/networklayer/common/InterfaceTable.h"


Define_Module(SpawnApp);


SpawnApp::~SpawnApp(){
}

void SpawnApp::initialize()
{
    numApp = par("numMecApp");
    int startTime = par("startTime");
    cMessage *mecAppCreationEvent = new cMessage();
    scheduleAt(simTime() + startTime, mecAppCreationEvent);
}

void SpawnApp::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()){
        EV << "Creating MecApp: " << numApp << " ... "<< endl;
        spawnAndConnectApp();
    }
    delete msg;
}

void SpawnApp::spawnAndConnectApp()
{
    int mecHostNum = par("mecHostNum");
    std::string mecApp = "mecApp["+std::to_string(numApp)+"]";
    const char * c0 = mecApp.c_str();
    std::string mecHostString = "mecHost"+std::to_string(mecHostNum);
    const char * mecHost = mecHostString.c_str();
    std::string mecHostMadString = mecHostString + ".mad";
    const char * mecHostMad = mecHostMadString.c_str();


    cModule *mecAppToSpawn = getModuleByPath(c0);
    cModule *vSwitchMod = getModuleByPath(mecHost);
    cModule *vSwitchMadMod = getModuleByPath(mecHostMad);
    EV << "Current app gates " << vSwitchMod->gateSize("vEthInt")<< endl;
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

    appGate_out->connectTo(vSwitchGate_in);
    vSwitchGate_out->connectTo(appGate_in);

    // Adding on MecAppDispatcher map the app.
    // NumApp -- GateOutMecAppDispatcher

    MecAppDispatcher *madModule = check_and_cast<MecAppDispatcher*>(vSwitchMadMod);

    std::string mecAppAddr = mecApp + ".vm.eth[0]";
    const char * macAddr = mecAppAddr.c_str();

    inet::InterfaceEntry *ethApp = check_and_cast<inet::InterfaceEntry*>(getModuleByPath(macAddr));
    inet::MacAddress addr = ethApp->getMacAddress();

    madModule->addMap(addr,madGate_out->getId());
    EV << "Inserted in MecAppDispatcher address: " << addr << " ; gate: " << madGate_out->getId() <<endl;

    // update routing table
    std::string mecAppRT = "mecApp["+std::to_string(numApp)+"].vm.ipv4.routingTable";
    const char * c1 = mecAppRT.c_str();
    inet::Ipv4RoutingTable *vm_routing_table = check_and_cast<inet::Ipv4RoutingTable*>(getModuleByPath(c1));

    std::vector<inet::Ipv4Address> vm_external_addr  = std::vector<inet::Ipv4Address>();
    for (auto addr : vm_routing_table->gatherAddresses())
        if (! (addr.getAddressCategory() == inet::Ipv4Address::AddressCategory::LOOPBACK))
            vm_external_addr.push_back(addr);

    std::string mecAppIT = "mecApp["+std::to_string(numApp)+"].vm.interfaceTable";
    const char * c2 = mecAppIT.c_str();

    inet::InterfaceEntry *vm_eth_interface = check_and_cast<inet::InterfaceTable*>(getModuleByPath(c2))->findInterfaceByName("eth0");
    inet::Ipv4Route *vm_tableEntry = new inet::Ipv4Route();
    vm_tableEntry->setInterface(vm_eth_interface);
    vm_routing_table->addRoute(vm_tableEntry);

//  Evitare grazie a configurator

//    std::string mecHostRoutingTableString = "mecHost"+std::to_string(mecHostNum)+".vrouter.ipv4.routingTable";
//    const char * mecHostRoutingTable = mecHostRoutingTableString.c_str();
//
//    std::string mecHostInterfaceTableString = "mecHost"+std::to_string(mecHostNum)+".vrouter.interfaceTable";
//    const char * mecHostInterfaceTable = mecHostInterfaceTableString.c_str();
//
//    inet::Ipv4RoutingTable *router_routing_table = check_and_cast<inet::Ipv4RoutingTable*>(getModuleByPath(mecHostRoutingTable));
//
//    inet::InterfaceTable *router_interface_table = check_and_cast<inet::InterfaceTable*>(getModuleByPath(mecHostInterfaceTable));
//    inet::InterfaceEntry *interface = router_interface_table->findInterfaceByName("eth1");
//
//    inet::Ipv4Route* tableEntry = new inet::Ipv4Route();
//    tableEntry->setDestination(vm_external_addr[0]);
//    tableEntry->setNetmask(inet::Ipv4Address("255.0.0.0"));
//    tableEntry->setInterface(interface);
//
//    router_routing_table->addRoute(tableEntry);
//    router_routing_table->printRoutingTable();
}






