# sisop-praktikum-modul-1-2023-HS-E01

| Nama                      | NRP           |Username           |
|---------------------------|---------------|--------------|
|Moh Rosy Haqqy Aminy       |5025211012     |rosyhaqqy          |
|Rule Lulu Damara           |5025211050     |rulelulu           |
|Rizky Alifiyah Rahma       |5025211209     |ikikk              |


## Soal 1
```text
Bocchi hendak melakukan University Admission Test di Jepang. Bocchi ingin masuk ke universitas yang bagus. 
Akan tetapi, dia masih bingung sehingga ia memerlukan beberapa strategi untuk melakukan hal tersebut. 
Untung saja Bocchi menemukan file .csv yang berisi ranking universitas dunia untuk melakukan penataan strategi  : 

1A. Bocchi ingin masuk ke universitas yang bagus di Jepang. Oleh karena itu, 
Bocchi perlu melakukan survey terlebih dahulu. Tampilkan 5 Universitas dengan ranking tertinggi di Jepang.

1.B Karena Bocchi kurang percaya diri dengan kemampuannya, coba cari Faculty Student Score(fsr score) 
yang paling rendah diantara 5 Universitas di Jepang. 

1.C Karena Bocchi takut salah membaca ketika memasukkan nama universitas, 
cari 10 Universitas di Jepang dengan Employment Outcome Rank(ger rank) paling tinggi.

1.D Bocchi ngefans berat dengan universitas paling keren di dunia. Bantu bocchi mencari 
universitas tersebut dengan kata kunci keren.
```

## jawaban 1
``` bash
printf "1A.\n" 
sort -t ',' -k 1n '2023 QS World University Rankings.csv' | awk -F ',' '/JP/{print$2}' | head -5
```
**penjelasan:**

- ```Sort``` akan mengurutkan isi file '2023 QS World University Rankings.csv'
  berdasarkan kolom pertama  secara numerik ```-k 1n```
  dan dengan menggunakan tanda koma ```','``` sebagai pemisah antar kolom ```-t```.
- ```awk``` untuk menampilkan isi kolom kedua (dalam hal ini kolom dengan nomor 2)
  dari baris-baris yang mengandung huruf ```"JP"``` pada file input.
  Tanda koma ```','``` digunakan sebagai pemisah antar kolom ```-F``` dan
  ```print $2``` akan menampilkan isi kolom kedua. Hasil dari perintah ini akan menjadi output.
- ```head -5``` untuk mengoutput 5 baris

``` bash
printf "1B.\n" 
sort -t ',' -k 9n '2023 QS World University Rankings.csv' | awk -F ',' '/JP/{print$2}' | head -5
```
**penjelasan:**

- ```Sort``` akan mengurutkan isi file '2023 QS World University Rankings.csv'
  berdasarkan kolom ke sembilan  secara numerik ```-k 9n```
  dan dengan menggunakan tanda koma ```','``` sebagai pemisah antar kolom ```-t```.
- ```awk``` untuk menampilkan isi kolom kedua 
  dari baris-baris yang mengandung huruf ```"JP"``` pada file input.
  Tanda koma ```','``` digunakan sebagai pemisah antar kolom ```-F``` dan
  ```print $2``` akan menampilkan isi kolom kedua. Hasil dari perintah ini akan menjadi output.
- ```head -5``` untuk mengoutput 5 baris

``` bash
printf "1C.\n" 
sort -t ',' -k 20n '2023 QS World University Rankings.csv' | awk -F ',' '/JP/{print$2}' | head -10
```
**penjelasan :**

- ```Sort``` akan mengurutkan isi file '2023 QS World University Rankings.csv'
  berdasarkan kolom ke dua puluh  secara numerik ```-k 20n```
  dan dengan menggunakan tanda koma ```','``` sebagai pemisah antar kolom ```-t```.
- ```awk``` untuk menampilkan isi kolom kedua (dalam hal ini kolom dengan nomor 2)
  dari baris-baris yang mengandung huruf ```"JP"``` pada file input.
  Tanda koma ```','``` digunakan sebagai pemisah antar kolom ```-F``` dan
  ```print $2``` akan menampilkan isi kolom kedua. Hasil dari perintah ini akan menjadi output.
- ```head -10``` untuk mengoutput 10 baris

``` bash
printf "1D.\n" 
awk -F ',' '/Keren/{print$2}' '2023 QS World University Rankings.csv'
```
**penjelasan :**

