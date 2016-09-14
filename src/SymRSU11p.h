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

#ifndef SymRSU11p_H
#define SymRSU11p_H

#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"

class SymRSU11p : public BaseWaveApplLayer {
    public:
        virtual void initialize(int stage);
    protected:
        // Wektory
        cOutVector rxVec;   // Sumuje przychodzące pakiety
        cOutVector rxPacketVec; // Do zestawienia pakietyzacji
    protected:
        virtual void onBeacon(WaveShortMessage* wsm);
        virtual void onData(WaveShortMessage* wsm);
        virtual void onVoIP(WaveShortMessage* wsm); // Dodana funkcja po otrzymaniu VoIP
        void sendMessage(std::string blockedRoadId);
        virtual void sendWSM(WaveShortMessage* wsm);
        //virtual void finish();
};

#endif
