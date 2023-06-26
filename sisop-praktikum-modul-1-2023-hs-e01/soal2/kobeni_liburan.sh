#!/bin/bash

download() {
    hour=$(date +%H)
    num_images=$((hour == 0 ? 1 : hour))

    cd /home/ikikk/Documents/sisop-praktikum-modul-1-2023-hs-e01/soal2

    if [ ! -f "numFolders.log" ]; then
        touch numFolders.log
        echo "0" > numFolders.log
    fi 

    num_folders=$(<numFolders.log)

    # num_folders=$(ls -d kumpulan_* | wc -l)
    foldername="kumpulan_$((num_folders + 1))"
    mkdir "$foldername"

    for i in $(seq 1 $num_images); do
        filename="perjalanan_$i.jpg"
        wget --no-check-certificate --output-document="$foldername/$filename" "https://source.unsplash.com/1600x900/?indonesia"    
    done

    num_folders=$(($num_folders+1))
    echo $num_folders > numFolders.log
}

zipping() {
    cd /home/ikikk/Documents/sisop-praktikum-modul-1-2023-hs-e01/soal2
    if [ ! -f "devil_1.zip" ]; then
        filename="devil_1.zip"
    else
        num_file=$(ls -f devil_*.zip | wc -l)
        filename="devil_$((num_file + 1)).zip"
    fi

    zip -rm $filename kumpulan_*

}

"$@"

#crontab -e
# 0 */10 * * * bash /home/ikikk/Documents/sisop-praktikum-modul-1-2023-hs-e01/soal2/kobeni_liburan.sh download
# 0 0 * * * bash /home/ikikk/Documents/sisop-praktikum-modul-1-2023-hs-e01/soal2/kobeni_liburan.sh zipping
