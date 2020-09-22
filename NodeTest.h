#include <string.h>
#include <omnetpp.h>
#include <stdio.h>

using namespace omnetpp;

class NodeTest: public cSimpleModule {
private:
    int next_mec_app = 0;

    void spawnAndConnectApp(int numApp); //create an AppTest module AND a gate, then connect the two

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};
