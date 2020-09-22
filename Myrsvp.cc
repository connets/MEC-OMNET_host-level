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

#include "Myrsvp.h"

Define_Module(Myrsvp);

void Myrsvp::Myrsvp_set(RsvpTe *rsvp){
    myrsvp = rsvp;
}

int Myrsvp::getInLabel_public(const inet::SessionObj& session, const inet::SenderTemplateObj& sender)
{
    return RsvpTe::getInLabel(session, sender);
}

void Myrsvp::insert_session(){
//    traffic_session_t newSession;
//
//    newSession.sobj.Tunnel_Id = tunnel_id;
//    newSession.sobj.Extended_Tunnel_Id = routerId.getInt();
//    newSession.sobj.DestAddress = destAddr;//getParameterIPAddressValue(session, "endpoint");
//
//    auto sit = findSession(newSession.sobj);
//
//    bool merge;
//
//    if (sit != traffic.end()) {
//        // session already exits, add new paths
//
//        merge = true;
//
//        newSession.sobj.setupPri = sit->sobj.setupPri;
//        newSession.sobj.holdingPri = sit->sobj.holdingPri;
//
//        sit->sobj = newSession.sobj;
//    }
//    else {
//        // session not found, create new
//
//        merge = false;
//
//        newSession.sobj.setupPri = 7;//getParameterIntValue(session, "setup_pri", 7);
//        newSession.sobj.holdingPri = 7;//getParameterIntValue(session, "holding_pri", 7);
//    }
//
//    const cXMLElement *paths = getUniqueChild(session, "paths");
//    checkTags(paths, "path");
//
//    cXMLElementList list = paths->getChildrenByTagName("path");
//    for (auto path : list) {
//        checkTags(path, "sender lspid bandwidth max_delay route permanent owner color");
//
//        int lspid = lspid;//getParameterIntValue(path, "lspid");
//        ;
//
//        std::vector<traffic_path_t>::iterator pit;
//
//        traffic_path_t newPath;
//
//        newPath.sender.SrcAddress = getParameterIPAddressValue(path, "sender", routerId);
//        newPath.sender.Lsp_Id = lspid;
//
//        // make sure path doesn't exist yet
//
//        if (merge) {
//            pit = findPath(&(*sit), newPath.sender);
//            if (pit != sit->paths.end()) {
//                EV_DETAIL << "path " << lspid << " already exists in this session, doing nothing" << endl;
//                continue;
//            }
//        }
//        else {
//            pit = findPath(&newSession, newPath.sender);
//            if (pit != newSession.paths.end()) {
//                EV_INFO << "path " << lspid << " already exists in this session, doing nothing" << endl;
//                continue;
//            }
//        }
//
//        const char *str = getParameterStrValue(path, "owner", "");
//        if (strlen(str)) {
//            cModule *mod = getModuleByPath(str);
//            newPath.owner = mod->getId();
//        }
//        else {
//            newPath.owner = getId();
//        }
//
//        newPath.permanent = getParameterBoolValue(path, "permanent", true);
//        newPath.color = getParameterIntValue(path, "color", 0);
//
//        newPath.tspec.req_bandwidth = getParameterDoubleValue(path, "bandwidth", 0.0);
//        newPath.max_delay = getParameterDoubleValue(path, "max_delay", 0.0);
//
//        const cXMLElement *route = getUniqueChildIfExists(path, "route");
//        if (route)
//            newPath.ERO = readTrafficRouteFromXML(route);
//
//        if (merge) {
//            EV_INFO << "adding new path into an existing session" << endl;
//
//            sit->paths.push_back(newPath);
//        }
//        else {
//            EV_INFO << "adding new path into new session" << endl;
//
//            newSession.paths.push_back(newPath);
//        }
//
//        // schedule path creation
//
//        sendPathNotify(getId(), newSession.sobj, newPath.sender, inet::PATH_RETRY, 0.0);
//    }
//
//    if (!merge) {
//        EV_INFO << "adding new session into database" << endl;
//
//        traffic.push_back(newSession);
//    }
}
