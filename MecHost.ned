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
import inet.node.ethernet.Eth100M;
import inet.node.inet.StandardHost;
import inet.node.inet.Router;
import inet.node.mpls.RsvpMplsRouter;

module MecHost
{
    @display("bgb=682.5,514.8");
    gates:
        inout vEthExt;
        inout vEthInt[] @loose;

    submodules:
        mecPlatform: StandardHost {
            @display("p=538,310");
        }
        mad: MecAppDispatcher {
            @display("p=273,198.9");
        }
        vrouter: Router {
            parameters:
                @display("p=297,310");
            gates:
                pppg[1];
        }
    connections:
        vrouter.pppg[0] <--> vEthExt;  //external
        vrouter.ethg++ <--> VirtualEthernet <--> mad.toSwitch;  // to Mec App Dispatcher
        vrouter.ethg++ <--> Eth100M <--> mecPlatform.ethg++;
}

