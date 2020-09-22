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
import inet.node.ethernet.Eth100M;
import inet.node.inet.StandardHost;
import inet.node.inet.Router;
import inet.visualizer.integrated.IntegratedCanvasVisualizer;
import inet.node.mpls.RsvpMplsRouter;

network mpls_network
{
    parameters:
        int maxMecApp = default(1);
    submodules:
        mecApp[maxMecApp]: MecApp {
            @display("p=458,160");
        }
        configurator: ConfiguratorMecMpls {
            @display("p=53,41");
        }
        visualizer: IntegratedCanvasVisualizer {
            @display("p=122,41");
        }
        mecHost1: MecHost {
            @display("p=419,344");
        }
        mecHost2: MecHost {
            @display("p=411,219");
        }
        user: StandardHost {
            @display("p=65,219");

        }
        dummy: StandardHost {
            @display("p=419,405");

        }
        router_dummy: Router {
            @display("p=308,405");
        }
        router_ext: Router_Mec {
            parameters:
                @display("p=133,269");
                peers = "ppp1";
            gates:
                pppg[2];
        }
        router_mecHost1: Router_Mec {
            parameters:
                @display("p=295,344");
                peers = "ppp0";
            gates:
                pppg[2];
        }
        router_mecHost2: Router_Mec {
            parameters:
                @display("p=295,219");
                peers = "ppp0";
            gates:
                pppg[2];
        }
        router_backbone: RsvpMplsRouter {
            parameters:
                @display("p=229,269");
                peers = "ppp0 ppp1 ppp2";
            gates:
                pppg[3];
        }

    connections:
        router_ext.pppg[0] <--> Eth100M <--> user.pppg++;
        // <MPLS-LISP>:
        router_ext.pppg[1] <--> Eth100M <--> router_backbone.pppg[0];
        router_backbone.pppg[1] <--> Eth100M <--> router_mecHost1.pppg[0];
        router_backbone.pppg[2] <--> Eth100M <--> router_mecHost2.pppg[0];
        //</MPLS-LISP>
        router_mecHost1.pppg[1] <--> Eth100M <--> mecHost1.vEthExt;
        router_mecHost2.pppg[1] <--> Eth100M <--> mecHost2.vEthExt;

        router_mecHost1.pppg++ <--> Eth100M <--> router_dummy.pppg++;
        router_dummy.pppg++ <--> Eth100M <--> dummy.pppg++;
}
