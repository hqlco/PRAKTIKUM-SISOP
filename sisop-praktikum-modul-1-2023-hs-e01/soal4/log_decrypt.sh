#!/bin/bash

#0 */2 * * * /bin/bash /home/rosy/sisop_modul1/soal4/log_encrypt.sh

read input


x=$(echo $input |cut -c 1)
y=$(echo $input |cut -c 2)
jam=$((10*$x+$y))
asli="abcdefghijklmnopqrstuvwxyz"
asli2="ABCDEFGHIJKLMNOPQRSTUVWXYZ"

enkrip=
for ((i=0; i<${#asli} ; i++))
do
	char=${asli:$i:1}
	asci=$(printf '%d' "'$char")
	a=97
	enkrip_asci=$((10#$asci - 10#$jam - 10#$a ))
	b=26
    enkrip_asci=$((10#$enkrip_asci + 10#$b ))
	enkrip_asci=$((10#$enkrip_asci % 10#$b))
	enkrip_asci=$((10#$enkrip_asci + 10#$a ))
	enkrip2=$(printf \\$(printf '%03o' "$enkrip_asci"))
	enkrip+=$enkrip2
done

enkrip2=
for ((i=0; i<${#asli2} ; i++))
do
        char2=${asli2:$i:1}
        asci2=$(printf '%d' "'$char2")
        a2=65
        enkrip_asci2=$((10#$asci2 - 10#$jam - 10#$a2 ))
        b2=26
        enkrip_asci2=$((10#$enkrip_asci2 + 10#$b2 ))
        enkrip_asci2=$((10#$enkrip_asci2 % 10#$b2))
        enkrip_asci2=$((10#$enkrip_asci2 + 10#$a2 ))
        enkrip22=$(printf \\$(printf '%03o' "$enkrip_asci2"))
        enkrip2+=$enkrip22
done
cat "$input" | tr "${asli}" "${enkrip}" | tr "${asli2}" "${enkrip2}" > "/home/rosy/sisop_modul1/soal4/asli.txt"
