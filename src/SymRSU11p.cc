//
// Copyright (C) 2006-2011 Christoph Sommer <christoph.sommer@uibk.ac.at>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#include "SymRSU11p.h"

Define_Module(SymRSU11p);

void SymRSU11p::initialize(int stage) {

    sigOpNode0 = registerSignal("node0"); // Sygnal do odbioru opoznien
    sigOpNode1 = registerSignal("node1");
    sigOpNode2 = registerSignal("node2");
    sigOpNode3 = registerSignal("node3");
    sigOpNode4 = registerSignal("node4");
    sigOpALL   = registerSignal("nodeALL");

    BaseWaveApplLayer::initialize(stage);
    if (stage == 0) {
        //rxVec.setName("RX Packets");
        rxPacketVec.setName("RX Packet");
        //opoznienieVec.setName("Opoznienie");

    }
}

void SymRSU11p::onBeacon(WaveShortMessage* wsm) {

}

void SymRSU11p::onData(WaveShortMessage* wsm) {

}

void SymRSU11p::onVoIP(WaveShortMessage* wsm) {

    // Dostęp do klasy licznika
    cModule *countLink = getParentModule()->getParentModule()->getSubmodule("counter");
    counter *voipCounter = check_and_cast<counter *>(countLink);
    myId = wsm->getSenderAddress(); // Id node'a wysyłającego pakiet

    voipCounter->addRx(myId);// Dodaj otrzymany pakiet do klasy licznika
    //rxVec.record(voipCounter->getTotalRx());
    //rxVec.record(simTime()-wsm->getTimestamp());
    emit(sigOpALL, simTime()-wsm->getTimestamp());
    rxPacketVec.record(0.003);
    EV << "Otrzymałem pakiet VoIP nr " << voipCounter->getRx(myId) << " z " << voipCounter->getTx(myId) << " wysłanych od node id: " << myId << " z opóźnieniem " << simTime()-wsm->getTimestamp() << "\n";
    EV << "Stan licznikow w RSU Rx: " << voipCounter->getRx(myId) << ", Tx: " << voipCounter->getTx(myId) << " dla node id: " << myId << "\n";

    // Sumowanie opóźnień
    voipCounter->addOp(myId, simTime()-wsm->getTimestamp());

    // Pochwal się ;)
    findHost()->bubble("Dostałem Wiadomość !!!");

    //Dodawanie opźnienia do wektora
    //if (myId == 0) opoznienieVec.record(simTime()-wsm->getTimestamp());

    //Zapisywanie opóźnieniń do statystyk danego node'a
    if (myId==0) emit(sigOpNode0, simTime()-wsm->getTimestamp()); // Zapisanie czasu opóźnienia
    if (myId==1) emit(sigOpNode1, simTime()-wsm->getTimestamp());
    if (myId==2) emit(sigOpNode2, simTime()-wsm->getTimestamp());
    if (myId==3) emit(sigOpNode3, simTime()-wsm->getTimestamp());
    if (myId==4) emit(sigOpNode4, simTime()-wsm->getTimestamp());

    //Wyślij wiadomość do node'a że dotarła z Recived Time i Opóźnieniem


    //Statystyki
    //simtime_t opoznienie = simTime()-wsm->getTimestamp();
    //opoznienieStats.collect(opoznienie);
}

void SymRSU11p::sendMessage(std::string blockedRoadId) {

}
void SymRSU11p::sendWSM(WaveShortMessage* wsm) {
    sendDelayedDown(wsm,individualOffset);
}

/*
void SymRSU11p::finish() {
    // Dostęp do klasy licznika
      cModule *countLink = getParentModule()->getParentModule()->getSubmodule("counter");
      counter *voipCounter = check_and_cast<counter *>(countLink);

      recordScalar("TX",voipCounter->getTotalTx());
      recordScalar("RX",voipCounter->getTotalRx());
      opoznienieStats.recordAs("Opóźnienie");
}
*/
