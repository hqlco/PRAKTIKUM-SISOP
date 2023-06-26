# sisop-praktikum-modul-2-2023-HS-E01

| Nama                      | NRP           |Username           |
|---------------------------|---------------|--------------|
|Moh Rosy Haqqy Aminy       |5025211012     |rosyhaqqy          |
|Rule Lulu Damara           |5025211050     |rulelulu           |
|Rizky Alifiyah Rahma       |5025211209     |ikikk              |

## Soal1
```text
    Grape-kun adalah seorang penjaga hewan di kebun binatang, dia mendapatkan tugas dari atasannya untuk melakukan penjagaan pada 
    beberapa hewan-hewan yang ada di kebun binatang sebelum melakukan penjagaan Grape-kun harus mengetahui terlebih dahulu hewan 
    apa aja yang harus dijaga dalam drive kebun binatang tersebut terdapat folder gambar dari hewan apa saja yang harus dijaga oleh 
    Grape-kun. Berikut merupakan link download dari drive kebun binatang tersebut : 
    https://drive.google.com/uc?export=download&id=1oDgj5kSiDO0tlyS7-20uz7t20X3atwrq 

    a. Grape-kun harus mendownload file tersebut untuk disimpan pada penyimpanan local komputernya. Dan untuk melakukan melihat file 
    gambar pada folder yang telah didownload Grape-kun harus melakukan unzip pada folder tersebut.
```
## Jawaban a
### Step 1 : Download file dari drive
``` bash
void DownloadFile(const char *url, const char *zipPath){
    pid_t pidDownload = fork();
    if (pidDownload < 0) 
    {
        perror("Failed to fork\n"); //print error message
        exit(EXIT_FAILURE); //terminate the program when an error occurs.
    }
    else if (pidDownload == 0) 
    {
        char *url_new = malloc(128 * sizeof(char));
        if (!url_new) 
        {
            exit(EXIT_FAILURE);
        }
        snprintf(url_new, 128, "https://drive.google.com/uc?export=download&id=%s", url);
        printf("Downloading file : %s\n", url);
        execl("/bin/wget", "wget", "--no-check-certificate", url_new, "-O", zipPath, NULL);
        free(url_new); 
    } 
    else 
    {
        int status;
        if (waitpid(pidDownload, &status, 0) < 0) 
        {
            perror("waitpid() failed");
            exit(EXIT_FAILURE);
        }else {
            while ((wait(&status)) > 0);
        }
    }
}
```

**Penjelasan:**

- Step pertama yang dilakukan adalah membuat sebuah fungsi bernama ```DownloadFile()``` yang digunakan untuk mengunduh file dari internet menggunakan perintah wget
- Jika nilai pidDownload sama dengan 0, maka ini adalah proses anak yang baru saja dibuat. Selanjutnya adalah mendeklarasikan variabel pointer dengan alokasi memori seukuran ```128*sizeof(char) url_new``` sebagai gabungan url drive google dengan link filenya ```char *url_new = malloc(128 * sizeof(char))```
- Setelah itu digabungkan string antara url drive google dengan link file menggunakan ``snprintf``
  ```snprintf(url_new, 128, "https://drive.google.com/uc?export=download&id=%s", url)```
- Kemudian dialihkan kode eksekusinya ke perintah ```execl()```. Fungsi ```execl()``` akan mengganti gambar program anak yang sedang berjalan dengan gambar program wget dan melewatkan argumen-argumen yang diperlukan untuk menjalankan wget. Argumen-argumen ini termasuk URL unduhan, opsi untuk menonaktifkan pemeriksaan sertifikat, dan path untuk menyimpan file yang diunduh menggunakan ```execl("/bin/wget", "wget", "--no-check-certificate", url_new, "-O", zipPath, NULL)```
- Setelah itu dilakukan pengosongan memori agar bisa digunakan lagi dengan free ```free(url_new)```
- Jika nilai pidDownload lebih besar dari 0, maka ini adalah proses induk. Proses induk ini akan menunggu proses anaknya selesai/ Setelah proses anak selesai, proses induk akan keluar dari loop while dan menyelesaikan eksekusi program.

### Step 2 : Unzip pada file

``` bash
void unzipping(const char *zipPath)
{
    const char* folderName = "binatang";

    pid_t pidUnzip = fork();
    if(pidUnzip == 0){
        printf("Unzipping file : %s\n", zipPath);
        execl("/bin/unzip", "unzip", "-d", "binatang", zipPath, NULL);
        exit(0);
    }
    else if (pidUnzip < 0)
    {
        perror("Failed to fork");
        exit(EXIT_FAILURE);
    } 
    else 
    {
        int status = 0;
        if (waitpid(pidUnzip, &status, 0) == -1) {
            perror("waitpid() failed");
            exit(EXIT_FAILURE);
        }
        else {
            while ((wait(&status)) > 0);
        }
    }
} 
```
**Penjelasan:**

Dalam fungsi diatas ada beberapa tahap :
-	```const char*``` yaitu mendefinisikan sebuah string konstanta Bernama foldername yang digunakan untuk menyimpan nama folder yang akan digunakan
-	```pid_t pidUnzip = fork()```, di Membuat sebuah proses baru menggunakan fungsi ```fork()``` dan menyimpan nilai pengembalian (PID) ke dalam variabel ```pidUnzip```.
-	Jika nilai ```pidUnzip``` sama dengan 0, maka ini adalah proses anak yang baru saja dibuat. Proses anak ini akan mengalihkan kode eksekusinya ke perintah ```execl()```. Fungsi ```execl("/bin/unzip", "unzip", "-d", "binatang", zipPath, NULL)``` akan mengganti gambar program anak yang sedang berjalan dengan gambar program unzip dan melewatkan argumen-argumen yang diperlukan untuk menjalankan unzip

## Jawaban b
```text
    b. Setelah berhasil melakukan unzip Grape-kun melakukan pemilihan secara acak pada file gambar tersebut untuk melakukan shift 
    penjagaan pada hewan tersebut.
```
``` bash
void gachaImages(const char *folderPath){
    int gachaIndex;
    const char* gachaImageName;

    DIR* dir;
    dir = opendir(folderPath);
    cekDir(folderPath);
    
    srand(time(NULL));
    int imgCount = 0;
    char* filenames[256];
    struct dirent* imagesFile;
    while ((imagesFile = readdir(dir)) != NULL) {
        if (strstr(imagesFile->d_name, ".jpg") != NULL) {
            filenames[imgCount] = strdup(imagesFile->d_name);
            imgCount++;
        }
    }
    closedir(dir);
    if (imgCount == 0) {
        printf("No image files found in directory '%s'\n", folderPath);
    }

    gachaIndex = rand() % imgCount;
    gachaImageName = filenames[gachaIndex];
    printf("Gacha : %s\n", gachaImageName);

    for (int i = 0; i < imgCount; i++) {
        free(filenames[i]);
    }
}
```
**Penjelasan:**

Kode di atas merupakan sebuah fungsi yang digunakan untuk mengambil gambar secara acak dari suatu direktori.
-	Variabel dir dideklarasikan sebagai pointer ke ```struct DIR (DIR*)```, yang akan digunakan untuk membuka direktori yang telah diinputkan pada parameter input
-	Dilakukan assignment variable dir dengan hasil dari pemanggilan fungsi ```opendir()```
-	Memanggil fungsi ```cekDir()``` dengan argument untuk memastikan bahwa direktori tersebut valid dan dapat diakses.
-	Fungsi ```srand()``` dipanggil untuk mengeset seed dari fungsi``` rand()``` agar hasilnya dapat random setiap kali program dijalankan
-	Deklarasi ```imgCount``` untuk menghitung jumlah gambar
-	```struct dirent*``` dideklarasikan sebagai pointer ke imagesFile
-	Dilakukan while loop untuk membaca setiap file direktori, dalam looping ini dipastikan juga bahwa file yang sedang dibaca memiliki ekstensi .jpg menggunakan syntax berikut ```strstr(imagesFile->d_name, ".jpg") != NULL```
-	Selanjutnya nama file disimpan ke dalam array ```filenames``` yang sudah dideklarasikan sebelumnya pada index yang saat ini terdapat imgCount menggunakan syntax berikut ```filenames[imgCount] = strdup(imagesFile->d_name)```
-	```imgCount``` kemudian ditambahkan satu setiap looping
-	```closedir()``` dipanggil untuk menutup direktori yang telah dibuka
-	variabel gachaIndex diisi dengan hasil dari pemanggilan fungsi ```rand()``` dan dimodulo dengan imgCount, ```gachaIndex = rand() % imgCount```
-	Variabel ```gachaImageName``` diisi dengan nilai dari array filenames pada index yang sama dengan nilai gachaIndex, ```gachaImageName = filenames[gachaIndex]```
-	Hasil dari ```gachaImageName``` ditampilan pada output message
-	Selanjutnya dilakukan loop untuk memfree setiap elemen pada array filenames menggunakan fungsi ```free(filenames[i])```

### Penjelasan Fungsi cekDir()
``` bash
void cekDir(const char *folderPath){
    DIR* dir;
    dir = opendir(folderPath);
    if (dir == NULL) 
    {
        printf("No Directory Named '%s'\n", folderPath);
    }
}
```
### Penjelasan:

    Fungsi ```cekDir()``` yang bertujuan untuk memeriksa apakah sebuah direktori sudah ada atau belum.

## Jawaban c
```text
    c.	Karena Grape-kun adalah orang yang perfeksionis Grape-kun ingin membuat direktori untuk memilah file gambar tersebut.
     Direktori tersebut dengan nama HewanDarat, HewanAmphibi, dan HewanAir. Setelah membuat direktori tersebut Grape-kun harus 
     melakukan filter atau pemindahan file gambar hewan sesuai dengan tempat tinggal nya. 
    
    Kemudian dibuat fungsi untuk memindahkan file gambar ke folder sesuai dengan kategori hewan tempat tinggalnya. 
```
```bash
void moveFile(const char *folderPath){

    pid_t pidMvFile = fork();
    DIR* dir = opendir(folderPath);

    if(pidMvFile < 0){
        perror("Failed to fork");
        exit(EXIT_FAILURE);
    }
    if(pidMvFile == 0){
        char* hewanDarat = "./HewanDarat";
        char* hewanAmphibi = "./HewanAmphibi";
        char* hewanAir = "./HewanAir";

        struct dirent* imagesFile;
        while ((imagesFile = readdir(dir)) != NULL) {
            if (strstr(imagesFile->d_name, ".jpg") != NULL) {
                char* tempatTinggal = NULL;
                if (strstr(imagesFile->d_name, "darat") != NULL) {
                    tempatTinggal = hewanDarat;
                } else if (strstr(imagesFile->d_name, "amphibi") != NULL) {
                    tempatTinggal = hewanAmphibi;
                } else if (strstr(imagesFile->d_name, "air") != NULL) {
                    tempatTinggal = hewanAir;
                }

                char folderAsal[100];
                char folderTujuan[100];
                createFolder(tempatTinggal);
                sprintf(folderAsal, "%s/%s", folderPath, imagesFile->d_name);
                sprintf(folderTujuan, "%s/%s", tempatTinggal, imagesFile->d_name);

                if (rename(folderAsal, folderTujuan) == -1) {
                    printf("Error: Unable to move file '%s' to directory '%s'\n", imagesFile->d_name, tempatTinggal);
                }

            }
        }
        closedir(dir);
    }
    else{
        int status;
         while ((wait(&status)) > 0);
    }
}
```
**Penjelasan:**

