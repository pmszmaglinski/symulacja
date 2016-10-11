//
// Copyright (C) 2011 David Eckhoff <eckhoff@cs.fau.de>
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

#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"


const simsignalwrap_t BaseWaveApplLayer::mobilityStateChangedSignal = simsignalwrap_t(MIXIM_SIGNAL_MOBILITY_CHANGE_NAME);


void BaseWaveApplLayer::initialize(int stage) {
	BaseApplLayer::initialize(stage);

	if (stage==0) {
		myMac = FindModule<WaveAppToMac1609_4Interface*>::findSubModule(getParentModule());
		assert(myMac);

		myId = getParentModule()->getIndex();

		headerLength = par("headerLength").longValue();
		double maxOffset = par("maxOffset").doubleValue();
		sendBeacons = par("sendBeacons").boolValue();
		beaconLengthBits = par("beaconLengthBits").longValue();
		beaconPriority = par("beaconPriority").longValue();

		sendData = par("sendData").boolValue();
		dataLengthBits = par("dataLengthBits").longValue();
		dataOnSch = par("dataOnSch").boolValue();
		dataPriority = par("dataPriority").longValue();

		sendBeaconEvt = new cMessage("beacon evt", SEND_BEACON_EVT);

		// Zmienne VoIP
		sendVoIPEvt = new cMessage("VoIP evt", SEND_VoIP_EVT);  //Dodanie inicjacji zmiennej wiadomości VoIP do harmonogramu pakietyzacji
		sendVoIP = par("sendVoIP").boolValue(); // Czy dany moduł ma pakietyzować (harmonogramować) VoIP
		voipLengthBits = par("voipLengthBits").longValue(); // Długość wiadomości VoIP
		voipPriority = par("voipPriority").longValue(); // Priorytet VoIP (1)

		// Wektory
		txVec.setName("TX Packets");
		txTotalVec.setName("TX Total");
		pakietyzacjaVec.setName("Pakietyzacja");


		//simulate asynchronous channel access
		double offSet = dblrand() * (par("beaconInterval").doubleValue()/2);
		//double offSet = dblrand() * (par("voipInterval").doubleValue()/2);

		offSet = offSet + floor(offSet/0.050)*0.050;
		individualOffset = dblrand() * maxOffset;

		findHost()->subscribe(mobilityStateChangedSignal, this);

		if (sendBeacons) {
			scheduleAt(simTime() + offSet, sendBeaconEvt);
		}

		if (sendVoIP) { //Dodana zmienna spradzająca czy wysyłającym jest node (nie RSU)
		// Czy nie dodać asynchrona również dla VoIP ?
		// Po dadaniu offSet PER zszedł z 37 na 25 % (Różny czas pakietyzacji w poszczególnych modułach)
		scheduleAt(simTime()+par("voipInterval") + offSet, sendVoIPEvt);
		}

	}
}

WaveShortMessage*  BaseWaveApplLayer::prepareWSM(std::string name, int lengthBits, t_channel channel, int priority, int rcvId, int serial) {
	WaveShortMessage* wsm =	new WaveShortMessage(name.c_str());
	//wsm->addBitLength(headerLength);
	//wsm->addBitLength(lengthBits);
	wsm->setBitLength(lengthBits);

	switch (channel) {
		case type_SCH: wsm->setChannelNumber(Channels::SCH1); break; //will be rewritten at Mac1609_4 to actual Service Channel. This is just so no controlInfo is needed
		case type_CCH: wsm->setChannelNumber(Channels::CCH); break;
	}
	wsm->setPsid(0);
	wsm->setPriority(priority);
	wsm->setWsmVersion(1);
	wsm->setTimestamp(simTime());
	wsm->setSenderAddress(myId);
	wsm->setRecipientAddress(rcvId);
	wsm->setSenderPos(curPosition);
	wsm->setSerial(serial);

	if (name == "beacon") {
		DBG << "Creating Beacon with Priority " << priority << " at Applayer at " << wsm->getTimestamp() << std::endl;
	}
	if (name == "data") {
		DBG << "Creating Data with Priority " << priority << " at Applayer at " << wsm->getTimestamp() << std::endl;
	}
	if (name == "voip") {
	    DBG << "Creating VoIP with Priority " << priority << " at Applayer at " << wsm->getTimestamp() << " Długość: " << wsm->getBitLength() << std::endl;
	    }

	return wsm;
}

void BaseWaveApplLayer::receiveSignal(cComponent* source, simsignal_t signalID, cObject* obj, cObject* details) {
	Enter_Method_Silent();
	if (signalID == mobilityStateChangedSignal) {
		handlePositionUpdate(obj);
	}
}

void BaseWaveApplLayer::handlePositionUpdate(cObject* obj) {
	ChannelMobilityPtrType const mobility = check_and_cast<ChannelMobilityPtrType>(obj);
	curPosition = mobility->getCurrentPosition();
}

void BaseWaveApplLayer::handleLowerMsg(cMessage* msg) {

	WaveShortMessage* wsm = dynamic_cast<WaveShortMessage*>(msg);
	ASSERT(wsm);

	if (std::string(wsm->getName()) == "beacon") {
		onBeacon(wsm);
	}
	else if (std::string(wsm->getName()) == "data") {
		onData(wsm);
	}
	else if (std::string(wsm->getName()) == "voip") {   // Dodane - co zrobić z pakietem VoIP
	        onVoIP(wsm);
	    }
	else {
		DBG << "unknown message (" << wsm->getName() << ")  received\n";
	}
	delete(msg);
}

