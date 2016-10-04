#!/bin/bash

#############################################################
# Przetwarzanie danych interwałów międzykanałowych,         #
# czasu pakietyzacji oraz dostarczenia z Omnet++ do gnuplot #
#############################################################

KONFIGURACJA=(VoIP-10ms_120B VoIP-20ms_200B VoIP-30ms_280B VoIP-50ms_440B)


for i in "${KONFIGURACJA[@]}" # Dla każdej konfiguracji
do

	for ((j=0; j<=0; j++)) # Tylko dla node 0
	do

OUTFILE=$i"-"$j.csv
################################
# Wydobycie interwałów kanałów #
################################
scavetool vector -p "module(SymScenerio.node[$j].nic.mac1609_4) AND name(\"Interwały kanałów\")" -F csv ./$i-0.vec

echo -e "Zmienne ze skryptu...\r";
echo -e "Konfiguracja\t $i\r";
echo -e "Node\t $j\r"
echo -e "OUTFILE\t $OUTFILE\r";
echo -e "\n";

# Zmiana separatorów
awk -F "," '{print $1,$2}' _out_.csv > temp

# Nagłówek SCH
echo "#SCH" > $OUTFILE

# Parsowanie  punktów SCH
awk 'NR==1 { next }; NR <= 19 && $2 ~ /0.001/ {print; print $1+0.050, $2; print "\r"};' temp >> $OUTFILE

# Oddzielenie bloków danych gnuplot
echo -e "\n" >> $OUTFILE

# Nagłówek CCH
echo "#CCH" >> $OUTFILE

# Parsowanie punktów CCH
awk 'NR==1 { next }; NR <= 19 && $2 ~ /0.002/ {print; print $1+0.050, $2; print "\r"};' temp >> $OUTFILE

rm -f temp

################################
# Wydobycie czasu pakietyzacji #
################################
scavetool vector -p "module(SymScenerio.node[$j].appl) AND name(\"Pakietyzacja\")" -F csv ./$i-0.vec

# Zmiana separatorów
awk -F "," '{print $1,$2}' _out_.csv > temp

# Oddzielenie bloków danych gnuplot
echo -e "\n" >> $OUTFILE

# Nagłówek TX
echo "#TX" >> $OUTFILE

# Parsowanie danych dla gnuplot
awk '$1 <= 1.10 {print $1,"0.003"}' temp >> $OUTFILE

rm -f temp

#######################################
# Pobranie czasu dostarczenia pakietu #
#######################################
scavetool vector -p "module(SymScenerio.rsu[0].appl) AND name(\"node$j:vector\")" -F csv ./$i-0.vec

# Zmiana separatorów
awk -F "," '{print $1,$2}' _out_.csv > temp

# Oddzielenie bloków danych gnuplot
echo -e "\n" >> $OUTFILE

# Nagłówek TX
echo "#Dostarczenie" >> $OUTFILE

# Parsowanie danych dla gnuplot
awk '$1 <= 1.10 {print $1,"0.004"}' temp >> $OUTFILE

rm -f temp

done
done
