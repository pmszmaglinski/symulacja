#!/bin/bash
#######################################################################
##### Skrypt pobiera dane dotyczące średniego opóźnienia	  #####
##### ze wszystkich konfiguracji i node'ów zapisanych w skalarach #####
#######################################################################

KONFIGURACJA=(VoIP-10ms_120B VoIP-20ms_200B VoIP-30ms_280B VoIP-50ms_440B)
> opoznienia.csv

for i in "${KONFIGURACJA[@]}" # Dla każdej konfiguracji
do

	for ((j=0; j<=4; j++)) # Dla każdego node'a(5)
	do
	# Pobierz i zapisz dane
	scavetool scalar -p "module(SymScenerio.rsu[0].appl) AND name(\"node$j:stats:mean\")" -F csv ./$i-*.sca
	
	# Pobierz kolumnę z opóźnieniami
	cat _out_.csv |awk -F "," '{if (NR!=1) {print $5}}' > temp
	
	# Dodaj nagłówek
	echo "$i $j" >> opoznienia.csv
	
	# Zamień separator liczb dziesiętnych
	cat temp | tr "." "," >> opoznienia.csv 
	
	# Usuń plik temp
	rm -f temp
	
	# Linia oddzielająca dane	
	echo -e "\r" >> opoznienia.csv

	done
	
done
