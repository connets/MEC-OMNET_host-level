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
import inet.node.ethernet.EtherSwitch;
import inet.visualizer.integrated.IntegratedCanvasVisualizer;
import inet.common.scenario.ScenarioManager;
import inet.node.mpls.RsvpMplsRouter;

network NetworkApplicationTest_mecmodule
{
    parameters:
        int maxMecApp = default(1);
    submodules:
        mecApp[maxMecApp]: MecApp {
            @display("p=458,160");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=53,41");
        }
        visualizer: IntegratedCanvasVisualizer {
            @display("p=178,41");
        }
        mecHost: MecHost {
            @display("p=459.64798,326.016");
        }
        user: StandardHost {
            @display("p=81,327");

        }
        router: RsvpMplsRouter {
            parameters:
                @display("p=284,327");
                peers = "ppp1";
            gates:
                pppg[2];
        }

    connections:
        router.pppg[0] <--> Eth100M <--> user.ethg++;
        router.pppg[1] <--> Eth100M <--> mecHost.vEthExt;
}