- ```awk``` untuk menampilkan isi kolom kedua (dalam hal ini kolom dengan nomor 2)
  dari baris-baris yang mengandung huruf ```""``` pada file input.
  Tanda koma ```','``` digunakan sebagai pemisah antar kolom ```-F``` dan
  ```print $2``` akan menampilkan isi kolom kedua. Hasil dari perintah ini akan menjadi output.

**Screenshot**
![Screenshot_2023-03-10_112923](/uploads/113c9e51e1814dbceb88b807a5a6fab1/Screenshot_2023-03-10_112923.png)


#### Kendala Soal 1
Karena codenya terlalu general dan idenya cuma sort awk/grep maka tim kami harus mengikuti sidang untuk membuktikan tidak bersalah. Dan endingnya memang tidak terbukti bersalah

## Soal 2

Kobeni ingin pergi ke negara terbaik di dunia bernama Indonesia. Akan tetapi karena uang Kobeni habis untuk beli headphone ATH-R70x, Kobeni tidak dapat melakukan hal tersebut.
- Untuk melakukan coping, Kobeni mencoba menghibur diri sendiri dengan mendownload gambar tentang Indonesia. Coba buat script untuk mendownload gambar sebanyak X kali dengan X sebagai jam sekarang (ex: pukul 16:09 maka X nya adalah 16 dst. Apabila pukul 00:00 cukup download 1 gambar saja). Gambarnya didownload setiap 10 jam sekali mulai dari saat script dijalankan. Adapun ketentuan file dan folder yang akan dibuat adalah sebagai berikut:
  - File yang didownload memilki format nama “perjalanan_NOMOR.FILE” Untuk NOMOR.FILE, adalah urutan file yang download (perjalanan_1, perjalanan_2, dst)
  - File batch yang didownload akan dimasukkan ke dalam folder dengan format nama “kumpulan_NOMOR.FOLDER” dengan NOMOR.FOLDER adalah urutan folder saat dibuat (kumpulan_1, kumpulan_2, dst) 
- Karena Kobeni uangnya habis untuk reparasi mobil, ia harus berhemat tempat penyimpanan di komputernya. Kobeni harus melakukan zip setiap 1 hari dengan nama zip “devil_NOMOR ZIP” dengan NOMOR.ZIP adalah urutan folder saat dibuat (devil_1, devil_2, dst). Yang di ZIP hanyalah folder kumpulan dari soal di atas.

## Jawaban Soal 2

### kobeni_liburan.sh

Dalam script bash ```kobeni_liburan.sh```, terdapat 2 fungsi yang memiliki kegunaan yang berbeda. 2 fung tersebut adalah ```download()``` dan ```zipping()``` 

#### download()
```bash
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
```
Dalam fungsi tersebut, ada beberapa tahap:
- ```hour=$(date +%H)``` menyimpan value berupa jam yang menjadi parameter utama dalam aturan mendownload gambar
- ```num_images=$((hour == 0 ? 1 : hour))``` menentukan jumlah gambar yang akan didownload. Jika jam menunjukkan jam 00.00, maka gambar akan didownload sekali. Selain itu, gambar akan didownload sesuai dengan jumlah jam sekarang.
- Buat file ```numFolder.log``` untuk menyimpan current_file yang telah dibuat atau bisa disebut sebagai ```counter```. Hal ini dilakukan agar setelah di zip, folder yang dibuat akan melanjutkan penomoran yang sudah dilakukan.
- Buat folder ```kumpulan_*``` dengan * sesuai dengan penomoran
- Download gambar menggunakan ```wget --no-check-certificate``` dengan format nama ```perjalanan_*``` dengan * sesuai dengan penomoran dalam looping
- Setelah proses download selesai, value yang disimpan dalam counter ```numFolders``` akan bertambah

#### zipping()
```bash
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
```
Dalam fungsi ```zipping()``` terdapat beberapa tahap:
- Membuat folder ```devil_1.zip``` sebagai value awal dalam penomoran folder ```devil_*.zip```
- Jika sudah terdapat folder zip, maka sistem akan menghitung banyak folder zip dengan nama ```devil_*``` dengan * adalah bilangan asli 
- Zip folder dengan penamaan ```kumpulan_*``` ke dalam folder zip ```devil_*``` menggunakan syntax ```zip -m <nama file zip> <nama folder yang akan di zip>``` jika ingin membuat folder zip tanpa menghapus folder yang sudah di zip. Jika ingin menghapus folder yang sudah di zip, bisa menggunakan syntax ```zip -rm <nama file zip> <nama folder yang akan di zip>```

