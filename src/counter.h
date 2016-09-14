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

#ifndef COUNTER_H_
#define COUNTER_H_

using namespace omnetpp;
const int tabSize = 5;

// int c - id node w tablicy
// int r - ilość pakietów

class counter : public cSimpleModule {
private:
    int tx[tabSize] = {0,0,0,0,0};  // Tablica wysłanych pakietów dla danego node - bez inicjacji zerami wyniki PER są fałszywe
    int rx[tabSize] = {0,0,0,0,0};  // Tablica odebranych pakietów dla danego node - bez inicjacji zerami wyniki PER są fałszywe
    simtime_t op[tabSize] = {0,0,0,0,0};    // Średnie opóźnienie dla danego node - bez inicjacji zerami wyniki PER są fałszywe

protected:

public:
    // Pakiety
    int getRx(int c);
    void setRx(int r, int c);
    int getTx(int c);
    void setTx(int t, int c);
    void addRx(int c);
    void addTx(int c);
    int getTotalRx();
    int getTotalTx();

    // PER
    float getPerTotal();
    float getPer(int c);     // Per dla pojedynczego node'a

    // Opóźnienia
    void addOp(int c, simtime_t t); // Dodaj do sumy opóźnień danego node'a
    simtime_t getSrOp(int c);  // Pobierz średnie opóźnienie dla danego node'a
    simtime_t getSrOpTotal();   // Pobierz całkowite średnie opóźnienie dla wszystkich node'ów

    counter();
    virtual ~counter();
};

Define_Module(counter);
#endif /* COUNTER_H_ */
