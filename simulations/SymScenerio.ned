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

package symulacja.simulations;

import symulacja.counter;
import symulacja.veins.nodes.RSU;
import symulacja.veins.nodes.Scenario;

network SymScenerio extends Scenario
{
    submodules:
        rsu[1]: RSU {
            @display("p=150,150;b=10,10,oval");
        }
        counter: counter {
            @display("p=240,135");
        }
}
