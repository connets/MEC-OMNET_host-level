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

#include "ConfiguratorMecMpls.h"
#include "inet/common/XMLUtils.h"
using namespace inet;

Define_Module(ConfiguratorMecMpls);

void ConfiguratorMecMpls::initialize(int stage)
{
    Ipv4NetworkConfigurator::initialize(stage);
    if(stage == inet::INITSTAGE_LAST){
        reinsert_routes();
        rsvp_insert();
    }
}

void ConfiguratorMecMpls::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
}

void ConfiguratorMecMpls::rsvp_insert(){
    //itera su tutti i moduli
    //i router_MecHost aggiungo gli user
    // i router_ext aggiungo le mec_app
    Matcher atMatcher("router_ext");
    for (int i = 0; i < topology.getNumNodes(); i++) {
        Node *node = (Node *)topology.getNode(i);
        if (node->routingTable) {
            std::string hostFullPath = node->module->getFullPath();
            std::string hostShortenedFullPath = hostFullPath.substr(hostFullPath.find('.') + 1);
            if (atMatcher.matches(hostShortenedFullPath.c_str()) || atMatcher.matches(hostFullPath.c_str())) {
                EV << "Full: " << hostFullPath << " \n Shortened: " << hostShortenedFullPath << endl;
//                cXMLElement *element =
//                node->rsvp->readTrafficSessionFromXML(element);
//                check_and_cast <MyRsvp *>;
            }
        }
    }

}

void ConfiguratorMecMpls::reinsert_routes(){
    cXMLElementList routeElements = configuration->getChildrenByTagName("route");
    for (auto & routeElement : routeElements) {
        const char *hostAttr = xmlutils::getMandatoryFilledAttribute(*routeElement, "hosts");
        const char *destinationAttr = xmlutils::getMandatoryAttribute(*routeElement, "destination");    // destination address  (L3AddressResolver syntax)
        const char *netmaskAttr = routeElement->getAttribute("netmask");    // default: 255.255.255.255; alternative notation: "/23"
        const char *gatewayAttr = routeElement->getAttribute("gateway");    // next hop address (L3AddressResolver syntax)
        const char *interfaceAttr = routeElement->getAttribute("interface");    // output interface name
        const char *metricAttr = routeElement->getAttribute("metric");

        try {
            // parse and check the attributes
            Ipv4Address destination;
            if (!isEmpty(destinationAttr) && strcmp(destinationAttr, "*"))
                destination = resolve(destinationAttr, L3AddressResolver::ADDR_IPv4).toIpv4();
            Ipv4Address netmask;
            if (!isEmpty(netmaskAttr) && strcmp(netmaskAttr, "*")) {
                if (netmaskAttr[0] == '/')
                    netmask = Ipv4Address::makeNetmask(atoi(netmaskAttr + 1));
                else
                    netmask = Ipv4Address(netmaskAttr);
            }
            if (!netmask.isValidNetmask())
                throw cRuntimeError("Wrong netmask %s", netmask.str().c_str());
            if (isEmpty(interfaceAttr) && isEmpty(gatewayAttr))
                throw cRuntimeError("Incomplete route: either gateway or interface (or both) must be specified");

            // find matching host(s), and add the route
            Matcher atMatcher(hostAttr);
            for (int i = 0; i < topology.getNumNodes(); i++) {
                Node *node = (Node *)topology.getNode(i);
                if (node->routingTable) {
                    std::string hostFullPath = node->module->getFullPath();
                    std::string hostShortenedFullPath = hostFullPath.substr(hostFullPath.find('.') + 1);
                    if (atMatcher.matches(hostShortenedFullPath.c_str()) || atMatcher.matches(hostFullPath.c_str())) {
                        // determine the gateway (its address towards this node!) and the output interface for the route (must be done per node)
                        InterfaceEntry *ie;
                        Ipv4Address gateway;
                        resolveInterfaceAndGateway(node, interfaceAttr, gatewayAttr, ie, gateway, topology);

                        // create and add route
                        Ipv4Route *route = new Ipv4Route();
                        route->setSourceType(IRoute::MANUAL);
                        route->setDestination(destination);
                        route->setNetmask(netmask);
                        route->setGateway(gateway);    // may be unspecified
                        route->setInterface(ie);
                        if (isNotEmpty(metricAttr))
                            route->setMetric(atoi(metricAttr));
                        node->routingTable->addRoute(route);
                    }
                }
            }
        }
        catch (std::exception& e) {
            throw cRuntimeError("Error in XML <route> element at %s: %s", routeElement->getSourceLocation(), e.what());
        }
    }
}
