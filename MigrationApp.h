#include <omnetpp.h>
#include <string.h>
#include <stdio.h>

using namespace omnetpp;

class MigrationApp : public cSimpleModule
{
  protected:
    int numMecApp;
    std::string from;
    std::string to;

    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
  private:
    void migrate();
    void route_label();
};
