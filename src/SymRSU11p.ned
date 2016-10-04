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

package symulacja;
import symulacja.veins.modules.application.ieee80211p.BaseWaveApplLayer;

simple SymRSU11p extends BaseWaveApplLayer
{
	@signal[node0](type=simtime_t);
	@signal[node1](type=simtime_t);
	@signal[node2](type=simtime_t);
	@signal[node3](type=simtime_t);
	@signal[node4](type=simtime_t);
	@signal[nodeALL](type=simtime_t);
	@statistic[node0](title="Opoznienia node0"; source="node0"; record=vector,stats,histogram);
	@statistic[node1](title="Opoznienia node1"; source="node1"; record=vector,stats,histogram);
	@statistic[node2](title="Opoznienia node2"; source="node2"; record=vector,stats,histogram);
	@statistic[node3](title="Opoznienia node3"; source="node3"; record=vector,stats,histogram);
	@statistic[node4](title="Opoznienia node4"; source="node4"; record=vector,stats,histogram);
	@statistic[nodeALL](title="Opoznienia ALL"; source="nodeALL"; record=vector,stats,histogram);
	
    @class(SymRSU11p);
    @display("i=block/app2");
}