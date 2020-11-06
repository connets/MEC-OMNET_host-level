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
import inet.linklayer.acking.AckingMac;
import inet.node.inet.StandardHost;
import inet.node.inet.Router;
import inet.node.ospfv2.OspfRouter;
import inet.visualizer.integrated.IntegratedCanvasVisualizer;
import inet.node.ethernet.Eth10G;
import inet.node.ethernet.EtherSwitch;
import inet.mobility.static.StationaryMobility;

module MecApp
{
    @display("bgb=337,232");

    gates:
        inout vEth @loose;
    submodules:
        vm: StandardHost {
            @display("p=158,116");
            numEthInterfaces = 1;
            ipv4.arp.typename = "Arp_public";
        }
        mobility: StationaryMobility;   
        
    connections:
        vEth <--> Eth10G <--> vm.ethg++; 
}