#### Crontab Soal 2
Terdapat 2 command crontab yang digunakan dalam ```Soal 2```, hal ini dikarenakan terdapat 2 fungsi yang akan dijalankan di waktu yang berbeda dalam script bash.

1. Command Crontab yang digunakan dalam fungsi ```download()```untuk mendownload gambar secara otomatis setiap 10 jam.
```bash 
0 */10 * * * bash /home/ikikk/Documents/sisop-praktikum-modul-1-2023-hs-e01/soal2/kobeni_liburan.sh download
```

2. Command Crontab yang digunakan dalam fungsi ```zipping()``` untuk melakukan zip folder ```kumpulan_*```yang telah dibuat setiap sekali sehari. Disini, kami tidak menggunakan 00.00 sebagai waktu untuk melakukan zip, karena jam tersebut masih rancu apakah sudah hari esok atau masih hari ini. Maka dari itu, kami menggunakan 00.01 dalam command crontab kami.
```bash
0 0 * * * bash /home/ikikk/Documents/sisop-praktikum-modul-1-2023-hs-e01/soal2/kobeni_liburan.sh zipping
```

**Screenshot***
<br>
Mendowload gambar sebanyak 11 kali, sesuai dengan jam sekarang yaitu jam 11.23
![WhatsApp_Image_2023-03-10_at_11.23.46](/uploads/d0f9d33b7326d18e87a02e11f45d6a83/WhatsApp_Image_2023-03-10_at_11.23.46.jpeg)

NumFolder.log menyimpan value 3 karena sudah terdapat 3 folder dengan nama kumpulan_*
![WhatsApp_Image_2023-03-10_at_11.25.31](/uploads/74bade69e2f61defcfa4b05eff2d9b50/WhatsApp_Image_2023-03-10_at_11.25.31.jpeg)

Melakukan zip folder kumpulan_* dan menghapus folder yang berhasil di zip
![WhatsApp_Image_2023-03-10_at_11.26.49](/uploads/4b519df5f3017989f03f77333ebc7790/WhatsApp_Image_2023-03-10_at_11.26.49.jpeg)

Saat mendownload gambar lagi, penamaan folder akan dilanjutkan sesuai dengan value yang disimpan dalan numFolder.log
![WhatsApp_Image_2023-03-10_at_11.28.56](/uploads/f201d3f74d7f36e999a037a4b430deba/WhatsApp_Image_2023-03-10_at_11.28.56.jpeg)

#### Kendala Soal 2
kendala pertama adalah kesulitan dalam mendowload gambar. Setelah saya dicari lebih lanjut mengenai ```wget```, ternyata ditambahkan syntax ```--no-check-certificate```.<br>
Kesulitan kedua adalah kesulitan dalam melalukan zip folder. Setelah ditelusuri dan mencoba mengubah-ubah variabel, ternyata fungsi dengan nama ```zip()``` dapat menyebabkan konflik karena terdapat command dengan nama serupa yaitu ```zip```. Setelah saya ubah nama fungsinya menjadi ```zipping()```, fungsi tersebut berhasil dijalankan dengan baik.

## Soal 3

Peter Griffin hendak membuat suatu sistem register pada script louis.sh dari setiap user yang berhasil didaftarkan di dalam file ```/users/users.txt```. Peter Griffin juga membuat sistem login yang dibuat di script ```retep.sh```

## Jawaban 3

#### Louis.sh

Script bash ```Louis.sh``` digunakan untuk register user. Data username dan password setiap user akan disimpan ke dalam file ```/users/users.txt```. Di awal program terdapat welcome message dan kemudian diminta untuk menginput username dan password seperti pada script di bawah. 

``` bash
echo Hello Buddy!! 
echo Welcome To The Register
echo "-------------"
echo Input Username :
read username
echo Input Password :
read -s password
```

kemudian dicek apakah file ```users.txt``` dan ```log.txt``` sudah ada, jika belum ada maka akan dibuat file Bernama ```users.txt``` dan ```log.txt```

``` bash
if [ ! -f "users.txt" ]; then
touch users.txt
fi
if [ ! -f "log.txt" ]; then
touch log.txt
fi
```
**penjelasan :**

