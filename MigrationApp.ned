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
import inet.applications.contract.IApp;

//
// PAR:
// Numero applicazione(deve esistere)
// StartTime (rimosso poi dall'orchestratore)
// From es: mecHost1
// To es: mecHost2
//

simple MigrationApp like IApp
{
    parameters:
        @signal[mecAppDeployed](type=MecApp);
        int startTime = default(0);
        int numMecApp;
        string from;
        string to;
   	gates:
   	    input socketIn;
   	    output socketOut;
}
