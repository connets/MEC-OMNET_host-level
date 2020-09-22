//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 
#include "MigrationApp.h"
#include "MecAppDispatcher.h"
#include "Arp_public.h"
#include "MecRsvpClassifier.h"
#include "inet/linklayer/ppp/Ppp.h"
#include "inet/networklayer/common/InterfaceEntry.h"
#include "inet/networklayer/ipv4/Ipv4RoutingTable.h"
#include "inet/networklayer/contract/ipv4/Ipv4Address.h"
#include "inet/networklayer/common/InterfaceTable.h"
#include "inet/networklayer/arp/ipv4/Arp.h"
#include "inet/networklayer/common/L3AddressResolver.h"

Define_Module(MigrationApp);

void MigrationApp::initialize()
{
    int startTime = par("startTime");
    cMessage *mecAppMigrationEvent = new cMessage();
    from = par("from").stringValue();
    to = par("to").stringValue();
    numMecApp = par("numMecApp");
    scheduleAt(simTime() + startTime, mecAppMigrationEvent);
}

void MigrationApp::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()){
        EV << "Migration MecApp: "<<endl;
        migrate();
    }
    delete msg;
}
void MigrationApp::migrate()
{
    //Prendi la mecApp[0]
    std::string mecAppString = "mecApp["+std::to_string(numMecApp)+"]";
    const char * mecApp = mecAppString.c_str();

    cModule *appToMigrate = getModuleByPath(mecApp);
    cModule *mecHostFrom = getModuleByPath((from).c_str());

    //Scollegala
    cGate *appGate_out = appToMigrate->gate("vEth$o");
    cGate *appGate_in = appToMigrate->gate("vEth$i");

    //the correct created gate della mecapp -> appGateMap[mecApp[0]]
    const char * dispatcher = (from + ".mad").c_str();
    MecAppDispatcher *dispFrom = check_and_cast<MecAppDispatcher*>(getModuleByPath(dispatcher));

    std::string macAddrApp = mecAppString + ".vm.eth[0]";
    const char * macAddr = macAddrApp.c_str();

    inet::InterfaceEntry *ethApp = check_and_cast<inet::InterfaceEntry*>(getModuleByPath(macAddr));
    inet::MacAddress addr = ethApp->getMacAddress();

    int gateId = dispFrom->findGateId(addr);
    cGate *vSwitchSource_out = mecHostFrom->gate(gateId);
    dispFrom->eraseMap(addr);

    // GATE MAD DA DISCONNETTERE (?) SI

    appGate_out->disconnect();
    //    vSwitchSource_out->getNextGate()->disconnect();
    vSwitchSource_out->getPreviousGate()->disconnect();
    vSwitchSource_out->disconnect();

    //Attaccala

    cModule *mecHostDest = getModuleByPath((to).c_str());
    cModule *mecHostMadDest = getModuleByPath((to + ".mad").c_str());

    mecHostDest->setGateSize("vEthInt", mecHostDest->gateSize("vEthInt") + 1);
    mecHostMadDest->setGateSize("toApps", mecHostMadDest->gateSize("toApps") +1);
    //

    MecAppDispatcher *dispTo = check_and_cast<MecAppDispatcher*>(mecHostMadDest);
    dispTo->addMap(addr, gateId);

    cGate *madGate_out= mecHostMadDest->gate("toApps$o", mecHostMadDest->gateSize("toApps")-1); //the last created gate
    cGate *madGate_in= mecHostMadDest->gate("toApps$i", mecHostMadDest->gateSize("toApps")-1); //the last created gate
    //
    cGate *mecHostGate_out = mecHostDest->gate("vEthInt$o", mecHostDest->gateSize("vEthInt")-1); //the last created gate
    cGate *mecHostGate_in = mecHostDest->gate("vEthInt$i", mecHostDest->gateSize("vEthInt")-1); //the last created gate

    //SIGNALS

    //    cModule *vm = getModuleByPath((mecAppString + ".vm.ppp[0].ppp").c_str());
    //    inet::Ppp * pppggate = check_and_cast<inet::Ppp *> (vm);
    //    EV << "modulo ppp subscribed: "<< isSubscribed(POST_MODEL_CHANGE, pppggate) << endl;
    //    unsubscribe(POST_MODEL_CHANGE, pppggate);

    //

    madGate_out->connectTo(mecHostGate_out);
    mecHostGate_in->connectTo(madGate_in);
    appGate_out->connectTo(mecHostGate_in);
    mecHostGate_out->connectTo(appGate_in);

    //Aggiorna il router esterno

    route_label();
}

void MigrationApp::route_label(){
    //    EV << "Aggiungo la route..." << "router_ext.classifier" << endl;
    EV << "Aggiungo la route..." << "router_"<< to <<".classifier" << endl;
    const char * router_dest = ("router_"+ to + ".classifier").c_str(); // non dovrebbe cambaire, vengono aggiunte in tutti
    MecRsvpClassifier *userClassifier = check_and_cast<MecRsvpClassifier*>(getModuleByPath(router_dest));

    //    userClassifier->add_in_table(3, 3, 300, "user");
    const char * dest = ("mecApp["+ std::to_string(numMecApp) +"].vm").c_str();
//    userClassifier->remove_from_table(4);
    userClassifier->add_in_table(4, 4, 402, dest);
    EV<<"Aggiunto bind con corretta label switch path verso " << dest << endl;

    // AGGIUNGO COSE CHE MI TOGLIE QUESTO PROCEDIMENTO

    std::string mecAppRT = "mecApp["+std::to_string(numMecApp)+"].vm.ipv4.routingTable";
    const char * c1 = mecAppRT.c_str();
    inet::Ipv4RoutingTable *vm_routing_table = check_and_cast<inet::Ipv4RoutingTable*>(getModuleByPath(c1));

    std::vector<inet::Ipv4Address> vm_external_addr  = std::vector<inet::Ipv4Address>();
    for (auto addr : vm_routing_table->gatherAddresses())
        if (! (addr.getAddressCategory() == inet::Ipv4Address::AddressCategory::LOOPBACK))
            vm_external_addr.push_back(addr);

    std::string mecAppIT = "mecApp["+std::to_string(numMecApp)+"].vm.interfaceTable";
    const char * c2 = mecAppIT.c_str();

    inet::InterfaceEntry *vm_eth_interface = check_and_cast<inet::InterfaceTable*>(getModuleByPath(c2))->findInterfaceByName("eth0");
    inet::Ipv4Route *vm_tableEntry = new inet::Ipv4Route();
    vm_tableEntry->setInterface(vm_eth_interface);
    vm_routing_table->addRoute(vm_tableEntry);



    const char * arp_module_string = ("mecApp["+ std::to_string(numMecApp)+ "].vm.ipv4.arp").c_str();
    Arp_public *arp_module = check_and_cast<Arp_public*>(getModuleByPath(arp_module_string));

    arp_module->flush_public();

}