-	```! -f “namafile”``` digunakan untuk memeriksa apakah file dengan nama ```users.txt``` dan ```log.txt``` ada di direktori kerja saat ini atau tidak. 
-	```touch “namafile”``` digunakan untuk membuat file baru.

Kemudian username yang sudah diinput akan dicek terlebih dahulu, yaitu dicek apakah username sudah pernah melakukan register sebelumnya dan terdapat pada file ```user.txt```

``` bash
if grep -q "$username" users.txt; then
        echo -e "\n-------------"
        echo "ERROR User already exist"
        echo "$(date '+%Y/%m/%d %H:%M:%S') REGISTER: ERROR User already exists" >> log.txt
        echo -e "\n-------------"
``` 
**penjelasan :**

-	Script ```grep -q "$username" users.txt``` digunakan untuk mendapatkan data username dari file ```users.txt.```
-	Apabila username ditemukan, maka akan ditampilkan pesan error. ERROR MESSAGE juga akan dimasukkan ke dalam file ```log.txt```.
-	File ```log.txt``` berisi  data setiap percobaan register dengan format : ```$(date '+%Y/%m/%d %H:%M:%S')``` + ERROR MESSAGE

Jika username tidak terdapat didalam ```users.txt``` maka akan dilakukan pengecekan pada kondisi selanjutnya. Pada kondisi kedua digunakan untuk cek apakah username mengandung spasi.

```bash
elif echo "$username" | grep -q  " "; then
        echo -e "\n-------------"
        echo "ERROR Username can not contains space"
        echo "$(date '+%Y/%m/%d %H:%M:%S') REGISTER: ERROR Username can not contains space" >> log.txt
        echo -e "\n-------------"
```
**penjelasan :**

-	Script ```echo "$username" | grep -q  " "``` digunakan untuk mendapatkan data username apakah mengandung spasi.
-	Apabila username mengandung spasi, maka akan ditampilkan ERROR MESSAGE. DATE + ERROR MESSAGE akan dimasukkan ke dalam file ```log.txt```.

Jika username tidak mengandung spasi maka akan dilakukan pengecekan kondisi selanjutnya. Kondisi ini menunjukan bahwa username yang dimasukkan belum terdapat di ```users.txt``` dan tidak mengandung spasi sehingga bisa dilanjutkan untuk pengecekan password.

```bash
else
        if [[ ( "^$password$" =~ "^$username$") ]]; then
        echo -e "\n-------------"
        echo "ERROR Password cannot like username, please try again with another username" 
        echo "$(date '+%Y/%m/%d %H:%M:%S') REGISTER: ERROR User $username password cannot like username" >> log.txt
        echo -e "\n------------"
```
**penjelasan :**

-	```( "^$password$" =~ "^$username$")``` digunakan untuk melakukan pengecekan apakah password sama dengan username
-	Apabila password sama dengan username, maka akan ditampilkan ERROR MESSAGE. DATE + ERROR MESSAGE akan dimasukkan ke dalam file ```log.txt```.
```teks
Kemudian apabila kondisi tidak terpenuhi maka akan dilanjutkan ke kondisi selanjutnya yaitu dilakukan pengecekan apakah password sudah sesuai dengan ketentuan.
```

```bash 
        else
                if [[ "$password" =~ [0-9A-Za-z]{8,} ]]; then
                        if [[ ! "$password" =~ [0-9] || ! "$password" =~ [A-Z] || ! "$password" =~ [a-z] ]]; then
                                echo -e "\n-------------"
                                echo "Password not valid!"
                                echo "ERROR: Password must contain at least one numeric, uppercase and lowercase letter"
                                echo "$(date '+%Y/%m/%d %H:%M:%S') REGISTER: ERROR User $username password must contain at least one numeric, uppercase and lowercase letter" >> log.txt
                                echo -e "\n-------------"
                        elif echo "$password" | grep -q -i -E "chicken|ernie"; then
                                echo -e "\n-------------"
                                echo "Password not valid!"
                                echo "ERROR: Password must not contain "ernie or chicken"!"
                                echo "$(date '+%Y/%m/%d %H:%M:%S') REGISTER: ERROR User $username password must not contain "ernie or chicken"!" >> log.txt
                                echo -e "\n-------------"
                        else
                                echo -e "\n-------------"
                                echo "Registered successfully"
                                echo "$username""-""$password" >> users.txt
                                echo "$(date '+%Y/%m/%d %H:%M:%S') REGISTER: INFO User $username registered successfully" >> log.txt
                                echo -e "\n-------------"
                        fi
                    #if password not valid
                else
                        echo -e "\n-------------"
                        echo "Password not valid!"
                        echo "ERROR: Password must contain at least 8 characters!"
                        echo "$(date '+%Y/%m/%d %H:%M:%S') REGISTER: INFO User $username password must contain at least 8 characters!" >> log.txt
                        echo -e "\n-------------"
                fi
```
**penjelasan :**

