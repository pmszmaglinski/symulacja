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

#include <counter.h>

counter::counter(){
    // TODO Auto-generated constructor stub
}

counter::~counter() {
    // TODO Auto-generated destructor stub
}

// int c - id node w tablicy
// int r - ilość pakietów

void counter::setRx(int r, int c) {
    this->rx[c] = r;
}

int counter::getRx(int c) {
    Enter_Method_Silent();
    return this->rx[c];
}

void counter::setTx (int r, int c) {
    Enter_Method_Silent();
    this->tx[c] = r;
}

int counter::getTx (int c) {
    Enter_Method_Silent();
   return this->tx[c];
}

void counter::addRx(int c) {
    Enter_Method_Silent();
    this->rx[c]+=1;
}

void counter::addTx(int c) {
    Enter_Method_Silent();
    this->tx[c]+=1;
}

int counter::getTotalRx() {
    Enter_Method_Silent();
    int totalRx = 0;
    for (int x = 0; x <= tabSize-1; x++){
        totalRx += rx[x];
    }
    return totalRx;
}

int counter::getTotalTx() {
    Enter_Method_Silent();
    int totalTx = 0;
    for (int x = 0; x <= tabSize-1; x++){
         totalTx += tx[x];
    }
    return totalTx;
}

float counter::getPerTotal() { // Obliczenie stosunku pakietów które niedotarły do celu do ogólnej liczby wysłanych pakietów
    Enter_Method_Silent();
    return (float)((getTotalTx()-getTotalRx())*100)/getTotalTx();
}

float counter::getPer(int c) {
    Enter_Method_Silent();
    return (float)((getTx(c)-getRx(c))*100)/getTx(c);
}

void counter::addOp(int c, simtime_t t) {
    Enter_Method_Silent();
    this->op[c]+=t;
}

simtime_t counter::getSrOp(int c) {
    Enter_Method_Silent();
    return op[c]/getRx(c);
}

simtime_t counter::getSrOpTotal() {
    Enter_Method_Silent();
    simtime_t totalOp = 0;
        for (int x = 0; x <= tabSize-1; x++){
             totalOp += getSrOp(x);
        }
        return totalOp/tabSize;
}


