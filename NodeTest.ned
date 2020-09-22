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

simple NodeTest like IApp
{
    parameters:
        @signal[mecAppDeployed](type=MecApp);
        int localPort = default(5000);
        int apps = default(1);
        int startTime = default(0);
   	gates:
   	    input socketIn;
   	    output socketOut;
}
