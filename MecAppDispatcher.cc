/*
 * MecAppDispatcher.cpp
 *
 *  Created on: 20 mag 2020
 *      Author: chris
 */
#include <algorithm>

#include "MecAppDispatcher.h"
#include "inet/networklayer/common/InterfaceEntry.h"
#include "inet/networklayer/common/NextHopAddressTag_m.h"
#include "inet/common/packet/Packet.h"
#include "inet/networklayer/common/L3Tools.h"
#include "inet/networklayer/ipv4/Ipv4Header_m.h"
#include "inet/linklayer/ethernet/EtherPhyFrame_m.h"
#include "inet/linklayer/ethernet/EtherFrame_m.h"



Define_Module(MecAppDispatcher);

MecAppDispatcher::MecAppDispatcher() {
    // TODO Auto-generated constructor stub

}

MecAppDispatcher::~MecAppDispatcher() {
    // TODO Auto-generated destructor stub
}

void MecAppDispatcher::sendMessage(cMessage *msg, cGate *gate) {
    MadMessage *madMsg = new MadMessage();
    madMsg->setMsg(msg);
    madMsg->setOutGate(gate);
    simtime_t schedule_time = std::max(simTime(), gate->findTransmissionChannel()->getTransmissionFinishTime());
    scheduleAt(schedule_time, madMsg);
}

void MecAppDispatcher::handleMessage(cMessage *message) {
    EV << message << " " << message->getSenderModule() << endl;
    if (message->isSelfMessage() && dynamic_cast<MadMessage*>(message)) {
        MadMessage *madMsg = (MadMessage*) message;
        send(madMsg->getMsg(), madMsg->getOutGate());
    } else {
        if (message->arrivedOn("toSwitch$i")) {
            inet::EthernetSignal *inc = check_and_cast<inet::EthernetSignal*>(message);
            inet::MacAddress dest;

            EV << "Messaggio ricevuto: "<< inc << endl;
            if(inc->hasEncapsulatedPacket()){
                inet::EthernetSignal *inc_copy = inc->dup();
                auto packet = check_and_cast<inet::Packet *>(inc_copy->decapsulate());
                delete inc_copy;

                packet->popAtFront<inet::EthernetPhyHeader>();

                auto frame = packet->peekAtFront<inet::EthernetMacHeader>();
                dest = frame->getDest();
                EV << "Destinazione Mac: "<< dest << endl;
                delete packet;
                if(dest.isBroadcast()){
                    EV<< "BROADCAST"<<endl;
                    for (int g_idx = 0; g_idx < gateSize("toApps"); g_idx++) {
                        if(gate("toApps$o", g_idx)->isConnected()){
                            sendMessage(message->dup(), gate("toApps$o", g_idx));
                            EV << "Sent to toApps$o[" << g_idx << "]" << endl;
                        }
                    }
                } else {
                    int gateDest = appGateMap[dest];
                    EV << "gateId di destinazione" << gateDest<< endl;
                    if(gate(gateDest)->isConnected() && gateDest>0){
                        sendMessage(message->dup(), gate(gateDest));
                    }
                }
            }
        } else {
            sendMessage(message->dup(), gate("toSwitch$o"));
            EV << "Sent to toSwitch" << endl;
        }
    }
    delete message;
}


void MecAppDispatcher::addMap(inet::MacAddress appMacAddr, int gateId)
{
    EV << "Adding on map, mecApp" << appMacAddr << " to gateId: " << gateId << endl;
    appGateMap.insert(std::pair<inet::MacAddress, int>(appMacAddr, gateId));
}

int MecAppDispatcher::findGateId(inet::MacAddress appMacAddr)
{
    EV << "Request for gate associated with " << appMacAddr << endl;
    std::map<inet::MacAddress, int>::iterator it = appGateMap.find(appMacAddr);
    return it->second;
}

void MecAppDispatcher::eraseMap(inet::MacAddress appMacAddr)
{
    EV << "Deleting from map..." << endl;
    appGateMap.erase(appMacAddr);
}
