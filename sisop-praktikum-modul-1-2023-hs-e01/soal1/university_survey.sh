#!/bin/bash
printf "1A.\n" 
sort -t ',' -k 1n '2023 QS World University Rankings.csv' | awk -F ',' '/JP/{print$2}' | head -5
printf "\n\n" 
printf "1B.\n" 
sort -t ',' -k 9n '2023 QS World University Rankings.csv' | awk -F ',' '/JP/{print$2}' | head -5
printf "\n\n"
printf "1C.\n" 
sort -t ',' -k 20n '2023 QS World University Rankings.csv' | awk -F ',' '/JP/{print$2}' | head -10
printf "\n\n"
printf "1D.\n" 
awk -F ',' '/Keren/{print$2}' '2023 QS World University Rankings.csv' 