-	Pada script ```"$password" =~ [0-9A-Za-z]{8,}```, [0-9A-Za-z] adalah karakter kelas yang memuat digit 0-9 dan karakter huruf a-z (baik huruf besar maupun kecil), sedangkan {8,} menunjukkan bahwa string tersebut harus memiliki setidaknya 8 karakter atau lebih.
-	Script ```[[ ! "$password" =~ [0-9] || ! "$password" =~ [A-Z] || ! "$password" =~ [a-z] ]]``` digunakan untuk pengecekan kondisi bahwa password tidak menggandung angka dan atau huruf Kapital dan atau huruf kecil.
-	Script ```echo "$password" | grep -q -i -E "chicken|ernie"``` digunakan untuk pengecekan kondisi bahwa password mengandung kata chicken dan ernie
Jika password tidak memenuhi kondisi diatas artinya password valid dan berhasil melakukan registrasi, Selanjutnya username dan password yang berhasil registrasi akan dimasukkan kedalam file ```users.txt``` dengan format ```“$username""-""$password"```. Setiap percobaan registrasi akan disimpan pada file ```log.txt``` dengan format DATE + MESSSAGE

#### Retep.sh

Script bash ```Retep.sh``` digunakan untuk login user. Diawal program terdapat welcome message dan kemudian diminta untuk menginput username dan password seperti pada script di bawah.

```bash
echo Login Page
echo "-------------"

echo Input Username :
read username

echo Input Password :
read -s password
```

Selanjutnya dilakukan pengecekan dengan cara membuat variable baru ```data_input```.

```bash
data_input="$username-$password"
        if ! grep -q "$username" users.txt; then
                echo -e "\n-------------"
                echo "ERROR User not exist"
                echo "$(date '+%Y/%m/%d %H:%M:%S') LOGIN: ERROR User $username  not exist" >> log.txt
                echo -e "\n-------------"
        elif grep -q "^$data_input$" users.txt; then
                echo -e "\n------------"
                echo "Logged in" 
                echo "$(date '+%Y/%m/%d %H:%M:%S') LOGIN: INFO User $username logged in" >> log.txt
                echo -e "\n------------"
        else
                echo -e "\n------------"
                echo "ERROR Failed login" 
                echo "$(date '+%Y/%m/%d %H:%M:%S') LOGIN: ERROR Failed login attempt on user $username" >> log.txt
                echo -e "\n------------"
        fi
```

-	```! grep -q "$username" users.txt``` digunakan untuk mengecek apakah username sudah terdaftar di ```users.txt```, jika sudah terdaftar maka akan keluar ERROR MESSAGE.
-	```grep -q "^$data_input$" users.txt``` digunakan untuk mengecek apakah username dan password yang dimasukkan sudah benar. Jika sudah benar maka berhasil log in. Setiap percobaan login akan disimpan dengan format date ```'+%Y/%m/%d %H:%M:%S```

Berikut merupakan hasil dari percobaan register dan login yang telah kami lakukan. Karena file terdapat dalam root maka dalam menjalankan script bash perlu menngunakan script ```sudo```. 
- ```sudo ./louis.sh``` dan ```sudo ./retep.sh``` untuk menjalankan script bash
- ```cat``` digunakan untuk menampilkan isi dari sebuah file

