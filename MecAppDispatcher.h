/*
 * MecAppDispatcher.h
 *
 *  Created on: 20 mag 2020
 *      Author: chris
 */
#include <omnetpp.h>
#include <map>
#include "MadMessage_m.h"
#include "inet/linklayer/common/MacAddress.h"

using namespace omnetpp;

class MecAppDispatcher : public cSimpleModule {
private:
    void sendMessage(cMessage* msg, cGate* gate);
    std::map <inet::MacAddress,int> appGateMap;


public:
    MecAppDispatcher();
    virtual ~MecAppDispatcher();
    virtual void handleMessage(cMessage *msg) override;
    void addMap(inet::MacAddress appMacAddr, int gateId);
    int findGateId(inet::MacAddress appMacAddr);
    void eraseMap(inet::MacAddress appMacAddr);
};
