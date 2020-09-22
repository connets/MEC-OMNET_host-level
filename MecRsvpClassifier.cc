#include "MecRsvpClassifier.h"
#include "Myrsvp.h"
#include "inet/networklayer/contract/ipv4/Ipv4Address.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/networklayer/common/L3Address.h"


Define_Module(MecRsvpClassifier);

std::vector<MecRsvpClassifier*> MecRsvpClassifier::bindings_instance = std::vector<MecRsvpClassifier*>();

void MecRsvpClassifier::initialize(int stage)
{
    RsvpClassifier::initialize(stage);

    if (stage == inet::INITSTAGE_LOCAL) {
        bindings_instance.push_back(this);
        EV << "" << bindings_instance.size() <<endl;
    }
    //    if(stage == inet::INITSTAGE_APPLICATION_LAYER){
    //        if(strcmp(this->getParentModule()->getName(),"routerMplsUser") == 0){
    //            this->add_in_table(200, 2, 100);
    //        }
    //    }
    //    if(stage == inet::INITSTAGE_LAST){
    //        EV << "" << bindings_instance.size() <<endl;
    //        for (auto & elem : bindings_instance) {
    //                EV << "cose" << elem->bindings.size() <<endl;
    //        }
    //    }

}

void MecRsvpClassifier::add_in_table(int fecid, int tunnel_id, int lspid, const char *dest)
{
    FecEntry newFec;

    newFec.id = fecid;
    newFec.dest = inet::L3AddressResolver().resolve(dest).toIpv4();
    newFec.src = inet::Ipv4Address();//getParameterIPAddressValue(fec, "source", Ipv4Address());

    newFec.session.Tunnel_Id = tunnel_id;
    newFec.session.Extended_Tunnel_Id = routerId.getInt();//getParameterIPAddressValue(fec, "extened_tunnel_id", routerId).getInt();
    newFec.session.DestAddress = newFec.dest;    // ??? always use newFec.dest ???

    newFec.sender.Lsp_Id = lspid;
    newFec.sender.SrcAddress = routerId;

    Myrsvp *myrsvp = new Myrsvp();
    myrsvp->Myrsvp_set(rsvp);

    // INCREMENTALE SUL NUMERO DI SESSIONI?

    newFec.inLabel = 7;//myrsvp->getInLabel_public(newFec.session, newFec.sender);

    for (auto elem : bindings_instance) {
        auto it = elem->findFEC(fecid);

        if (it == elem->bindings.end()) {
            EV << "Aggiunta nuova!"<<endl;
            // create new binding
            elem->bindings.push_back(newFec);
        }
        else {
            EV << "DEVO FARE QUESTO!"<<endl;
            // update existing binding
            *it = newFec;
        }
    }

    //    if (it == bindings.end()) {
    //        // create new binding
    //        bindings.push_back(newFec);
    //    }
    //    else {
    //        // update existing binding
    //        *it = newFec;
    //    }
}

void MecRsvpClassifier::remove_from_table(int fecid){
    for (auto elem : bindings_instance) {
        auto it = elem->findFEC(fecid);
        if (it == elem->bindings.end()) {
            // ignore
            continue;
        }
        else {
            EV << "CANCELLO!"<<endl;
            elem->bindings.erase(it);
        }
    }
}

