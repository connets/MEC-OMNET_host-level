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
#include <omnetpp.h>
#include <iostream>
#include <vector>

#include<inet/networklayer/rsvpte/RsvpClassifier.h>

using namespace omnetpp;

class MecRsvpClassifier : public inet::RsvpClassifier
{
private:
    static std::vector<MecRsvpClassifier*> bindings_instance;
protected:
    void initialize(int numstage) override;
public:
    void remove_from_table(int fecid);
    void add_in_table(int fecid, int tunnel_id, int lspid, const char *dest);
};
