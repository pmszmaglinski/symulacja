#!/bin/bash
#########################################################
##### Skrypt pobiera dane dotyczące liczby wysłanych ####
##### pakietów ze wszystkich konfiguracji i node'ów  ####
##### zapisanych w skalarach		             ####
#########################################################

KONFIGURACJA=(VoIP-10ms_120B VoIP-20ms_200B VoIP-30ms_280B VoIP-50ms_440B)
> pakiety.csv

for i in "${KONFIGURACJA[@]}" # Dla każdej konfiguracji
do
	# Kryterium - konfiguracja
	echo "$i" >> pakiety.csv

	for ((j=0; j<=4; j++)) # Dla każdego node'a(5)
	do
	# Pobierz i zapisz dane
	scavetool scalar -p "module(SymScenerio.node[$j].nic.mac1609_4) AND name(\"SentPackets\")" -F csv ./$i-*.sca

	# Pobierz kolumnę z opóźnieniami
	cat _out_.csv |awk -F "," '{if (NR!=1) {print $5}}' > temp
	
	# Dodaj nagłówek
	#echo "$i $j" >> pakiety.csv
	
	# Zamień separator liczb dziesiętnych
	cat temp | tr "." "," >> pakiety.csv 
	
	# Usuń plik temp
	rm -f temp
	
	# Linia oddzielająca dane	
	#echo -e "\r" >> pakiety.csv

	done
	
done
