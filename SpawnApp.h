#include <string.h>
#include <omnetpp.h>
#include <stdio.h>

using namespace omnetpp;

class SpawnApp: public cSimpleModule {
private:
    int numApp = 0;

    void spawnAndConnectApp();

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
public:
    virtual ~SpawnApp();
};