Isi dari file ```users.txt```
```bash
rulelulu-Rulelulu1
rizkyalifiyah-Riszkyalifiyah7
rossyhaqqy-Rossyhaqqy2
angelaoryza-angelaOryza7
```
Isi dari file ```log.txt```
```bash
2023/03/03 04:22:14 REGISTER: INFO User rulelulu registered successfully
2023/03/03 04:22:33 REGISTER: ERROR User already exists
2023/03/03 04:22:48 REGISTER: ERROR Username can not contains space
2023/03/03 04:23:40 REGISTER: ERROR User rizkyalifiyah password cannot like username
2023/03/03 04:24:03 REGISTER: ERROR User rizkyalifiyah password must contain at least one numeric, uppercase and lowercase letter
2023/03/03 04:24:29 REGISTER: ERROR User rossyhaqqy password must not contain ernie or chicken!
2023/03/03 04:25:33 REGISTER: INFO User rizkyalifiyah registered successfully
2023/03/03 04:25:47 REGISTER: INFO User rossyhaqqy registered successfully
2023/03/03 04:26:15 LOGIN: ERROR User ahmad  not exist
2023/03/03 04:26:28 LOGIN: ERROR Failed login attempt on user Rulelulu
2023/03/03 04:27:00 LOGIN: INFO User rizkyalifiyah logged in
```

#### Kendala Soal 3
Mempelajari command-command sederhana dan fungsinya yang ternyata memiliki pengaruh besar seperti ``` -i, -s, -e ```

## Soal 4
```text
Johan Liebert adalah orang yang sangat kalkulatif. Oleh karena itu ia mengharuskan dirinya untuk mencatat
log system komputernya. File syslog tersebut harus memiliki ketentuan : 
- Backup file log system dengan format jam:menit tanggal:bulan:tahun (dalam format .txt).
Isi file harus dienkripsi dengan string manipulation yang disesuaikan dengan jam dilakukannya 
backup seperti berikut:
- Menggunakan sistem cipher dengan contoh seperti berikut. 
Huruf b adalah alfabet kedua, sedangkan saat ini waktu menunjukkan pukul 12, 
sehingga huruf b diganti dengan huruf alfabet yang memiliki urutan ke 12+2 = 14
Hasilnya huruf b menjadi huruf n karena huruf n adalah huruf ke empat belas, dan seterusnya.
Setelah huruf z akan kembali ke huruf a
- Buat juga script untuk dekripsinya.
- Backup file syslog setiap 2 jam untuk dikumpulkan
```
## jawaban 4

### log_encrypt.sh
``` bash
#!/bin/bash

#0 */2 * * * /bin/bash /home/rosy/sisop_modul1/soal4/log_encrypt.sh
waktu=$(date "+%H:%M %d:%m:%Y")
jam=$(date "+%H")
nama_file="$waktu.txt"
asli="abcdefghijklmnopqrstuvwxyz"
asli2="ABCDEFGHIJKLMNOPQRSTUVWXYZ"

enkrip=
for ((i=0; i<${#asli} ; i++))
do
	char=${asli:$i:1}
	asci=$(printf '%d' "'$char")
	a=97
	enkrip_asci=$((10#$asci + 10#$jam - 10#$a ))
	b=26
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
        enkrip_asci2=$((10#$asci2 + 10#$jam - 10#$a2 ))
        b2=26
        enkrip_asci2=$((10#$enkrip_asci2 % 10#$b2))
        enkrip_asci2=$((10#$enkrip_asci2 + 10#$a2 ))
        enkrip22=$(printf \\$(printf '%03o' "$enkrip_asci2"))
        enkrip2+=$enkrip22
done
cat /var/log/syslog  | tr "${asli}" "${enkrip}" | tr "${asli2}" "${enkrip2}" > "/home/rosy/sisop_modul1/soal4/${nama_file}"

```
**penjelasan:**

- ambil waktu dengan ```date "+%H:%M %d:%m:%Y"``` dan simpan dalam variabel ```waktu```
- ambil jam dengan ```date "+%H"``` dan simpan dalam variabel ```jam```
- buat nama_file dengan format ```$waktu.txt``` dan simpan dalam variabel ```nama_file```
- buat variabel ```asli``` yang berisi ```abcdefghijklmnopqrstuvwxyz```
- buat variabel ```asli2``` yang berisi ```ABCDEFGHIJKLMNOPQRSTUVWXYZ```
- buat variabel ```enkrip``` yang berisi string kosong
- buat variabel ```enkrip2``` yang berisi string kosong
- lakukan perulangan sebanyak panjang variabel ```asli``` yaitu 26 kali
- ambil karakter ke-i dari variabel ```asli``` dan simpan dalam variabel ```char```
- ubah karakter ke-i dari variabel ```asli``` menjadi ascii dan simpan dalam variabel ```asci```
- buat variabel ```a``` yang berisi 97 (ASSCII a)
- buat variabel ```b``` yang berisi 26
- ubah ascii menjadi enkripsi dengan rumus ```enkrip_asci = (asci + jam - a) % b + a```
  dan simpan dalam variabel ```enkrip_asci``` ingat untuk mengubah ke basis 10 terlebih dahulu dengan ```10#$variabel```
