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



import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.inet.INetworkNode;
import inet.physicallayer.contract.packetlevel.IRadioMedium;
import inet.physicallayer.unitdisk.UnitDiskRadioMedium;
import inet.node.ethernet.Eth100M;
import inet.node.inet.StandardHost;
import inet.node.inet.WirelessHost;
import inet.node.inet.Router;
import inet.node.ospfv2.OspfRouter;
import inet.node.mpls.RsvpMplsRouter;
import inet.node.ethernet.EtherSwitch;
import inet.visualizer.integrated.IntegratedCanvasVisualizer;
import inet.common.scenario.ScenarioManager;


network NetworkApplicationTest
{
    parameters:
        int maxMecApp = default(1);

        @display("bgb=764.39996,572");
    submodules:
        mecApp[maxMecApp]: MecApp {
            @display("p=568.1,132.59999");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=53,41");
        }

        nfvi1: StandardHost {
            @display("p=619,327");
        }

        user: StandardHost {
            @display("p=81,327");

        }
        router: RsvpMplsRouter {
            parameters:
                peers = "ppp0";
                @display("p=284,327");
            gates:
                pppg[3];
        }
        vSwitch1: vSwitch {
            @display("p=442,327");
        }
        visualizer: IntegratedCanvasVisualizer {
            @display("p=178,41");
        }
        vSwitch2: vSwitch {
            @display("p=325,425.1");
        }
        nfvi2: StandardHost {
            @display("p=442,501.8");
        }
    connections:
        nfvi1.pppg++ <--> Eth100M <--> vSwitch1.vEthMng;
        router.pppg[0] <--> Eth100M <--> user.pppg++;
        vSwitch1.vEthExt <--> Eth100M <--> router.pppg[1];

        nfvi2.pppg++ <--> Eth100M <--> vSwitch2.vEthMng;
        vSwitch2.vEthExt <--> Eth100M <--> router.pppg[2];
}