Berikut merupakan step-step dalam fungsi ```moveFile()```
-	```pid_t pidMvFile = fork()``` Membuat proses baru menggunakan ```fork()``` dan menyimpan nilai PID-nya di variabel pidMvFile.
-	Jika ```pidMvFile == 0``` yaitu jika proses yang dijalankan ada child proses maka dilanjutkan esekusi kode di dalamnya
-	```char* var = “path”``` digunakan untuk mendeklarasikan variable string yang berisi path folder tempat tinggal hewan
-	Mendeklarasikan pointer imagesFile bertipe struct dirent yang akan digunakan untuk membaca setiap file dan direktori di dalam direktori dir dengan menggunakan fungsi ```readdir()```
-	```strstr(imagesFile->d_name, ".jpg") != NULL```, Memeriksa apakah nama file yang sedang dibaca memiliki ekstensi .jpg
-	Selanjutnya dengan menggunakan ```else if``` conditional file diperiksa apakah mengandung kata kunci darat, amphibi, air. Jika benar, maka variable tempattinggal akan diisi dengan path folder tujuan yang sesuai.
-	Mendeklarasikan folderAsal dan folderTujuan dengan ukuran 100 karakter
-	Selanjutnya memanggil fungsi ```createFolder()``` untuk membuat folder tujuan bila belum ada. Mengisi nilai ‘folderAsal’ dengan path lengkap file yang sedang dibaca di direktori folderPath menggunakan ```sprintf(folderAsal, "%s/%s", folderPath, imagesFile->d_name)```
-	Mengisi nilai folderTujuan dengan path lengkap file yang akan dipindahkan ke folder tujuan yang ditentukan menggunakan ```sprintf(folderTujuan, "%s/%s", tempatTinggal, imagesFile->d_name)```
-	Memanggil fungsi ```rename(folderAsal, folderTujuan) == -1 ``` dalam conditional if untuk memindahkan dari folder asal ke folder tujuan.
-	Selajutnya direktori ditutup
-	Menunggu child process selesai dengan menggunakan fungsi ```wait()```. Variabel status digunakan untuk menampung status exit dari child process. Loop akan terus berjalan selama masih ada child process yang belum selesai.

## Jawaban d
```text
    d.	Setelah mengetahui hewan apa saja yang harus dijaga Grape-kun melakukan zip kepada direktori yang dia buat sebelumnya 
    agar menghemat penyimpanan. 
```
```bash
void zipping(const char *dirName) 
{
    pid_t pidZipping = fork();
    if (pidZipping == 0) 
    {
        char zipFolder[100];
        sprintf(zipFolder, "%s.zip", dirName);
        execl("/bin/zip", "zip", "-r", zipFolder, dirName, NULL);
        exit(EXIT_FAILURE);
    } else {
        int status;
         while ((wait(&status)) > 0);
    }
}
```
**Penjelasan:**

Fungsi ```zipping()``` yang memiliki satu parameter yaitu ```dirName``` yang berisi nama direktori yang akan di-zip.
Fungsi ini akan membuat sebuah proses baru untuk melakukan zip pada direktori yang diberikan.

Berikut Step-Stepnya :
- ```pid_t pidZipping = fork()``` Fungsi fork() akan membuat sebuah proses baru yang merupakan salinan dari proses yang sedang berjalan.
- ```if (pidZipping == 0)``` Dilakukan pengecekan apakah nilai dari pidZipping adalah 0. Jika iya, maka proses yang saat ini berjalan adalah child process.
- ```char zipFolder[100]``` Variabel zipFolder dideklarasikan sebagai array karakter dengan panjang 100.
- ```sprintf(zipFolder, "%s.zip", dirName)``` Nilai dari variabel dirName digabungkan dengan string ".zip" menggunakan fungsi ```sprintf()```. Hasil penggabungan tersebut akan disimpan dalam variabel ```zipFolder```.
- ```execl("/bin/zip", "zip", "-r", zipFolder, dirName, NULL)``` Pada baris ini, fungsi ```execl()``` digunakan untuk mengeksekusi perintah zip pada direktori yang diberikan. Fungsi ```execl()``` akan mengganti image dari process yang saat ini berjalan dengan image dari perintah yang diberikan.

## Fungsi main
```bash
int main()
{
    //https://drive.google.com/uc?export=download&id=1oDgj5kSiDO0tlyS7-20uz7t20X3atwrq 
    // 1a
    const char *zipPath = "binatang.zip";
    DownloadFile("1oDgj5kSiDO0tlyS7-20uz7t20X3atwrq", "binatang.zip");
    unzipping(zipPath);

    // 1b.
    const char* folderPath = "./binatang";
    gachaImages(folderPath);

    // 1c.
    moveFile(folderPath);

    // 1d.
    zipping("HewanAir");
    zipping("HewanAmphibi");
    zipping("HewanDarat");
}
```
**Penjelasan:**

Dalam ```main()``` digunakan untuk memanggil fungsi-fungsi yang sudah dijelaskan diatas
-	Step pertama adalah dengan mendeklarasikan ```const char *zipPath = "binatang.zip";```
-	Selanjutnya memanggil fungsi ```downloadFile()``` dengan parameter id dari link drive dan nama file zip yang akan disimpan
-	Selanjutnnya mendeklarasikan folderPath dan kemudian memanggil fungsi ```gachaImages()``` dengan argument folderPath
-	Selanjutnya memanggil fungsi ```moveFile()``` dengan argument folderPath
-	Selanjutnya dilakukan ```zipping()``` dengan argument yang sudah ditentukan

## Dokumentasi