- ubah ascii menjadi karakter dengan rumus ```enkrip2 = \$(printf '%03o' "$enkrip_asci")```
  dan simpan dalam variabel ```enkrip2```
- tambahkan ```enkrip2``` ke variabel ```enkrip```
- lakukan hal yang sama seperti looping pertama dengan perbedaan nilai ```a2``` adalah 65(ASSCII A) 
- enkripsi isi file syslog dengan ```cat /var/log/syslog  | tr "${asli}" "${enkrip}" | tr "${asli2}" "${enkrip2}" > "/home/rosy/sisop_modul1/soal4/${nama_file}"```
- ```tr``` disini berperan untuk mengganti karakter yang ada di file syslog dengan karakter yang sudah dienkripsi
- simpan hasil enkripsi ke dalam file dengan nama ```$nama_file```
- jalankan script dengan ```0 */2 * * * /bin/bash /home/rosy/sisop_modul1/soal4/log_encrypt.sh```
  yang berarti script akan dijalankan setiap 2 jam sekali di menit ke-0
### log_decrypt.sh
``` bash
#!/bin/bash
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

```
**penjelasan:**

- ambil jam dari inputan dengan ```x=$(echo $input |cut -c 1)``` dan ```y=$(echo $input |cut -c 2)``` untuk mengambil karakter pertama dan kedua
- ubah jam menjadi basis 10 dengan ```10#$variabel``` dan menjadikan integer dengan ```$((10#$variabel))```
- buat variabel ```asli``` yang berisi ```abcdefghijklmnopqrstuvwxyz```
- buat variabel ```asli2``` yang berisi ```ABCDEFGHIJKLMNOPQRSTUVWXYZ```
- buat variabel ```enkrip``` yang berisi string kosong
- buat variabel ```enkrip2``` yang berisi string kosong
- lakukan perulangan sebanyak panjang variabel ```asli``` yaitu 26 kali
- ambil karakter ke-i dari variabel ```asli``` dan simpan dalam variabel ```char```
- ubah karakter ke-i dari variabel ```asli``` menjadi ascii dan simpan dalam variabel ```asci```
- buat variabel ```a``` yang berisi 97 (ASSCII a)
- buat variabel ```b``` yang berisi 26
- ubah ascii menjadi enkripsi dengan rumus ```enkrip_asci = (asci - jam - a) % b + a```
  dan simpan dalam variabel ```enkrip_asci``` ingat untuk mengubah ke basis 10 terlebih dahulu dengan ```10#$variabel```
- ubah ascii menjadi karakter dengan rumus ```enkrip2 = \$(printf '%03o' "$enkrip_asci")```
  dan simpan dalam variabel ```enkrip2```
- tambahkan ```enkrip2``` ke variabel ```enkrip```
- lakukan hal yang sama seperti looping pertama dengan perbedaan nilai ```a2``` adalah 65(ASSCII A)
- dekripsi isi file syslog dengan ```cat "$input" | tr "${asli}" "${enkrip}" | tr "${asli2}" "${enkrip2}" > "/home/rosy/sisop_modul1/soal4/asli.txt"```
- ```tr``` disini berperan untuk mengganti karakter yang ada di file syslog dengan karakter yang sudah didekripsi
- simpan hasil dekripsi ke dalam file dengan nama ```asli.txt```

**Screenshot**
![Screenshot_2023-03-10_114343](/uploads/0d3d38c8120d29318e3795888987644b/Screenshot_2023-03-10_114343.png)
![Screenshot_2023-03-10_114413](/uploads/16102e45a31af5d16d0e62c70e1b3623/Screenshot_2023-03-10_114413.png)
![Screenshot_2023-03-10_114513](/uploads/3c1e63040eedb00bb30f791c039ed030/Screenshot_2023-03-10_114513.png)
![Screenshot_2023-03-10_114544](/uploads/320beda98b358a26b04f669088ffeced/Screenshot_2023-03-10_114544.png)

#### Kendala Soal 4
Overflow karena awalnya tidak mengunakan basis sepuluh
