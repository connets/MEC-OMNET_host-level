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
#include "Ted_Mec.h"

Define_Module(Ted_Mec);

void Ted_Mec::initialize(int stage)
{
    Ted::initialize(stage);
}

void Ted_Mec::handleMessage(cMessage *msg)
{
    EV<<"ERROR"<<endl;
}

void Ted_Mec::rebuildRoutingTable(){
    Ted::rebuildRoutingTable();
    ConfiguratorMecMpls *configurator = check_and_cast<ConfiguratorMecMpls*>(getModuleByPath("configurator"));
    configurator->reinsert_routes();
}