![Screenshot 1189](https://user-images.githubusercontent.com/105763198/230564253-0a8abcc6-1278-44fb-8977-009cc1eef170.png)

## Output

    Output yang dihasilkan adalah sebagai berikut :
    1. File dari drive sudah terdownload, ini dibuktikan pada output kalimat "Downloading file : 1oDgj5kSiDO0tlyS7-20uz7t20X3atwrq"
    2. File yang terdownload di unzip, ini dibuktikan pada output kalimat "Unzipping file : binatang.zip"
    3. Pemilihan secara acak pada file gambar untuk melakukan shift penjagaan pada hewan, ini dibuktikan dengan output kalimat 
    "Gacha : singa_darat.jpg"
![Screenshot 1215](https://user-images.githubusercontent.com/105763198/230563980-db937dcc-2e53-4f20-a837-5df44f8e9bf4.png)
    4. Melakukan filter dan pemindahan file gambar hewan sesuai dengan tempat tinggal dan dilakukan zip
![Screenshot 1111](https://user-images.githubusercontent.com/105763198/230563983-1acf0213-82ae-4af1-8d3a-d357c94843e6.png)
    5. Tidak melakukan zip dna unzip menggunakan system seperti yang sudah diperintahkan


## Kendala
```text
    Kendala yang kami hadapi, kami sering menjumpai segmentation error. hal ini ternyata baru kami sadari bahwa saat ingin 
    melakukan write ke sebuah file, pastikan permission nya sudah sesuai.
    Saya juga menghadapi kendala pada saat melakukan zip folder terkadang keluar error message yang tertera pada gambar di bawah ini
```
![Screenshot 1112](https://user-images.githubusercontent.com/105763198/230564185-7a67c322-20ce-423c-9b5b-814be77fb078.png)

## Soal 2
Sucipto adalah seorang seniman terkenal yang berasal dari Indonesia. Karya nya sudah terkenal di seluruh dunia, dan lukisannya sudah dipajang di berbagai museum mancanegara. Tetapi, akhir-akhir ini sucipto sedang terkendala mengenai ide lukisan ia selanjutnya. Sebagai teman yang jago sisop, bantu sucipto untuk melukis dengan mencarikannya gambar-gambar di internet sebagai referensi !

- Pertama-tama, buatlah sebuah folder khusus, yang dalamnya terdapat sebuah program C yang per 30 detik membuat sebuah folder dengan nama timestamp [YYYY-MM-dd_HH:mm:ss].
- Tiap-tiap folder lalu diisi dengan 15 gambar yang di download dari https://picsum.photos/ , dimana tiap gambar di download setiap 5 detik. Tiap gambar berbentuk persegi dengan ukuran (t%1000)+50 piksel dimana t adalah detik Epoch Unix. Gambar tersebut diberi nama dengan format timestamp [YYYY-mm-dd_HH:mm:ss].
- Agar rapi, setelah sebuah folder telah terisi oleh 15 gambar, folder akan di zip dan folder akan di delete(sehingga hanya menyisakan .zip).
- Karena takut program tersebut lepas kendali, Sucipto ingin program tersebut men-generate sebuah program "killer" yang siap di run(executable) untuk menterminasi semua operasi program tersebut. Setelah di run, program yang menterminasi ini lalu akan mendelete dirinya sendiri.
- Buatlah program utama bisa dirun dalam dua mode, yaitu MODE_A dan MODE_B. untuk mengaktifkan MODE_A, program harus dijalankan dengan argumen -a. Untuk MODE_B, program harus dijalankan dengan argumen -b. Ketika dijalankan dalam MODE_A, program utama akan langsung menghentikan semua operasinya ketika program killer dijalankan. Untuk MODE_B, ketika program killer dijalankan, program utama akan berhenti tapi membiarkan proses di setiap folder yang masih berjalan sampai selesai(semua folder terisi gambar, terzip lalu di delete).

##### Catatan :
- Tidak boleh menggunakan system()
- Proses berjalan secara daemon
- Proses download gambar pada beberapa folder dapat berjalan secara bersamaan (overlapping)

## Jawaban Soal 2
### Step 1: Membuat Folder (Soal 2a)
```text
Pertama-tama, buatlah sebuah folder khusus, yang dalamnya terdapat sebuah program C yang per 30 detik membuat sebuah folder dengan nama timestamp [YYYY-MM-dd_HH:mm:ss].
```
#### Source Code
```c
time_t now;
struct tm *timeinfo;
time(&now);
timeinfo = localtime(&now);
DIR *dir;
dir = opendir(FOLDER_PATH);
if ((dir) != NULL)
{
    strftime(folder_name, sizeof(folder_name), "%Y-%m-%d_%H:%M:%S", timeinfo);
    mkdir(folder_name, 0777);
}
wait(NULL);
sleep(30);
```
#### Penjelasan 2a
- Setelah membuat syntax untuk proses Daemon, buat syntax untuk overlapping memakai ```if (fork() == 0)``` di dalam ```while(1)```
- Di dalam ```if (fork() == 0)```, buat syntax untuk membuat foldernya
- Inisialisasi timestamp yang akan digunakan dalam format penamaan folder:
```c
time_t now;
struct tm *timeinfo;
time(&now);
timeinfo = localtime(&now);
```
- Buka ```Folder Path``` yang dikehendaki yang akan menampung folder-folder, zip-zip, dan program killer yang dihasilkan:
```c
DIR *dir;
dir = opendir(FOLDER_PATH);
```
- Buat folder dengan format penamaan menggunakan timestamp saat folder dibuat:
```c
if ((dir) != NULL)
{
    strftime(folder_name, sizeof(folder_name), "%Y-%m-%d_%H:%M:%S", timeinfo);
    mkdir(folder_name, 0777);
}
```
- Menunggu proses hingga selesai menggunakan ```wait(NULL);```
- Tunggu selama 30 deti untuk membuat folder selanjutnya menggunakan ```sleep(30)```

### Step 2: Membuat Fungsi Download Gambar (Soal 2b)
```text
Tiap-tiap folder lalu diisi dengan 15 gambar yang di download dari https://picsum.photos/ , dimana tiap gambar di download setiap 5 detik. Tiap gambar berbentuk persegi dengan ukuran (t%1000)+50 piksel dimana t adalah detik Epoch Unix. Gambar tersebut diberi nama dengan format timestamp [YYYY-mm-dd_HH:mm:ss].
```
#### Source Code
```c
for (int i = 0; i < 15; i++)
{
    if (fork() == 0)
    {
        time_t now1;
        struct tm *timeinfo1;
        time(&now1);
        timeinfo1 = localtime(&now1);
        strcpy(img_name, folder_name);
        strcat(img_name, "/");
        strftime(img, sizeof(img), "%Y-%m-%d_%H:%M:%S", timeinfo1);
        strcat(img_name, img);
        strcat(img_name, ".jpg");
        sprintf(url, PIC_URL, (((int)now1) % 1000) + 50);
        char *argv[] = {"wget", "-qO", img_name, url, NULL};
        execv("/bin/wget", argv);
    }
    sleep(5);
}
wait(NULL);
```
#### Penjelasan 2b
- Buat perulangan untuk mendownload gambar sebanyak 15 kali dengan ```for (int i = 0; i < 15; i++)```
- Inisialisasi timestamp yang akan digunakan dalam format penamaan gambar:
```c
time_t now1;
struct tm *timeinfo1;
time(&now1);
timeinfo1 = localtime(&now1);
```
- Ubah direktori untuk menyimpan gambar:
```c
strcpy(img_name, folder_name);
strcat(img_name, "/");
```
- Membuat nama file gambar menggunakan timestamp:
```c
strftime(img, sizeof(img), "%Y-%m-%d_%H:%M:%S", timeinfo1);
strcat(img_name, img);
strcat(img_name, ".jpg");
```
- Menyesuaikan ukuran gambar sesuai ketentuan:
```c
sprintf(url, PIC_URL, (((int)now1) % 1000) + 50);
```
- Mendownload gambar:
```c
char *argv[] = {"wget", "-qO", img_name, url, NULL};
execv("/bin/wget", argv);
```
- Menunggu 5 detik untuk melakukan proses download pada folder berikutnya menggunakan syntax ```sleep(5);```
- Tunggu hingga proses selesai menggunakan ```wait(NULL);```
### Step 3: Membuat Fungsi Zip (Soal 2c)
```text
Agar rapi, setelah sebuah folder telah terisi oleh 15 gambar, folder akan di zip dan folder akan di delete(sehingga hanya menyisakan .zip).
```
#### Source Code
```c
if (fork() == 0)
{
    char zip_name[200];
    strcpy(zip_name, folder_name);
    strcat(zip_name, ".zip");
    char *argv[] = {"zip", "-rmq", zip_name, folder_name, NULL};
    execv("/usr/bin/zip", argv);
}
```

#### Penjelasan 2c
- Buat nama folder zip menggunakan nama folder yang akan di zip:
```c
strcpy(zip_name, folder_name);
strcat(zip_name, ".zip");
```
- Buat folder zip menggunakan ```-rmq```. Argumen ```-rmq``` disini maksudnya adalah ```-r```untuk memperluas pengarsipan secara rekursif pada semua direktori dan subdirektori dalam direktori yang diberikan , ```-m``` untuk menghapus file yang telah diarsipkan setelah berhasil mengompresinya, dan ```-q``` untuk menjalankan zip dalam mode "senyap" (quiet mode) sehingga tidak ada output yang dihasilkan selain pesan kesalahan jika terjadi kesalahan.:
```c
char *argv[] = {"zip", "-rmq", zip_name, folder_name, NULL};
execv("/usr/bin/zip", argv);
```
### Step 4: Membuat Program Killer (Soal 2d & 2e)
```text
d. Karena takut program tersebut lepas kendali, Sucipto ingin program tersebut men-generate sebuah program "killer" yang siap di run(executable) untuk menterminasi semua operasi program tersebut. Setelah di run, program yang menterminasi ini lalu akan mendelete dirinya sendiri.
e. Buatlah program utama bisa dirun dalam dua mode, yaitu MODE_A dan MODE_B. untuk mengaktifkan MODE_A, program harus dijalankan dengan argumen -a. Untuk MODE_B, program harus dijalankan dengan argumen -b. Ketika dijalankan dalam MODE_A, program utama akan langsung menghentikan semua operasinya ketika program killer dijalankan. Untuk MODE_B, ketika program killer dijalankan, program utama akan berhenti tapi membiarkan proses di setiap folder yang masih berjalan sampai selesai(semua folder terisi gambar, terzip lalu di delete).
```
#### Source Code
```c
void coba_Kill(char *tanda2, int pid)
{
    char killname[10000], kill_name[10000];
    strcpy(killname, FOLDER_PATH);
    strcat(killname, "/killer");

    strcpy(kill_name, killname);
    strcat(kill_name, ".c");

    FILE *fp;
    fp = fopen(kill_name, "w");

    if (strcmp(tanda2, "-a") == 0)
    {
        fprintf(fp, "#include <signal.h>\n");
        fprintf(fp, "#include <unistd.h>\n\n");
        fprintf(fp, "int main(int argc, char **argv){\n");
        fprintf(fp, "\tremove(\"%s\");\n", killname);
        fprintf(fp, "\t\tchar *arr[] = {\"pkill\", \"-f\", \"./lukisan\", NULL};\n");
        fprintf(fp, "\texecv(\"/bin/pkill\", arr);\n");
        fprintf(fp, "\treturn 0;\n}");
    }
    else if (strcmp(tanda2, "-b") == 0)
    {
        fprintf(fp, "#include <signal.h>\n");
        fprintf(fp, "#include <unistd.h>\n\n");
        fprintf(fp, "int main(int argc, char **argv){\n");
        fprintf(fp, "\tremove(\"%s\");\n", killname);
        fprintf(fp, "\t\tkill(%d, SIGTERM);\n", getpid());
        fprintf(fp, "\treturn 0;\n}");
    }
    fclose(fp);

    if (fork() == 0)
    {
        char *args[] = {"gcc", kill_name, "-o", killname, NULL};
        execv("/bin/gcc", args);
    }
    wait(NULL);

    if (fork() == 0)
    {
        char *args[] = {"rm", kill_name, NULL};
        execv("/bin/rm", args);
    }
    wait(NULL);
}
```

#### Penjelasan 2d & 2e
- Simpan killer dalam direktori yang dikehendaki:
```c
strcpy(killname, FOLDER_PATH);
strcat(killname, "/killer");
```
- Beri eksistensi .c pada program killer tersebut:
```c
strcpy(kill_name, killname);
strcat(kill_name, ".c");
```
- Buka file killer yang masih kosong:
```c
FILE *fp;
fp = fopen(kill_name, "w");
```
- Buat percabangan di dalam program killer, argumen ```-a``` dan ```-b```. Argumen ```-a``` untuk langsung menghentikan semua operasinya ketika program killer dijalankan dan argumen ```-b``` untuk menghentikan proses namun tetap membiarkan proses di setiap folder yang masih berjalan sampai selesai. Pertama buat program killer dengan argumen ```-a``` terlebih dahulu:
```c
if (strcmp(tanda2, "-a") == 0)
{
    fprintf(fp, "#include <signal.h>\n");
    fprintf(fp, "#include <unistd.h>\n\n");
    fprintf(fp, "int main(int argc, char **argv){\n");
    fprintf(fp, "\tremove(\"%s\");\n", killname);
    fprintf(fp, "\t\tchar *arr[] = {\"pkill\", \"-f\", \"./lukisan\", NULL};\n");
    fprintf(fp, "\texecv(\"/bin/pkill\", arr);\n");
    fprintf(fp, "\treturn 0;\n}");
}
```
- Buat percabangan untuk argumen ```-b```:
```c
else if (strcmp(tanda2, "-b") == 0)
{
    fprintf(fp, "#include <signal.h>\n");
    fprintf(fp, "#include <unistd.h>\n\n");
    fprintf(fp, "int main(int argc, char **argv){\n");
    fprintf(fp, "\tremove(\"%s\");\n", killname);
    fprintf(fp, "\t\tkill(%d, SIGTERM);\n", getpid());
    fprintf(fp, "\treturn 0;\n}");
}
```
- Generate program killer tersebut menggunakan:
```c
if (fork() == 0)
{
    char *args[] = {"gcc", kill_name, "-o", killname, NULL};
    execv("/bin/gcc", args);
}
wait(NULL);
```
- Buat program untuk mendelete dirinya sendiri:
```c
if (fork() == 0)
{
    char *args[] = {"rm", kill_name, NULL};
    execv("/bin/rm", args);
}
wait(NULL);
```
### Source Code lengkap
```c
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <wait.h>
#include <dirent.h>

#define FOLDER_PATH "/home/ikikk/Documents/sisop-praktikum-modul-2-2023-hs-e01/soal2"
#define PIC_URL "https://picsum.photos/%d"

void coba_Kill(char *tanda2, int pid)
{
    char killname[10000], kill_name[10000];
    // simpan killer di direktori
    strcpy(killname, FOLDER_PATH);
    strcat(killname, "/killer");

    strcpy(kill_name, killname);
    strcat(kill_name, ".c");

    FILE *fp;
    fp = fopen(kill_name, "w");

    if (strcmp(tanda2, "-a") == 0)
    {
        fprintf(fp, "#include <signal.h>\n");
        fprintf(fp, "#include <unistd.h>\n\n");
        fprintf(fp, "int main(int argc, char **argv){\n");
        fprintf(fp, "\tremove(\"%s\");\n", killname);
        fprintf(fp, "\t\tchar *arr[] = {\"pkill\", \"-f\", \"./lukisan\", NULL};\n");
        fprintf(fp, "\texecv(\"/bin/pkill\", arr);\n");
        fprintf(fp, "\treturn 0;\n}");
    }
    else if (strcmp(tanda2, "-b") == 0)
    {
        fprintf(fp, "#include <signal.h>\n");
        fprintf(fp, "#include <unistd.h>\n\n");
        fprintf(fp, "int main(int argc, char **argv){\n");
        fprintf(fp, "\tremove(\"%s\");\n", killname);
        fprintf(fp, "\t\tkill(%d, SIGTERM);\n", getpid());
        fprintf(fp, "\treturn 0;\n}");
    }
    fclose(fp);

    if (fork() == 0)
    {
        char *args[] = {"gcc", kill_name, "-o", killname, NULL};
        execv("/bin/gcc", args);
    }
    wait(NULL);
    if (fork() == 0)
    {
        char *args[] = {"rm", kill_name, NULL};
        execv("/bin/rm", args);
    }
    wait(NULL);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("eror1\n");
        return 1;
    }
    pid_t process_id = 0;
    pid_t sid = 0;
    process_id = fork();
    if (process_id < 0)
    {
        printf("fork failed!\n");
        exit(1);
    }
    if (process_id > 0)
    {
        exit(0);
    }
    umask(0);
    sid = setsid();
    if (sid < 0)
    {
        exit(1);
    }
    chdir("/home/ikikk/Documents/sisop-praktikum-modul-2-2023-hs-e01/soal2");
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    char tanda[100];
    strcpy(tanda, argv[1]);
    coba_Kill(tanda, (int)getpid());

    while (1)
    {
        if (fork() == 0)
        {
            char folder_name[100], url[100], img[100], img_name[100];
            time_t now;
            struct tm *timeinfo;
            time(&now);
            timeinfo = localtime(&now);
            DIR *dir;
            dir = opendir(FOLDER_PATH);
            if ((dir) != NULL)
            {
                strftime(folder_name, sizeof(folder_name), "%Y-%m-%d_%H:%M:%S", timeinfo);
                mkdir(folder_name, 0777);
            }
            wait(NULL);
            for (int i = 0; i < 15; i++)
            {
                if (fork() == 0)
                {
                    time_t now1;
                    struct tm *timeinfo1;
                    time(&now1);
                    timeinfo1 = localtime(&now1);
                    strcpy(img_name, folder_name);
                    strcat(img_name, "/");
                    strftime(img, sizeof(img), "%Y-%m-%d_%H:%M:%S", timeinfo1);
                    strcat(img_name, img);
                    strcat(img_name, ".jpg");
                    sprintf(url, PIC_URL, (((int)now1) % 1000) + 50);
                    char *argv[] = {"wget", "-qO", img_name, url, NULL};
                    execv("/bin/wget", argv);
                }
                sleep(5);
            }
            wait(NULL);
            if (fork() == 0)
            {
                char zip_name[200];
                strcpy(zip_name, folder_name);
                strcat(zip_name, ".zip");
                char *argv[] = {"zip", "-rmq", zip_name, folder_name, NULL};
                execv("/usr/bin/zip", argv);
            }
            wait(NULL);
            exit(EXIT_SUCCESS);
        }
        sleep(30);
    }
}
```

### Dokumentasi
![Screenshot 2023-04-07 215644](https://user-images.githubusercontent.com/86828535/230628009-d42f3628-78d1-44c9-85cc-370f31846d4e.png)
![Screenshot 2023-04-07 215746](https://user-images.githubusercontent.com/86828535/230628176-418d3d05-6da7-4fad-8484-72cad1ea7375.png)
![Screenshot 2023-04-07 215913](https://user-images.githubusercontent.com/86828535/230628367-885d992c-89d2-4fad-a93b-4f77089f0fd1.png)
![Screenshot 2023-04-07 220028](https://user-images.githubusercontent.com/86828535/230628555-d140c5de-4a21-4125-87f5-f422d6b807f3.png)

### Output
```text
Output yang dihasilkan adalah berikut :
- Folder dengan nama timestamp, folder ini masih ada hingga proses download selesai. 
- Setelah proses download selesai, folder akan menghilang digantikan dengan folder zip dengan nama yang sama
- 15 Gambar di dalam setiap folder yang berhasil di zip
- Folder zip yang berisi 15 gambar, ini adalah hasil akhir dari semua proses di nomor 2
- Program killer untuk kill proses pembuatan folder, download gambar, maupun zip serta kill dirinya sendiri
```

### Kendala
```text
Kendala yang dihadapi adalah kesulitan saat menggunakan fungsi secara modularisasi. 
Jadi kami membuatnya menjadi satu dalam fungsi main kecuali program killernya
```

## Soal3
```text
    Ten Hag adalah seorang pelatih Ajax di Liga Belanda. Suatu hari, Ten Hag mendapatkan tawaran untuk menjadi manajer Manchester United. 
    Karena Ten Hag masih mempertimbangkan tawaran tersebut, ia ingin mengenal para pemain yang akan dilatih kedepannya. Dikarenakan Ten 
    Hag hanya mendapatkan url atau link database mentah para pemain bola, maka ia perlu melakukan klasifikasi pemain Manchester United. 
    Bantulah Ten Hag untuk mengenal para pemain Manchester United tersebut hanya dengan 1 Program C bernama “filter.c”20uz7t20X3atwrq 

    a.	Pertama-tama, Program filter.c akan mengunduh file yang berisikan database para pemain bola. Kemudian dalam program yang sama 
    diminta dapat melakukan extract “players.zip”. Lalu hapus file zip tersebut agar tidak memenuhi komputer Ten Hag.
    b.	Dikarenakan database yang diunduh masih data mentah. Maka bantulah Ten Hag untuk menghapus semua pemain yang bukan dari Manchester 
    United yang ada di directory.  
```
## Jawaban A dan B
### Step 1 : Membuat fungsi download file
``` bash
void DownloadFile(const char *url, const char *downloadName, char *folderName  ) {
    pid_t pidMkDir = fork();
    int status;

    char baseFolder[256] = "/home/rulelulu/Documents/praktikum/modul2/soal3/";
    strcat(baseFolder, folderName);
    if (pidMkDir < 0) {
        perror("Failed to fork\n"); //print error message
        exit(EXIT_FAILURE); //terminate the program when an error occurs.
    }
    else if (pidMkDir == 0) {
        execl("/bin/mkdir", "mkdir", folderName, NULL);
         
    } else {
        while ((wait(&status)) > 0);
        pid_t pidDownload = fork();
        int status2;

        // Download zip
        if (pidDownload == 0) {
            char *url_new = malloc(128 * sizeof(char)); //membuat memory buffer
            
            if (!url_new) {
                exit(EXIT_FAILURE);
            }

            sprintf(url_new, "https://drive.google.com/uc?export=download&id=%s", url);
            printf("Downloading file : %s\n", url);
            execl("/bin/wget", "wget", "--no-check-certificate", url_new, "-O", downloadName, NULL);
            free(url_new);

        } else {
            while ((wait(&status2)) > 0);
            pid_t pidUzipping = fork();
            int status3;

            // unzziping zip
            if (pidUzipping == 0) {
                unzipping(downloadName);
            } else {
                while ((wait(&status3)) > 0);
                pid_t pidRemove = fork();
                int status4;

                // remove file
                if (pidRemove == 0) {
                    removeFile();
                    removeZip(downloadName);
                } else {
                    while ((wait(&status4)) > 0);
                }
            } 
        }
    }
}
```

**Penjelasan:**
- Membuat sebuah fungsi bernama ```DownloadFile()``` yang digunakan untuk mengunduh file zip dari internet menggunakan perintah wget, memanggil fungsi ```uzziping()``` untuk meng uzip file yang baru didownload, dan menmanggil fungsi ```removeZip()``` untuk mengapus file zip yang baru didownload agar tidak membuhi memori komputer TenHag.
- Mendeklarasikan variabel ```pidMkDir``` dan status dengan tipe data pid_t dan int. pidMkDir digunakan untuk menyimpan nilai dari fungsi ```fork()``` yang akan menghasilkan child process.
  ```pid_t pidMkDir = fork()```.
- Mendeklarasikan sebuah array char bernama baseFolder dengan ukuran 256 dan nilai ```"/home/rulelulu/Documents/praktikum/modul2/soal3/".``` Kemudian menambahkan nilai dari parameter folderName ke dalam baseFolder menggunakan fungsi ```strcat().```
  ```strcat(baseFolder, folderName)```
- Jika nilai dari pidMkDir sama dengan 0, maka program sedang berjalan di child process dan akan menjalankan perintah untuk membuat direktori baru menggunakan fungsi ```execl().```
  ```execl("/bin/mkdir", "mkdir", folderName, NULL)```
- Melakukan ``wait()`` hingga status child process yang terdahulu selesai. Kemudian melakukan ``fork() ``lagi untuk membuat child process baru bernama ```pidDownload```. Variabel status2 digunakan untuk menyimpan status dari child process tersebut.
- Jika ```pidDownload``` sama dengan 0, maka ini adalah proses anak yang baru saja dibuat. Selanjutnya adalah mendeklarasikan variabel pointer dengan alokasi memori seukuran ```128*sizeof(char) url_new``` sebagai gabungan url drive google dengan link filenya ```char *url_new = malloc(128 * sizeof(char))``` dan menambahkan nilai dari url ke dalamnya menggunakan sprintf().
  ```sprintf(url_new, "https://drive.google.com/uc?export=download&id=%s", url)```
- Setelah itu, mengeprint pesan "Downloading file : %s\n" yang menampilkan URL file yang sedang didownload, kemudian menjalankan perintah wget dengan ```execl()```.
  ```execl("/bin/wget", "wget", "--no-check-certificate", url_new, "-O", downloadName, NULL)```
- Dilakukan pengosongan memori agar bisa digunakan lagi dengan free ```free(url_new)```
  Setelah itu, dealokasikan memori yang
- Setelah itu digabungkan string antara url drive google dengan link file menggunakan snprintf
  ```snprintf(url_new, 128, "https://drive.google.com/uc?export=download&id=%s", url)```
- Jika nilai pidDownload lebih besar dari 0, maka ini adalah proses induk. Mendeklarasikan variabel ````pidUzipping```` dan ```status3``` dengan tipe data ```pid_t``` dan ```int```. ````pidUzipping```` digunakan untuk menyimpan nilai dari fungsi ```fork()``` yang akan menghasilkan child process.
- Jika nilai dari ````pidUzipping```` sama dengan 0, maka program sedang berjalan di child process dan akan melanjutkan ke proses selanjutnya yaitu mengekstrak file zip yang telah diunduh dengan memanggil fungsi ```unzipping()```.
- Jika nilai ````pidUzipping```` lebih besar dari 0, maka ini adalah proses induk. Mendeklarasikan variabel ```pidRemove``` dan status4 dengan tipe data ``pid_t`` dan ``int``. ```pidRemove``` digunakan untuk menyimpan nilai dari fungsi``` fork()``` yang akan menghasilkan child process.
- Jika nilai dari ```pidRemove``` sama dengan 0, maka program sedang berjalan di child process dan akan melanjutkan ke proses selanjutnya yaitu menghapus file zip yang telah diunduh dengan memanggil fungsi ```removeFile()``` untuk mengapus file yang tidak diperlukan dan ```removeZip()``` untuk menghapus file zip.
- Setelah child process selesai, program akan berakhir dengan kode keluaran EXIT_SUCCESS.

## Penjelasan Fungsi Unzipping()

Berikut merupakan penjelasan dari fungsi unzipping()

``` bash
void unzipping(const char *downloadName) {
    printf("Unzipping file : %s\n", downloadName);
    chdir("/home/rulelulu/Documents/praktikum/modul2/soal3/");
    execl("/bin/unzip", "unzip", downloadName , NULL);
    exit(0);
} 
```
**Penjelasan:**

Dalam fungsi diatas ada beberapa tahap :
- ```printf("Unzipping file : %s\n", downloadName)```Menampilkan pesan ke layar bahwa proses unzip file sedang dilakukan dengan nama file yang akan di-unzip.
- ``` chdir("/home/rulelulu/Documents/praktikum/modul2/soal3/")``` Mengubah direktori kerja saat ini menjadi direktori yang telah ditentukan. Hal ini dilakukan agar hasil unzip file akan disimpan pada direktori tersebut.
- ```execl("/bin/unzip", "unzip", downloadName , NULL)``` Melakukan eksekusi command unzip dengan menggunakan fungsi ```execl()```. Command ini akan melakukan proses unzip file dengan nama yang telah diberikan sebelumnya.

## Penjelasan Fungsi removeFile ()

fungsi ```removeFile()``` digunakan untuk menghapus file-file yang terdapat di dalam folder /home/rulelulu/Documents/praktikum/modul2/soal3/players, kecuali file dengan nama ManUtd
 ``` bash
void removeFile() {
    pid_t pidRemove;
    int status4;
    char baseFolder[] = "/home/rulelulu/Documents/praktikum/modul2/soal3/players";
    char* del[4] = {"sh", "-c", "rm -r $(ls | grep -v ManUtd)", NULL};

    pidRemove = fork();
    if (pidRemove < 0) {
        exit(EXIT_FAILURE);
    } else if (pidRemove == 0) {
        if (chdir(baseFolder) < 0) {
            printf("Failed to open %s\n", baseFolder);
            exit(EXIT_FAILURE);
        }
        execvp("sh", del);
        exit(EXIT_FAILURE);
    } else {
        while ((wait(&status4)) > 0);
        printf("players not from Manchester United are removed \n");
    }
}
```
**Penjelasan:**

Dalam fungsi diatas ada beberapa tahap :
- ``pid_t pidRemove`` Membuat variabel pidRemove yang bertipe data ``pid_t`` yang digunakan untuk menyimpan proses ID dari proses yang akan dilakukan ```fork()```.
- ```int status4```; : Membuat variabel status4 yang bertipe data int yang digunakan untuk menyimpan status keluaran dari proses yang dijalankan.
- ```char baseFolder[] = "/home/rulelulu/Documents/praktikum/modul2/soal3/players"``` Mendefinisikan variabel baseFolder yang berisi path direktori tempat file-file yang akan dihapus disimpan.
- ```char* del[4] = {"sh", "-c", "rm -r $(ls | grep -v ManUtd)", NULL}``` Mendefinisikan variabel del yang berisi command yang akan dijalankan menggunakan sh. Command ini akan menghapus semua file di dalam direktori baseFolder kecuali file dengan nama ManUtd.
- ```pidRemove = fork()```Membuat proses baru menggunakan fungsi ``fork()`` dan menyimpan proses ID-nya ke dalam variabel pidRemove.
- Jika proses fork berhasil dilakukan dan program sedang berjalan di dalam child process, maka akan menjalankan perintah-perintah di bawah ini:
    - ```if (chdir(baseFolder) < 0) { printf("Failed to open %s\n", baseFolder); exit(EXIT_FAILURE); }``` Mengubah direktori kerja ke direktori baseFolder. Jika gagal, maka program akan keluar dengan kode error.
    - ```execvp("sh", del)``` Menjalankan command yang telah didefinisikan sebelumnya pada variabel del menggunakan fungsi ```execvp()```.
- ```else { while ((wait(&status4)) > 0); printf("players not from Manchester United are removed \n"); }``` Jika program sedang berjalan pada parent process, maka akan menunggu child process yang sedang berjalan selesai dan menampilkan pesan bahwa file-file di dalam direktori baseFolder kecuali file dengan nama ManUtd telah dihapus.

### Penjelasan Fungsi removeZip ()

Fungsi di bawah adalah sebuah fungsi yang digunakan untuk menghapus file zip yang telah di-download dan di-unzip sebelumnya.
Berikut ini adalah penjelasan dari setiap baris kode di dalam fungsi ```removeZip()```

 ``` bash
void removeZip(const char *downloadName) {
    printf("remove zip file : %s\n", downloadName);
    execl("/bin/rm", "rm", downloadName, NULL);
    exit(0);
}
```

**Penjelasan:**

Dalam fungsi diatas ada beberapa tahap :
-	```printf("remove zip file : %s\n", downloadName)``` Menampilkan pesan ke layar bahwa proses penghapusan file zip sedang dilakukan dengan nama file yang akan dihapus.
-	```execl("/bin/rm", "rm", downloadName, NULL);``` Melakukan eksekusi command rm dengan menggunakan fungsi ```execl()```. Command ini akan melakukan proses penghapusan file zip dengan nama yang telah diberikan sebelumnya.


## Jawaban C
```text
    c.	Setelah mengetahui nama-nama pemain Manchester United, Ten Hag perlu untuk mengkategorikan pemain tersebut sesuai dengan 
    posisi mereka dalam waktu bersamaan dengan 4 proses yang berbeda. Untuk kategori folder akan menjadi 4 yaitu Kiper, Bek, 
    Gelandang, dan Penyerang.
```

``` bash
void category(char *baseFolder) {
    DIR *dir;
    dir = opendir(baseFolder);
    //printf("dir now :%s\n",baseFolder);
    cekDir(baseFolder);

    struct dirent *ep;
    while ((ep = readdir(dir)) != NULL)
    {
        char *ext = strrchr(ep->d_name, '.');
        if (ext != NULL && (strcmp(ext, ".png") == 0 ))
        {
            char *nama = strtok(ep->d_name, "_");
            char *tim = strtok(NULL, "_");
            char *posisi = strtok(NULL, "_");
            char *ranking = strtok(NULL, "_");
            // printf("ed :%s\n",ep->d_name );
            createFolder(posisi);

            char result[1024];
            sprintf(result, "%s_%s_%s_%s", nama, tim, posisi, ranking);
            moveFile(result, posisi, baseFolder);
        }
    }
    closedir(dir);
}
```
**Penjelasan:**

Fungsi ```category() ```digunakan untuk mengkategorikan gambar-gambar pemain sepak bola berdasarkan posisi pemainnya.
Berikut adalah penjelasan per baris dari fungsi category:

- ```DIR *dir`` Deklarasi variabel pointer untuk menyimpan direktori.
- ```dir = opendir(baseFolder)``` Membuka direktori dengan menggunakan path yang di-passing melalui parameter fungsi category.
- ```cekDir(baseFolder)``` Memanggil fungsi cekDir untuk mengecek apakah folder sesuai dengan yang dibutuhkan oleh program. Jika belum ada, maka folder akan dibuat.
- ```struct dirent *ep``` Deklarasi variabel pointer untuk menyimpan informasi dari direktori.
- ```while ((ep = readdir(dir)) != NULL)``` Melakukan iterasi selama informasi direktori masih tersedia.
- ```char *ext = strrchr(ep->d_name, '.')``` Mencari extensi file pada setiap informasi direktori menggunakan fungsi strrchr.
- ``if (ext != NULL && (strcmp(ext, ".png") == 0 ))`` Memastikan bahwa file tersebut berekstensi .png.
- ```char *nama = strtok(ep->d_name, "_")``` Menggunakan fungsi strtok untuk membagi string nama file berdasarkan separator underscore dan mengambil nama pemain.
- ```char *tim = strtok(NULL, "_")``` Menggunakan fungsi strtok untuk membagi string nama file berdasarkan separator underscore dan mengambil nama tim pemain.
- ```char *posisi = strtok(NULL, "_")``` Menggunakan fungsi strtok untuk membagi string nama file berdasarkan separator underscore dan mengambil posisi pemain.
- ```char *ranking = strtok(NULL, "_")``` Menggunakan fungsi strtok untuk membagi string nama file berdasarkan separator underscore dan mengambil ranking pemain.
- ```createFolder(posisi)``` Memanggil fungsi createFolder untuk membuat folder sesuai dengan posisi pemain.
- ```char result[1024]``` Deklarasi variabel untuk menyimpan hasil akhir setelah string nama file diproses.
- ```sprintf(result, "%s_%s_%s_%s", nama, tim, posisi, ranking) ``` Menggabungkan string nama pemain, nama tim, posisi pemain, dan ranking pemain menjadi satu string menggunakan fungsi sprintf.
- ```moveFile(result, posisi, baseFolder)``` Memanggil fungsi moveFile untuk memindahkan file ke dalam folder yang sesuai dengan posisi pemain.
- ```closedir(dir)``` Menutup direktori.

## Jawaban D
```text
    d.	Setelah mengkategorikan anggota tim Manchester United, Ten Hag memerlukan Kesebelasan Terbaik untuk menjadi senjata utama 
    MU berdasarkan rating terbaik dengan wajib adanya kiper, bek, gelandang, dan penyerang. (Kiper pasti satu pemain). 
    Untuk output nya akan menjadi Formasi_[jumlah bek]-[jumlah gelandang]-[jumlah penyerang].txt dan akan ditaruh di /home/[users]/
```
## Penjelasan fungsi makeTxt()
```bash
void makeTxt() {
    char path_kiper[] = "/home/rulelulu/Documents/praktikum/modul2/soal3/Kiper";
    char path_bek[] = "/home/rulelulu/Documents/praktikum/modul2/soal3/Bek";
    char path_gelandang[] = "/home/rulelulu/Documents/praktikum/modul2/soal3/Gelandang";
    char path_penyerang[] = "/home/rulelulu/Documents/praktikum/modul2/soal3/Penyerang";

    FILE *output_file;
    output_file = fopen("/home/rulelulu/Documents/praktikum/modul2/soal3/daftarPemain.txt", "w");

    if (output_file == NULL) {
        printf("Tidak dapat membuka file teks\n");
        exit(EXIT_FAILURE);
    }
    listFiles(path_kiper, output_file);
    listFiles(path_bek, output_file);
    listFiles(path_gelandang, output_file);
    listFiles(path_penyerang, output_file);

    fclose(output_file);
}
```
**Penjelasan:**
- Deklarasi variabel ```path_kiper``` dengan nilai ```string "/home/rulelulu/Documents/praktikum/modul2/soal3/Kiper"```.
- Deklarasi variabel ```path_bek``` dengan nilai ```string "/home/rulelulu/Documents/praktikum/modul2/soal3/Bek"```.
- Deklarasi variabel ```path_gelandang``` dengan nilai ```string "/home/rulelulu/Documents/praktikum/modul2/soal3/Gelandang"```.
- Deklarasi variabel ```path_penyerang``` dengan nilai string ```"/home/rulelulu/Documents/praktikum/modul2/soal3/Penyerang"```.
- Deklarasi file pointer ```output_file``` dan membuka file teks ```"/home/rulelulu/Documents/praktikum/modul2/soal3/daftarPemain.txt"``` dalam mode write.
- Jika file tidak dapat dibuka, maka program akan menampilkan pesan kesalahan dan keluar.
- Memanggil fungsi ```listFiles()``` dengan argumen ```path_kiper``` dan ```output_file```.
- Memanggil fungsi ```listFiles()``` dengan argumen ```path_bek``` dan ```output_file```.
- Memanggil fungsi ```listFiles()``` dengan argumen ```path_gelandang``` dan ```output_file```.
- Memanggil fungsi ```listFiles()``` dengan argumen ```path_penyerang``` dan ```output_file```.
- Menutup file yang telah dibuka dengan ```fclose(output_file)```.

## Penjelasan fungsi listFiles()
```text
    Fungsi listFiles memiliki dua parameter yaitu path dan output_file. Fungsi ini berfungsi untuk membaca semua file dalam direktori 
    yang ditentukan oleh path dan menulis informasi tentang pemain ke dalam file yang diberikan oleh output_file.
```
```bash
void listFiles(const char *path, FILE *output_file) {
    struct dirent *dp;
    DIR *dir = opendir(path);
    if (!dir) return;

    while ((dp = readdir(dir)) != NULL) {
        if (strstr(dp->d_name, ".png")) {
            char player_name[100];
            strcpy(player_name, dp->d_name);
            //printf("playername %s\n", player_name);
            // Memotong ekstensi ".png" dari nama file
            player_name[strlen(player_name) - 4] = '\0';

            // Memisahkan nama pemain dan rating
            char *token;
            token = strtok(player_name, "_");
            fprintf(output_file, "%s\n", player_name);
            while (token != NULL) {
                token = strtok(NULL, "_");
                if (token != NULL) {
                    fprintf(output_file, "%s ", token);
                }
            }
            fprintf(output_file, "\n");
        }
    }
    closedir(dir);
}

```
**Penjelasan:**

Berikut merupakan penjelasan dari fungsi ```listFiles()``` :
- ```struct dirent *dp``` membuat variabel bertipe dirent dengan nama dp
- ```DIR *dir = opendir(path)``` membuka direktori yang diberikan oleh path dan mengembalikan file descriptor ke direktori
- ```if (!dir) return jika gagal membuka direktori```, maka keluar dari fungsi dengan return
- ```while ((dp = readdir(dir)) != NULL)``` membaca setiap file dalam direktori yang dibuka dan menyimpan informasi file dalam variabel dp
- ```if (strstr(dp->d_name, ".png"))``` jika nama file yang dibaca memiliki ekstensi .png, maka lakukan:
- ```char player_name[100]``` membuat array karakter dengan ukuran 100 bernama player_name
- ```strcpy(player_name, dp->d_name)``` menyalin nama file ke dalam player_name
- ```player_name[strlen(player_name) - 4] = '\0'``` menghapus ekstensi .png dari player_name
- ```char *token``` membuat pointer karakter bernama token
- ```token = strtok(player_name, "_")``` memotong nama pemain dari player_name menggunakan _ sebagai delimiter dan menyimpan hasilnya di dalam token
- ```fprintf(output_file, "%s\n", player_name)``` menulis nama pemain ke dalam output_file
- ```while (token != NULL)``` jika token masih ada, lakukan:
- ```token = strtok(NULL, "_")``` memotong informasi klub, posisi, dan rating menggunakan _ sebagai delimiter dan menyimpan hasilnya di dalam token
- ```if (token != NULL)``` jika token masih ada, maka tuliskan informasi tersebut ke dalam output_file dengan format yang ditentukan
- ```fprintf(output_file, "\n")``` menulis karakter baris baru ke dalam output_file
- ```closedir(dir)``` menutup direktori yang telah dibuka

## Mmanggil Fungsi readPlayersData()
```bash
struct Pemain {
    char nama[50];
    char club[50];
    char posisi[20];
    int rating;
};

struct Pemain players[100];

void readPlayersData(struct Pemain *players, int jumlah_pemain) {
    FILE *fp;
    fp = fopen("/home/rulelulu/Documents/praktikum/modul2/soal3/daftarPemain.txt", "r");
    if (fp == NULL) {
        printf("File daftarPemain.txt tidak ditemukan!");
        exit(1);
    }
    
    int i = 0;
    while (!feof(fp)) {
        fscanf(fp, "%s %s %s %d", players[i].nama, players[i].club, players[i].posisi, &players[i].rating);
        i++;
    }
    jumlah_pemain = i-1;
    fclose(fp);
```
**Penjelasan:**

Kode di atas mendefinisikan sebuah struktur struct Pemain yang berisi informasi tentang pemain, yaitu nama, club, posisi, dan rating.
Kemudian, sebuah array of struct players yang terdiri dari 100 struct struct Pemain juga dideklarasikan. Mendefinisikan fungsi
```readPlayersData()``` yang menerima dua parameter yaitu pointer ke players dan ```jumlah_pemain```. Fungsi ini bertujuan untuk
membaca data pemain dari file ```daftarPemain.txt``` dan menyimpannya ke dalam array players. Fungsi ini akan membuka file
````daftarPemain.txt```` menggunakan fungsi fopen dan memeriksa apakah file tersebut berhasil dibuka atau tidak. Jika file tidak
berhasil dibuka, maka program akan mengeluarkan pesan error dan keluar dari program menggunakan fungsi exit.
- Membuka file ````daftarPemain.txt```` dengan mode ```r``` (read).
- Menginisialisasi variabel i dengan nilai 0. Kemudian, membaca data dari file dengan menggunakan ````fscanf```` dengan format %s %s %s %d dan menyimpannya ke dalam array players pada indeks ke-i. Setelah itu, variabel i ditambah 1.
- Menetapkan nilai ````jumlah_pemain```` dengan nilai i-1, karena i terakhir sudah melebihi jumlah data pemain yang sebenarnya.
- Menutup file yang dibuka menggunakan ````fclose````.

## Memanggil Fungsi sortPlayer()
```bash
void sortPlayer(struct Pemain *pemain, int jumlah_pemain) {
    struct Pemain temp;
    for (int i = 0; i < jumlah_pemain-1; i++) {
        for (int j = i+1; j < jumlah_pemain; j++) {
            if (pemain[i].rating < pemain[j].rating) {
                temp = pemain[i];
                pemain[i] = pemain[j];
                pemain[j] = temp;
            }
        }
    }
    // for (int i = 0; i < jumlah_pemain; i++) {
    //     printf("%s %s %s %d\n", pemain[i].nama, pemain[i].club, pemain[i].posisi, pemain[i].rating);
    // }
}
```
**Penjelasan:**

Fungsi``` sortPlayers()``` digunakan untuk mengurutkan players yang berada didalam file ```daftarPemain.txt``` berdasarkan nilai
rating dari yang terbesar ke yang terkecil.
Berikut merupakan penjelasannya :
- Membuat sebuah struct Pemain dengan nama temp yang nantinya akan digunakan untuk menukar pemain yang memiliki rating yang lebih rendah dengan pemain yang memiliki rating yang lebih tinggi.
- Membuka loop for dengan tujuan untuk memindahkan pemain dengan rating yang lebih tinggi ke indeks yang lebih kecil di array.
- Membuka loop for yang nested di dalam loop for, dengan variabel j dimulai dari i+1 dan berakhir sebelum jumlah_pemain. Tujuan dari loop ini adalah untuk membandingkan rating antara pemain pada indeks i dan j, dan menukar posisi apabila rating pemain pada indeks j lebih tinggi daripada rating pemain pada indeks i.
- Menggunakan operator if untuk membandingkan rating pemain pada indeks i dan j.
- Jika rating pemain pada indeks i lebih kecil daripada rating pemain pada indeks j, maka pemain pada indeks i ditukar dengan pemain pada indeks j dengan menggunakan variabel temp sebagai variabel penampung.
- Menutup loop for pada baris 3 dan 4.
- Mengakhiri fungsi ```sortPlayer()```.

## Memanggil Fungsi buatTim ()
```bash
void buatTim(int jumlah_bek, int jumlah_gelandang, int jumlah_penyerang) {
    tentukanKesebelasan(players, 30, jumlah_bek, jumlah_gelandang, jumlah_penyerang);
}
```
**Penjelasan:**

Fungsi ```buatTim()``` digunakan untuk memanggil fungsi ```tentukanKesebelasan()```

## penjelasan fungsi tentukanKesebelasan ()
```bash
void tentukanKesebelasan(struct Pemain *pemain, int jumlah_pemain, int jumlah_bek, int jumlah_gelandang, int jumlah_penyerang) {
    if (jumlah_bek + jumlah_gelandang + jumlah_penyerang + 1 > jumlah_pemain) {
        printf("players not enough\n");
        return;
    }
    
    int index_kiper = 0;
    int index_bek = 0;
    int index_gelandang = 0;
    int index_penyerang = 0;
    
    // Cari kiper terbaik
    struct Pemain kiper_terbaik[1];
    for (int i = 0; i < jumlah_pemain; i++) {
        if (strcmp(pemain[i].posisi, "Kiper") == 0) {
            if (pemain[i].rating > pemain[index_kiper].rating) {
            index_kiper = i;
            }
        }
    }
    // Cari bek terbaik
    struct Pemain bek_terbaik[jumlah_bek];
    for (int i = 0; i < jumlah_pemain; i++) {
        if (strcmp(pemain[i].posisi, "Bek") == 0) {
            if (index_bek < jumlah_bek) {
                bek_terbaik[index_bek] = pemain[i];
                index_bek++;
            } else {
                for (int j = 0; j < jumlah_bek; j++) {
                    if (pemain[i].rating > bek_terbaik[j].rating) {
                        bek_terbaik[j] = pemain[i];
                        break;
                    }
                }
            }
        }
    }

    // Cari gelandang terbaik
    struct Pemain gelandang_terbaik[jumlah_gelandang];
    for (int i = 0; i < jumlah_pemain; i++) {
        if (strcmp(pemain[i].posisi, "Gelandang") == 0) {
            if (index_gelandang < jumlah_gelandang) {
                gelandang_terbaik[index_gelandang] = pemain[i];
                index_gelandang++;
            } else {
                for (int j = 0; j < jumlah_gelandang; j++) {
                    if (pemain[i].rating > gelandang_terbaik[j].rating) {
                        gelandang_terbaik[j] = pemain[i];
                        break;
                    }
                }
            }
        }
    }

    // Cari penyerang terbaik
    struct Pemain penyerang_terbaik[jumlah_penyerang];
    for (int i = 0; i < jumlah_pemain; i++) {
        if (strcmp(pemain[i].posisi, "Penyerang") == 0) {
            if (index_penyerang < jumlah_penyerang) {
                penyerang_terbaik[index_penyerang] = pemain[i];
                index_penyerang++;
            } else {
                for (int j = 0; j < jumlah_penyerang; j++) {
                    if (pemain[i].rating > penyerang_terbaik[j].rating) {
                        penyerang_terbaik[j] = pemain[i];
                        break;
                    }
                }
            }
        }
    }

    // Tulis ke file output
    char output_file[100];
    sprintf(output_file, "/home/rulelulu/Formasi_%d-%d-%d.txt", jumlah_bek, jumlah_gelandang, jumlah_penyerang);
    FILE* out = fopen(output_file, "w");

    fprintf(out, "Formasi :%d %d %d\n", jumlah_bek, jumlah_gelandang, jumlah_penyerang);
    fprintf(out, "--Kiper--\n");
    fprintf(out, "Kiper: %s_Club: %s_Rating: %d\n", pemain[index_kiper].nama, pemain[index_kiper].club, pemain[index_kiper].rating);

    fprintf(out, "--Bek--\n");
    for (int i = 0; i < jumlah_bek; i++) {
        fprintf(out, "Bek: %s_Club: %s_Rating: %d\n", bek_terbaik[i].nama, bek_terbaik[i].club, bek_terbaik[i].rating);
    }

    fprintf(out, "--Gelandang--\n");
    for (int i = 0; i < jumlah_gelandang; i++) {
    fprintf(out, "Gelandang: %s_Club: %s_Rating: %d\n", gelandang_terbaik[i].nama, gelandang_terbaik[i].club, gelandang_terbaik[i].rating);
    }

    fprintf(out, "--Penyerang--\n");
    for (int i = 0; i < jumlah_penyerang; i++) {
    fprintf(out, "Penyerang: %s_Club: %s_Rating: %d\n", penyerang_terbaik[i].nama, penyerang_terbaik[i].club, penyerang_terbaik[i].rating);
    }

    fclose(out);

    printf("Formasi write in %s\n", output_file);
} 
```
**Penjelasan:**

Fungsi ```tentukanKesebelasan()``` digunakan untuk menentukan kesebelasan sepakbola dengan formasi tertentu. Setelah itu, fungsi ini
akan menuliskan formasi dan pemain-pemain terbaik ke dalam sebuah file output dengan nama dan lokasi tertentu. Nama file output
disesuaikan dengan jumlah pemain di tiap posisi. Terakhir, fungsi ini akan menampilkan pesan bahwa formasi sudah ditulis ke dalam file
output.

Berikut penjelasan dari fungsi di atas :
- Pertama dilakukan pengecakan menggunakan if else condition dengan syarat Jika jumlah pemain tidak cukup untuk membentuk tim, maka akan muncul pesan ```"players not enough"```.
- Inisialisasi variabel ```index_kiper```, ```index_bek```, ```index_gelandang```, dan ```index_penyerang``` dengan nilai awal 0.
- Deklarasi sebuah array kiper_terbaik dengan satu elemen bertipe struct Pemain.
  ````struct Pemain kiper_terbaik[1]````
- Looping untuk mencari kiper terbaik berdasarkan rating tertinggi. Jika rating pemain saat ini lebih besar dari rating pemain sebelumnya, maka index kiper terbaik akan diubah. Posisi pemain dicari dengan menggunakan syntax berikut
  ```strcmp(pemain[i].posisi, "Kiper") == 0```
- Deklarasi sebuah array bek_terbaik dengan ukuran sebanyak jumlah_bek elemen bertipe struct Pemain.
  ```struct Pemain bek_terbaik[jumlah_bek]```
- Looping untuk mencari bek terbaik berdasarkan rating tertinggi. Jika jumlah bek terbaik masih kurang dari jumlah_bek, maka pemain saat ini akan dimasukkan ke dalam array. Jika sudah mencapai batas jumlah_bek, maka pemain saat ini akan dibandingkan dengan pemain terburuk pada array bek_terbaik. Jika rating pemain saat ini lebih besar dari rating pemain terburuk, maka pemain terburuk akan digantikan dengan pemain saat ini. Posisi pemain dicari dengan menggunakan syntax berikut
  ```strcmp(pemain[i].posisi, "Bek") == 0```
- Deklarasi sebuah array gelandang_terbaik dengan ukuran ```jumlah_gelandang```
  ```struct Pemain gelandang_terbaik[jumlah_gelandang]```
- Kemudian, fungsi melakukan looping sebanyak jumlah_pemain kali untuk mencari pemain dengan posisi "Gelandang" yang memiliki rating tertinggi. Setiap kali fungsi menemukan pemain dengan posisi "Gelandang", fungsi akan memeriksa apakah index pada array gelandang_terbaik masih kurang dari jumlah_gelandang. Jika masih kurang, pemain tersebut akan disimpan pada index array yang sesuai dan index pada gelandang_terbaik akan di-increment. Namun, jika index pada array gelandang_terbaik sudah sama dengan jumlah_gelandang, fungsi akan melakukan looping sebanyak jumlah_gelandang kali untuk mencari pemain dengan rating tertinggi dan menggantikan pemain dengan rating terendah di dalam array. Posisi pemain dicari dengan menggunakan syntax berikut
  ```strcmp(pemain[i].posisi, "Gelandang") == 0```
- Setelah looping selesai, fungsi kemudian membuka file output dengan nama yang disesuaikan dengan jumlah pemain pada setiap posisi dan menuliskan data formasi dan data pemain dengan posisi dan rating tertinggi ke dalam file tersebut menggunakan fungsi ```fprintf().```
- Hal berikut juga dilakuakan pada posisi penyerang.
- program mendeklarasikan sebuah array bernama ```output_file```. Program menggunakan fungsi ```sprintf``` untuk menyimpan string format alamat file yang akan digunakan untuk menuliskan formasi ke dalam file teks. Alamat file ini akan diisi dengan jumlah pemain di posisi bek, gelandang, dan penyerang yang telah diinput sebelumnya.
- program membuka file teks dengan mode ```w``` (write), yaitu dengan mengakses alamat file yang sudah ditentukan sebelumnya.
- Kemudian, program menuliskan informasi formasi ke dalam file teks menggunakan fungsi ```fprintf```.
- Setelah seluruh pemain terbaik pada masing-masing posisi berhasil ditentukan, maka akan dibuat sebuah file teks dengan nama ```output_file``` yang disimpan pada direktori /home/rulelulu/ menggunakan fungsi ```sprintf``` dan ````fopen````.
- Setelah file berhasil dibuat, dilakukan penulisan informasi mengenai formasi dan pemain pada setiap posisi pada file tersebut menggunakan fungsi ```fprintf```. Pada setiap penulisan informasi pemain, informasi nama, klub, dan rating pemain akan dituliskan pada file.
- Setelah semua informasi telah berhasil dituliskan pada file, file tersebut ditutup menggunakan fungsi ```fclose``` dan kemudian diakhiri dengan mencetak pesan ke layar bahwa formasi telah berhasil dituliskan pada file yang bersangkutan.

## Fungsi main
```bash
int main()
{
    //https://drive.google.com/uc?export=download&id=1oDgj5kSiDO0tlyS7-20uz7t20X3atwrq 
    //https://drive.google.com/file/d/1zEAneJ1-0sOgt13R1gL4i1ONWfKAtwBF/view

    const char *downloadName = "players.zip";
    char* folderName = "players";
    const char* url = "1zEAneJ1-0sOgt13R1gL4i1ONWfKAtwBF";
    const char* folderPath = "./players";
    //3a dan 3b
    DownloadFile( url, downloadName, folderName);

    // 3c
    category("/home/rulelulu/Documents/praktikum/modul2/soal3/players");

    //3d
    makeTxt();
    int jumlah_pemain  = 30;

    readPlayersData( players, jumlah_pemain);
    sortPlayer(players, jumlah_pemain);
    //showPlayers(players, jumlah_pemain);
    //findPlayers(players, jumlah_pemain, "Gelandang");
    int jumlahBek, jumlahGelandang, jumlahPenyerang;
    printf("input Formasi yang kamu inginkan menggunakan enter :\n");
    scanf("%d%d%d", &jumlahBek, &jumlahGelandang, &jumlahPenyerang);

    buatTim(jumlahBek,jumlahGelandang,jumlahPenyerang);
    
} 
```
**Penjelasan:**

Dalam ```main()``` digunakan untuk memanggil fungsi-fungsi yang sudah dijelaskan diatas
- Deklarasi variabel konstan pointer ```(const char *) downloadName ```dengan nilai "players.zip".
- Deklarasi variabel pointer ```(char *) folderName``` dengan nilai "players".
- Deklarasi variabel konstan pointer ```(const char *) url``` dengan nilai "1zEAneJ1-0sOgt13R1gL4i1ONWfKAtwBF".
- Deklarasi variabel konstan pointer ```(const char *) folderPath``` dengan nilai "./players".
- Memanggil fungsi DownloadFile dengan parameter url, downloadName, dan folderName untuk mengunduh file players.zip dan mengekstraknya ke dalam folder players.
- Memanggil fungsi ```category()``` dengan parameter folderPath untuk mengkategorikan pemain berdasarkan posisinya ke dalam folder yang sesuai.
- Memanggil fungsi ```makeTxt()``` untuk membuat file txt untuk setiap posisi pemain di dalam folder pemain masing-masing.
- Deklarasi variabel integer ```jumlah_pemain``` dengan nilai 30.
- Deklarasi array of ```struct player``` dengan nama players dengan ukuran ```jumlah_pemain```.
- Memanggil fungsi ```readPlayersData()``` dengan parameter players, dan ```jumlah_pemain``` untuk membaca data pemain dari file txt yang telah dibuat sebelumnya dan menyimpannya di dalam array players.
- Memanggil fungsi ```sortPlayer()``` dengan parameter players, dan jumlah_pemain untuk mengurutkan pemain berdasarkan nilai rating mereka.
- Memanggil fungsi ```buatTim()``` dengan parameter jumlahBek, jumlahGelandang, dan jumlahPenyerang untuk membuat tim sepak bola dengan formasi yang dimasukkan oleh pengguna.

## Dokumentasi
![Screenshot 1116](https://user-images.githubusercontent.com/105763198/230563977-91fe9561-f7d9-4a3b-b52b-59ee5d110f20.png)
![Screenshot 1126](https://user-images.githubusercontent.com/105763198/230563973-e208a675-78fe-4630-abe8-157618d39b4c.png)
## Output

    Output yang dihasilkan adalah sebagai berikut :
    1. File dari drive sudah terdownload, ini dibuktikan pada output kalimat "Downloading file : 1zEAneJ1-0sOgt13R1gL4i1ONWfKAtwBF"
    2. File yang terdownload di unzip, ini dibuktikan pada output kalimat "Unzipping file : players.zip"
    3. Menghapus file zip, ini dibuktikan pada output kalimat "remove zip file : players.zip"
    4. Mengkategorikan dan memindahkan file sesuai dengan posisinya
![Screenshot 1113](https://user-images.githubusercontent.com/105763198/230563963-8c74692b-26d8-4563-96e0-9f6bba42c908.png)
    5. Membuat kesebelasan terbaik dalam bentuk txt dengan input yang dimasukkan oleh user. FIle txt disimpan dalam "home", ini 
    dibuktikan pada output kalimat "input Formasi yang kamu inginkan menggunakan enter :
    3
    3
    4
    Formasi write in /home/rulelulu/Formasi_3-3-4.txt"
![Screenshot 1114](https://user-images.githubusercontent.com/105763198/230564446-e5ed7152-2d56-4dc3-bc94-6070df0e2930.png)

## Kendala
```text
    Kendala yang kami hadapi, kami sering menjumpai segmentation error. hal ini ternyata baru kami sadari bahwa saat ingin 
    melakukan write ke sebuah file, pastikan permission nya sudah sesuai.
``` 
![Screenshot 1115](https://user-images.githubusercontent.com/105763198/230564264-7bb27753-2a45-4074-82ee-05bab2f86c3a.png)

## Soal4
```text
    Banabil adalah seorang mahasiswa yang rajin kuliah dan suka belajar. Namun naasnya Banabil salah mencari teman, dia diajak ke toko mainan oleh teman-temannya dan teracuni untuk membeli banyak sekali mainan 
    dan kebingungan memilih mainan mana yang harus dibeli. Hal tersebut menyebabkan Banabil kehilangan fokus dalam pengerjaan tugas-tugas yang diberikan oleh dosen nya. Untuk mengembalikan fokusnya, 
    Banabil harus melatih diri sendiri dalam membuat program untuk menjalankan script bash yang menyerupai crontab 
    dan menggunakan bahasa C karena baru dipelajari olehnya.
    Untuk menambah tantangan agar membuatnya semakin terfokus, Banabil membuat beberapa ketentuan custom yang harus dia ikuti sendiri. Ketentuan tersebut berupa:
    - Banabil tidak ingin menggunakan fungsi system(), karena terlalu mudah.
    - Dalam pelatihan fokus time managementnya, Banabil harus bisa membuat program yang dapat menerima argumen berupa Jam (0-23), Menit (0-59), Detik (0-59), 
      Tanda asterisk [ * ] (value bebas), serta path file .sh.
    - Dalam pelatihan fokus untuk ketepatan pilihannya, Banabil ingin programnya dapat mengeluarkan pesan “error” apabila argumen yang diterima program tidak sesuai. 
      Pesan error dapat dibentuk sesuka hati oleh pembuat program. terserah bagaimana, yang penting tulisan error.
    - Terakhir, dalam pelatihan kesempurnaan fokusnya, Banabil ingin program ini berjalan dalam background dan hanya menerima satu config cron.
    - Bonus poin apabila CPU state minimum.
    Contoh untuk run: /program \* 44 5 /home/Banabil/programcron.sh

```
## Jawaban
```bash
int valid1(char *fileName)
{
    int pjg = strlen(fileName);
    if (fileName[0] == '*' && pjg == 1)
    {
        return 0;
    }
    else
    {
        if (pjg == 1)
        {
            if (fileName[0] == '0' || fileName[0] == '1' || fileName[0] == '2' || fileName[0] == '3' || fileName[0] == '4' || fileName[0] == '5' || fileName[0] == '6' || fileName[0] == '7' || fileName[0] == '8' || fileName[0] == '9')
            {
                return 0;
            }
            else
            {
                return 1;
            }
        }
        else if (pjg == 2)
        {
            if (fileName[0] == '1' || fileName[0] == '2')
            {
                if (fileName[1] == '0' || fileName[1] == '1' || fileName[1] == '2' || fileName[1] == '3' || fileName[1] == '4' || fileName[1] == '5' || fileName[1] == '6' || fileName[1] == '7' || fileName[1] == '8' || fileName[1] == '9')
                {
                    return 0;
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                return 1;
            }
        }
        else
        {
            return 1;
        }
    }
}
int valid2(char *fileName)
{
    int pjg = strlen(fileName);
    if (fileName[0] == '*' && pjg == 1)
    {
        return 0;
    }
    else
    {
        if (pjg == 1)
        {
            if (fileName[0] == '0' || fileName[0] == '1' || fileName[0] == '2' || fileName[0] == '3' || fileName[0] == '4' || fileName[0] == '5' || fileName[0] == '6' || fileName[0] == '7' || fileName[0] == '8' || fileName[0] == '9')
            {
                return 0;
            }
            else
            {
                return 1;
            }
        }
        else if (pjg == 2)
        {
            if (fileName[0] == '1' || fileName[0] == '2' || fileName[0] == '3' || fileName[0] == '4' || fileName[0] == '5')
            {
                if (fileName[1] == '0' || fileName[1] == '1' || fileName[1] == '2' || fileName[1] == '3' || fileName[1] == '4' || fileName[1] == '5' || fileName[1] == '6' || fileName[1] == '7' || fileName[1] == '8' || fileName[1] == '9')
                {
                    return 0;
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                return 1;
            }
        }
        else
        {
            return 1;
        }
    }
}

int valid3(char *fileName)
{
    int pjg = strlen(fileName);
    if (fileName[0] == '*' && pjg == 1)
    {
        return 0;
    }
    else
    {
        if (pjg == 1)
        {
            if (fileName[0] == '0' || fileName[0] == '1' || fileName[0] == '2' || fileName[0] == '3' || fileName[0] == '4' || fileName[0] == '5' || fileName[0] == '6' || fileName[0] == '7' || fileName[0] == '8' || fileName[0] == '9')
            {
                return 0;
            }
            else
            {
                return 1;
            }
        }
        else if (pjg == 2)
        {
            if (fileName[0] == '1' || fileName[0] == '2' || fileName[0] == '3' || fileName[0] == '4' || fileName[0] == '5')
            {
                if (fileName[1] == '0' || fileName[1] == '1' || fileName[1] == '2' || fileName[1] == '3' || fileName[1] == '4' || fileName[1] == '5' || fileName[1] == '6' || fileName[1] == '7' || fileName[1] == '8' || fileName[1] == '9')
                {
                    return 0;
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                return 1;
            }
        }
        else
        {
            return 1;
        }
    }
}

int valid4(const char *fileName)
{

    if (!access(fileName, F_OK))
    {
        // printf("The File %s was Found\n", fileName);
        return 0;
    }
    else
    {
        // printf("The File %s not Found\n", fileName);
        return 1;
    }
}
```
**Penjelasan:**
- Fungsi ```valid1``` untuk mengecek apakah inputan jam valid atau tidak
- Fungsi ```valid2``` untuk mengecek apakah inputan menit valid atau tidak
- Fungsi ```valid3``` untuk mengecek apakah inputan detik valid atau tidak
- Fungsi ```valid4``` untuk mengecek apakah file yang akan di download ada atau tidak

```bash
int main(int argc, char *argv[])
{

    if (argc != 5)
    {
        printf("eror1\n");
        return 1;
    }
    if (valid1(argv[1]))
    {
        printf("eror2\n");
        return 1;
    };
    if (valid2(argv[2]))
    {
        printf("eror3\n");
        return 1;
    };
    if (valid3(argv[3]))
    {
        printf("eror4\n");
        return 1;
    };
    if (valid4(argv[4]))
    {
        printf("eror5\n");
        return 1;
    };
    
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int jam = atoi(argv[1]);
    int menit = atoi(argv[2]);
    int detik = atoi(argv[3]);
    FILE *filePointer = fopen("checker.hq", "r");
    char output[100];
    char a1[100], a2[100], a3[100];
    for (int i = 0; i < 3; i++)
    {
        if (i == 0)
        {
            fscanf(filePointer, "%s", output);
            strcpy(a1, output);
        }
        else if (i == 1)
        {
            fscanf(filePointer, "%s", output);
            strcpy(a2, output);
        }
        else if (i == 2)
        {
            fscanf(filePointer, "%s", output);
            strcpy(a3, output);
        }
    }
    if (strcmp(a1, argv[1]) != 0 || strcmp(a2, argv[2]) != 0 ||strcmp(a3, argv[3]) != 0)
    {
        printf("hanya di izinkan 1 config cron\n");
        return 0;
    }
    pid_t process_id = 0;
    pid_t sid = 0;
    // Create child process
    process_id = fork();
    // Indication of fork() failure
    if (process_id < 0)
    {
        printf("fork failed!\n");
        // Return failure in exit status
        exit(1);
    }
    // PARENT PROCESS. Need to kill it.
    if (process_id > 0)
    {
        // printf("process_id of child process %d \n", process_id);
        // return success in exit status
        exit(0);
    }
    // unmask the file mode
    umask(0);
    // set new session
    sid = setsid();
    if (sid < 0)
    {
        // Return failure
        exit(1);
    }
    // Change the current working directory to root.
    chdir("/home/rosy/coba");
    // Close stdin. stdout and stderr
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while (1)
    {
        pid_t child_id = fork();
        if (child_id < 0)
        {
            exit(EXIT_FAILURE);
        }
        else if (child_id == 0)
        {
            sleep(1);
            char *args[] = {argv[0], argv[1], argv[2], argv[3], argv[4], NULL};
            execv(argv[0], args);
        }
        else
        {
            if (argv[1][0] == '*' && argv[2][0] == '*' && argv[3][0] == '*') //! * * *
            {
                execl(argv[4], argv[4], NULL);
            }
            else if (argv[1][0] == '*' && argv[2][0] == '*' && argv[3][0] != '*') //! * * x
            {
                if (tm.tm_sec == detik)
                {
                    execl(argv[4], argv[4], NULL);
                }
                else
                {
                    exit(EXIT_SUCCESS);
                }
            }
            else if (argv[1][0] == '*' && argv[2][0] != '*' && argv[3][0] == '*') //! * x *
            {
                if (tm.tm_min == menit)
                {
                    execl(argv[4], argv[4], NULL);
                }
                else
                {
                    exit(EXIT_SUCCESS);
                }
            }
            else if (argv[1][0] == '*' && argv[2][0] != '*' && argv[3][0] != '*') //! * x x
            {
                if (tm.tm_sec == detik && tm.tm_min == menit)
                {
                    execl(argv[4], argv[4], NULL);
                }
                else
                {
                    exit(EXIT_SUCCESS);
                }
            }
            else if (argv[1][0] != '*' && argv[2][0] == '*' && argv[3][0] == '*') //! x * *
            {
                if (tm.tm_hour == jam)
                {
                    execl(argv[4], argv[4], NULL);
                }
                else
                {
                    exit(EXIT_SUCCESS);
                }
            }
            else if (argv[1][0] != '*' && argv[2][0] == '*' && argv[3][0] != '*') //! x * x
            {
                if (tm.tm_sec == detik && tm.tm_hour == jam)
                {
                    execl(argv[4], argv[4], NULL);
                }
                else
                {
                    exit(EXIT_SUCCESS);
                }
            }
            else if (argv[1][0] != '*' && argv[2][0] != '*' && argv[3][0] == '*') //! x x *
            {
                if (tm.tm_min == menit && tm.tm_hour == jam)
                {
                    execl(argv[4], argv[4], NULL);
                }
                else
                {
                    exit(EXIT_SUCCESS);
                }
            }
            else if (argv[1][0] != '*' && argv[2][0] != '*' && argv[3][0] != '*') //! x x x
            {
                if (tm.tm_sec == detik && tm.tm_min == menit && tm.tm_hour == jam)
                {
                    execl(argv[4], argv[4], NULL);
                }
                else
                {
                    exit(EXIT_SUCCESS);
                }
            }
            else
            {
                exit(EXIT_SUCCESS);
            }


        }
        //  Implement and call some function that does core work for this daemon.
    }
    return (0);
}
```
**penjelasan**
- Pada fungsi ```main``` pertama tama terdapat pengecekan apakah argumen yang diinputkan sudah benar atau belum, jika belum maka akan menampilkan pesan error dan program akan berhenti.
- lalu terdapat ```checker``` untuk mengecek apakah crontab berjalan dengan config ganda atau tidak, jika config ganda maka akan menampilkan pesan error dan program akan berhenti.
- lalu mengunakan ```time.h``` untuk mendapatkan waktu saat ini.
- lalu mengunakan tampalan ```daemon``` untuk menjalankan program sebagai daemon.
- lalu terdapat ```while(1)``` untuk menjalankan program secara berulang-ulang.
- lalu terdapat ```fork()``` untuk membuat child process.
- lalu terdapat ```sleep(1)``` untuk mengevaluasi setiap jeda 1 detik.
- lau pada child process terdapat ```execv``` untuk menjalankan program  C yang diinputkan.
- lalu pada parent proses terdapat ```execl``` untuk menjalankan program bash sesuai argumen yang dimasukkan.


## Dokumentasi
### Run eror dan berhasil
![Screenshot 2023-03-25 140109](https://user-images.githubusercontent.com/86828535/227701786-d0a45498-fd65-4bf3-96b9-dff9452bf7ee.png)
### CPU state minimum dan bukti hasil
![Screenshot 2023-03-25 140154](https://user-images.githubusercontent.com/86828535/227701782-d7dfda50-0e1f-4f26-a893-58cf2972ac08.png)
### Script .sh untuk membuktikan cron berjalan
![Screenshot 2023-03-25 140530](https://user-images.githubusercontent.com/86828535/227701954-2e65f41d-c4ff-47cc-adae-815913d96f22.png)

## Kendala
Dalam penyelesaian soal ini, kami mengalami kendala dalam pembuatan daemon untuk mendapatkan CPU state minimum
