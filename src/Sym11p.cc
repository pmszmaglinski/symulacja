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

#include "Sym11p.h"

using Veins::TraCIMobilityAccess;

Define_Module(Sym11p);

void Sym11p::initialize(int stage) {
    BaseWaveApplLayer::initialize(stage);
    if (stage == 0) {
        mobility = TraCIMobilityAccess().get(getParentModule());
        traci = mobility->getCommandInterface();
        traciVehicle = mobility->getVehicleCommandInterface();

    }
}

void Sym11p::onBeacon(WaveShortMessage* wsm) {

}

void Sym11p::onData(WaveShortMessage* wsm) {

}

void Sym11p::onVoIP(WaveShortMessage* wsm) {

}

void Sym11p::sendMessage(std::string blockedRoadId) {

}
void Sym11p::receiveSignal(cComponent* source, simsignal_t signalID, cObject* obj, cObject* details) {

}
void Sym11p::handleParkingUpdate(cObject* obj) {

}
void Sym11p::handlePositionUpdate(cObject* obj) {

}
void Sym11p::sendWSM(WaveShortMessage* wsm) {
    sendDelayedDown(wsm,individualOffset);
}