void BaseWaveApplLayer::handleSelfMsg(cMessage* msg) {
	switch (msg->getKind()) {
		case SEND_BEACON_EVT: {
			sendWSM(prepareWSM("beacon", beaconLengthBits, type_CCH, beaconPriority, 0, -1));
			scheduleAt(simTime() + par("beaconInterval").doubleValue(), sendBeaconEvt);
			break;
		}
		case SEND_VoIP_EVT: {   //Dodana akcja - wysłanie pakietu VoIP i harmonogramowanie kolejnej wysyłki

		    myId = getParentModule()->getIndex();

		    cModule *countLink = getParentModule()->getParentModule()->getSubmodule("counter"); // Wydobycie linka do modułu counter
		    counter *voipCounter = check_and_cast<counter *>(countLink);    // Rzutowanie zmiennej linka klasy cModule na typ counter i przypisanie do zmiennej


		    //sendWSM(prepareWSM("voip", voipLengthBits, type_SCH, voipPriority, 0, -1)); // Wysłanie pakietu VoIP
		    // Rzutowanie cMessage (WSM) na cPacket - dostęp do zmiennych długości pakietu
		    //WaveShortMessage* WSM = prepareWSM("voip", voipLengthBits, type_SCH, voipPriority, 0, -1);
		    //cPacket* cWSM = check_and_cast<cPacket *>(WSM);
		    //cWSM->addBitLength(0);
		    //EV << "Długość pakietu: " << cWSM->getBitLength() << "\n";

		    sendWSM(prepareWSM("voip", voipLengthBits, type_SCH, voipPriority, 0, -1));
		    voipCounter->addTx(myId);   // Inkrementacja wysłanych pakietów
		    txVec.record(voipCounter->getTx(myId)); // Zliczenie wysyłki pakietu do wektora - poszczególny node
		    txTotalVec.record(voipCounter->getTotalTx()); // Zliczanie wszystkich wysłanych pakietów
		    pakietyzacjaVec.record(1.002);

		    EV << "Tu wysłałem pakiet VoIP nr " << voipCounter->getTx(myId) << "\n";


		    scheduleAt(simTime() + par("voipInterval").doubleValue(), sendVoIPEvt); // Harmonogramowanie kolejnego interwału pakietyzacji (20 lub 30 ms)

		    EV << "Stan licznikow w BWAL Tx: " << voipCounter->getTx(myId) << ", Rx: " << voipCounter->getRx(myId) << " dla id: "<< myId << "\n\n";

		    break;
		}
		default: {
			if (msg)
				DBG << "APP: Error: Got Self Message of unknown kind! Name: " << msg->getName() << endl;
			break;
		}
	}
}

void BaseWaveApplLayer::sendWSM(WaveShortMessage* wsm) {
	sendDelayedDown(wsm,individualOffset);
}

void BaseWaveApplLayer::finish() {
	if (sendVoIPEvt->isScheduled()) {
		cancelAndDelete(sendVoIPEvt);
	}
	else {
		delete sendVoIPEvt;
	}

	//Dostęp do klasy voipCounter
	myId = getParentModule()->getIndex();
    cModule *countLink = getParentModule()->getParentModule()->getSubmodule("counter");
    counter *voipCounter = check_and_cast<counter *>(countLink);

    // Inne statystyki dla node i dla rsu
    if (std::string(this->getParentModule()->getName()) == "node") {
	EV << "Wywołuję funkcję finish() dla modułu: "<< this->getParentModule()->getName() << myId << "\n";
	EV << "Pakiety wysłane: " << voipCounter->getTx(myId) << ", odebrane: " << voipCounter->getRx(myId) << ".\n";

	//Zapisanie skalarów
	recordScalar("TX Packets", voipCounter->getTx(myId));
	recordScalar("RX Packets", voipCounter->getRx(myId));
	recordScalar("PER", voipCounter->getPer(myId));
	recordScalar("Średnie opóźnienie", voipCounter->getSrOp(myId));
    }

    else if (std::string(this->getParentModule()->getName()) == "rsu") {
    EV << "Wywołuję funkcę finish() dla modułu: " << this->getParentModule()->getName() << "\n";
    EV << "Suma wysłanych pakietów: " << voipCounter->getTotalTx() << "\n";
    EV << "Suma odebranych pakietów: " << voipCounter->getTotalRx() << "\n";
    EV << "PER wynosi: " << voipCounter->getPerTotal() << "%\n";
    EV << "Średnie opóźnienie wynosi: " << voipCounter->getSrOpTotal() << "\n";

    //Zaspianie skalarów
    recordScalar("Total TX Packets", voipCounter->getTotalTx());
    recordScalar("Total RX Packets", voipCounter->getTotalRx());
    recordScalar("PER Total", voipCounter->getPerTotal());
    recordScalar("Całkowite średnie opóźnienie", voipCounter->getSrOpTotal());
    }

    else {
        EV << "Nieznany moduł: " << std::string(this->getParentModule()->getName()) << "\n";
    }


	findHost()->unsubscribe(mobilityStateChangedSignal, this);

}

BaseWaveApplLayer::~BaseWaveApplLayer() {

}
