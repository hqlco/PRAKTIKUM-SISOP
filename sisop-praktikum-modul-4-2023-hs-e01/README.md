# sisop-praktikum-modul-4-2023-HS-E01

| Nama                      | NRP           |Username           |
|---------------------------|---------------|--------------|
|Moh. Rosy Haqqy Aminy       |5025211012     |rosyhaqqy          |
|Rule Lulu Damara           |5025211050     |rulelulu           |
|Rizky Alifiyah Rahma       |5025211209     |ikikk              |

## Soal 1
Kota Manchester sedang dilanda berita bahwa kota ini mau juara (yang kota ‘loh ya, bukan yang satunya). Tagline #YBBA (#YangBiruBiruAja #anjaykelaspepnihbossenggoldong 💪🤖🤙🔵⚪) sudah mewabah di seluruh dunia. Semua warga pun sudah menyiapkan pesta besar-besaran. 

Seorang pelatih sepak bola handal bernama Peb merupakan pelatih klub Manchester Blue, sedang berjuang memenangkan Treble Winner. Untuk meraihnya, ia perlu melakukan pembelian pemain dengan ideal. Agar sukses, ia memahami setiap detail data performa pemain sepak bola seluruh dunia yang meliputi statistik pemain, umur, tinggi dan berat badan, potensi, klub dan negaranya, serta banyak data lainnya. Namun, tantangan tersendiri muncul ketika mengelola dan mengakses data berukuran besar ini.

Kesulitan Peb tersebut mencapai telinga kalian, seorang mahasiswa Teknik Informatika yang ahli dalam pengolahan data. Mengetahui tantangan Peb, kalian diminta untuk membantu menyelesaikan masalahnya melalui beberapa langkah berikut.

a. Langkah pertama, adalah memperoleh data yang akan digunakan. Kalian membuat file bernama storage.c. Oleh karena itu, download dataset tentang pemain sepak bola dari Kaggle. Dataset ini berisi informasi tentang pemain sepak bola di seluruh dunia, termasuk Manchester Blue. Kalian tahu bahwa dataset ini akan sangat berguna bagi Peb. Gunakan command ini untuk men-download dataset.

kaggle datasets download -d bryanb/fifa-player-stats-database

Setelah berhasil men-download dalam format .zip, langkah selanjutnya adalah mengekstrak file tersebut.  Kalian melakukannya di dalam file storage.c untuk semua pengerjaannya. 

b. Selanjutnya, Peb meminta kalian untuk melakukan analisis awal pada data tersebut dan mencari pemain berpotensi tinggi untuk direkrut. Oleh karena itu, kalian perlu membaca file CSV khusus bernama FIFA23_official_data.csv dan mencetak data pemain yang berusia di bawah 25 tahun, memiliki potensi di atas 85, dan bermain di klub lain selain Manchester City. Informasi yang dicetak mencakup nama pemain, klub tempat mereka bermain, umur, potensi, URL foto mereka, dan data lainnya. kalian melakukannya di dalam file storage.c untuk analisis ini.

c. Peb menyadari bahwa sistem kalian sangat berguna dan ingin sistem ini bisa diakses oleh asisten pelatih lainnya. Oleh karena itu, kalian perlu menjadikan sistem yang dibuat ke sebuah Docker Container agar mudah di-distribute dan dijalankan di lingkungan lain tanpa perlu setup environment dari awal. Buatlah Dockerfile yang berisi semua langkah yang diperlukan untuk setup environment dan menentukan bagaimana aplikasi harus dijalankan.

Setelah Dockerfile berhasil dibuat, langkah selanjutnya adalah membuat Docker Image. Gunakan Docker CLI untuk mem-build image dari Dockerfile kalian. Setelah berhasil membuat image, verifikasi bahwa image tersebut berfungsi seperti yang diharapkan dengan menjalankan Docker Container dan memeriksa output-nya.

d. Setelah sukses membuat sistem berbasis Docker, Peb merasa bahwa sistem ini tidak hanya berguna untuk dirinya sendiri, tetapi juga akan akan membantu para scouting-nya yang terpencar di seluruh dunia dalam merekrut pemain berpotensi tinggi. Namun, satu tantangan muncul, yaitu bagaimana caranya para scout dapat mengakses dan menggunakan sistem yang telah diciptakan?

Merasa terpanggil untuk membantu Peb lebih jauh, kalian memutuskan untuk mem-publish Docker Image sistem ke Docker Hub, sebuah layanan cloud yang memungkinkan kalian untuk membagikan aplikasi Docker kalian ke seluruh dunia. Output dari pekerjaan ini adalah file Docker kalian bisa dilihat secara public pada https://hub.docker.com/r/{Username}/storage-app.

e. Berita tagline #YBBA (#YangBiruBiruAja) semakin populer, dan begitu juga sistem yang telah kalian buat untuk membantu Peb dalam rekrutmen pemain. Beberapa klub sepak bola lain mulai menunjukkan minat pada sistem tersebut dan permintaan penggunaan semakin bertambah. Untuk memastikan sistem kalian mampu menangani lonjakan permintaan ini, kalian memutuskan untuk menerapkan skala pada layanan menggunakan Docker Compose dengan instance sebanyak 5. Buat folder terpisah bernama Barcelona dan Napoli dan jalankan Docker Compose di sana.


    Catatan: 
    - Cantumkan file hubdocker.txt yang berisi URL Docker Hub kalian (public).
    - Perhatikan port  pada masing-masing instance.

## Jawaban

### 1.a
cukup menggunakan ```system()``` untuk mendownload dan unzip.

```bash
    if (!fileExists("fifa-player-stats-database.zip"))
    {
        system("kaggle datasets download -d bryanb/fifa-player-stats-database");
    }
    if (!fileExists("FIFA23_official_data.csv"))
    {
        system("unzip fifa-player-stats-database");
    }

```

### 1.b
cukup menggunakan ```awk``` untuk mencari baris baris bersyarat seperti keterangan soal
```bash
    system("awk -F',' 'BEGIN {OFS=\",\"; count=1} $3 < 25 && $8 > 85 && $9 != \"Manchester City\" { print count++, $2\"\\n\" $3\"\\n\" $9\"\\n\" $5\"\\n\" $8\"\\n\" $4\"\\n\"}' FIFA23_official_data.csv");

```
### penjelasan 1.b
```BEGIN {OFS=\",\"; count=1}``` digunakan untuk nomer akan di inciment setiap baris.

```$3 < 25 && $8 > 85 && $9 != \"Manchester City\"``` syarat syarat sesuai ketentuan soal.

```{ print count++, $2\"\\n\" $3\"\\n\" $9\"\\n\" $5\"\\n\" $8\"\\n\" $4\"\\n\"}'``` print sesuai ketentuan soal.

### 1.c
kita dapat melakukan apa yang telah di set up dilcoal linux untuk selanjutnya diterapkan didocker file seperti memasukkan API key, mesakukkan storage.c ke dockerfile dan set up ```pip install kaggle```

``` bash

FROM ubuntu:latest

# Install system dependencies
RUN apt-get update && apt-get install -y \
    python3 \
    python3-pip \
    build-essential \
    wget unzip

# Set the working directory
WORKDIR /root

# Install required dependencies
RUN pip install kaggle


# Set up Kaggle API credentials
RUN mkdir /root/.kaggle
COPY kaggle.json /root/.kaggle/kaggle.json
RUN mkdir /root/.kg
# RUN mkdir /root/.local/bin
RUN touch /root/.kg/kaggle

RUN echo '#!/usr/bin/python3' >> /root/.kg/kaggle && \
    echo '# -*- coding: utf-8 -*-' >> /root/.kg/kaggle && \
    echo 'import re' >> /root/.kg/kaggle && \
    echo 'import sys' >> /root/.kg/kaggle && \
    echo 'from kaggle.cli import main' >> /root/.kg/kaggle && \
    echo 'if __name__ == "__main__":' >> /root/.kg/kaggle && \
    echo '    sys.argv[0] = re.sub(r"(-script\.pyw|\.exe)?$", "", sys.argv[0])' >> /root/.kg/kaggle && \
    echo '    sys.exit(main())' >> /root/.kg/kaggle

RUN echo 'alias kaggle="~/.kg/kaggle"' >> /root/.bashrc

# Copy the C file
COPY storage.c /root/storage.c

# Download the Kaggle dataset and unzip and show data
RUN gcc storage.c -o storage 

# Set the command to be executed when the Docker container starts
CMD ["./storage"]

```
### 1.d
kita dapat push ke docker hub dengan cara membuat akun terlebih dahulu setelah itu dapat push dengan command 
``` bash
docker tag local-image:tagname new-repo:tagname
docker push new-repo:tagname
```
berikut link repo saya
```bash
https://hub.docker.com/r/leolco/storage-app
```


### 1.e
kita dapat menggunakan docker compose dan menggunakan replicas untuk membuat 5 container

```bash
version: '3'
services:
  barcelona:
   # container_name: barcelona
    image: leolco/storage-app
    command: tail -f /dev/null
    deploy:
      replicas: 5
  napoli:
    #container_name: napoli
    image: leolco/storage-app
    command: tail -f /dev/null
    deploy:
      replicas: 5
```




## Dokumentasi Soal 1

### run local
![Screenshot 2023-06-01 100520](https://github.com/hqlco/sampah/assets/86828535/b0fd8419-6be2-4bb4-ab15-04723a229782)

### run with docker
![Screenshot 2023-06-01 100708](https://github.com/hqlco/sampah/assets/86828535/870e2141-0761-43e5-8aac-7d5c236fb516)

### run with repo docker hub
![Screenshot 2023-06-01 100800](https://github.com/hqlco/sampah/assets/86828535/422286c1-d968-4109-ae82-4378b919b9d6)

### run docker compose napoli
![Screenshot 2023-06-01 103620](https://github.com/hqlco/sampah/assets/86828535/93bd9607-c367-445b-972c-6fe68c0f5c40)

### run docker compose barcelona
![Screenshot 2023-06-01 103511](https://github.com/hqlco/sampah/assets/86828535/0deaac1d-98a2-476e-b058-e4bd90080782)

## Soal 4
Pada suatu masa, terdapat sebuah perusahaan bernama Bank Sabar Indonesia yang berada pada masa kejayaannya. Bank tersebut memiliki banyak sekali kegiatan-kegiatan yang  krusial, seperti mengolah data nasabah yang mana berhubungan dengan uang. Suatu ketika, karena susahnya maintenance, petinggi bank ini menyewa seseorang yang mampu mengorganisir file-file yang ada di Bank Sabar Indonesia. 
Salah satu karyawan di bank tersebut merekomendasikan Bagong sebagai seseorang yang mampu menyelesaikan permasalahan tersebut. Bagong memikirkan cara terbaik untuk mengorganisir data-data nasabah dengan cara membagi file-file yang ada dalam bentuk modular, yaitu membagi file yang mulanya berukuran besar menjadi file-file chunk yang berukuran kecil. Hal ini bertujuan agar saat terjadi error, Bagong dapat mudah mendeteksinya. Selain dari itu, agar Bagong mengetahui setiap kegiatan yang ada di filesystem, Bagong membuat sebuah sistem log untuk mempermudah monitoring kegiatan di filesystem yang mana, nantinya setiap kegiatan yang terjadi akan dicatat di sebuah file log dengan ketentuan sebagai berikut. 
- Log akan dibagi menjadi beberapa level, yaitu REPORT dan FLAG.
- Pada level log FLAG, log akan mencatat setiap kali terjadi system call rmdir (untuk menghapus direktori) dan unlink (untuk menghapus file). Sedangkan untuk sisa operasi lainnya, akan dicatat dengan level log REPORT.
Format untuk logging yaitu sebagai berikut.
[LEVEL]::[yy][mm][dd]-[HH]:[MM]:[SS]::[CMD]::[DESC ...]<br> Contoh:
REPORT::230419-12:29:28::RENAME::/bsi23/bagong.jpg::/bsi23/bagong.jpeg <br>
REPORT::230419-12:29:33::CREATE::/bsi23/bagong.jpg <br>
FLAG::230419-12:29:33::RMDIR::/bsi23 <br>
<br>
Tidak hanya itu, Bagong juga berpikir tentang beberapa hal yang berkaitan dengan ide modularisasinya sebagai berikut yang ditulis dalam modular.c.

- Pada filesystem tersebut, jika Bagong membuat atau me-rename sebuah direktori dengan awalan module_, maka direktori tersebut akan menjadi direktori modular. 
- Bagong menginginkan agar saat melakukan modularisasi pada suatu direktori, maka modularisasi tersebut juga berlaku untuk konten direktori lain di dalam direktori (subdirektori) tersebut.
- Sebuah file nantinya akan terbentuk bernama fs_module.log pada direktori home pengguna (/home/[user]/fs_module.log) yang berguna menyimpan daftar perintah system call yang telah dijalankan dengan sesuai dengan ketentuan yang telah disebutkan sebelumnya.
- Saat Bagong melakukan modularisasi, file yang sebelumnya ada pada direktori asli akan menjadi file-file kecil sebesar 1024 bytes dan menjadi normal ketika diakses melalui filesystem yang dirancang oleh dia sendiri.<br>
Contoh:
file File_Bagong.txt berukuran 3 kB pada direktori asli akan menjadi 3 file kecil, yakni File_Bagong.txt.000, File_Bagong.txt.001, dan File_Bagong.txt.002 yang masing-masing berukuran 1 kB (1 kiloBytes atau 1024 bytes).
- Apabila sebuah direktori modular di-rename menjadi tidak modular, maka isi atau konten direktori tersebut akan menjadi utuh kembali (fixed).

## Jawaban
Untuk beberapa poin pada soal nomor 4 saling mendukung satu sama lain, sehingga saya akan memberi penjelasan berdasarkan kode yang saya buat
### Deklarasi
```bash
static const char *log_file_path = "/home/ikikk/       fs_module.log";
 ```
```log_file_path``` akan menyimpan path dari file log

```bash
static const char *dirpath = "/home/ikikk/sisop-praktikum-modul-4-2023-hs-e01/soal4.1/haloges";
```
```dirpath``` akan menyimpan path root dari filesystem yang akan dibuat

### Struct
```bash
struct log_info {
    const char *level;
    const char *cmd;
    const char *desc;
}
```
struct ```log_info``` digunakan untuk menyimpan informasi log seperti level, perintah (command), dan deskripsi

### Fungsi

1. ```void log_command```
    ```bash
    void log_command(const struct log_info *info) {
        FILE *file = fopen(log_file_path, "a");
        if (file) {
            struct timeval tv;
            gettimeofday(&tv, NULL);
            struct tm *tm_info = localtime(&tv.tv_sec);
            
            fprintf(file, "%s::%02d%02d%02d-%02d:%02d:%02d::%s::%s\n", info->level,
                    tm_info->tm_year % 100, tm_info->tm_mon + 1, tm_info->tm_mday,
                    tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec,
                    info->cmd, info->desc);
            fclose(file);
        }
    }
    ```
    Fungsi ```log_command``` digunakan untuk mencatat log ke dalam file. Log akan dicetak dalam format yang ditentukan menggunakan fungsi fprintf. Log akan mencakup informasi waktu dan juga informasi level, perintah (command), dan deskripsi yang diberikan dalam struktur log_info.

2. ```void split```
    ```bash
    void split(char *path){
        DIR *dir = opendir(path);
        struct dirent *ent;
        struct stat sb;

        while ((ent = readdir(dir)) != NULL){
            if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..")) continue;

            char curr[300];
            sprintf(curr, "%s/%s", path, ent->d_name);
            if (stat(curr, &sb) == -1) 
                continue;

            if (S_ISDIR(sb.st_mode)){
                split(curr);
            }
            else if (S_ISREG(sb.st_mode)){       
                FILE *input = fopen(curr, "rb");
                if (input == NULL) return;

                fseek(input, 0, SEEK_END);

                long fileSize = ftell(input);

                if (fileSize <= sz) continue;

                rewind(input);

                int count = (fileSize + sz - 1) / sz;

                for (int i = 0; i < count; i++){
                    char num[5], tempPath[350];

                    if (i < 10) {
                        sprintf(num, "00%d", i);
                    }
                    else if (i < 100) {
                        sprintf(num, "0%d", i);
                    }
                    else {
                        sprintf(num, "%d", i);
                    }

                    sprintf(tempPath, "%s.%s", curr, num);

                    FILE *output = fopen(tempPath, "wb");
                    if (output == NULL) return;
                    
                    char *buffer = (char*) malloc(sz);
                    if (buffer == NULL) return;

                    size_t bytes = fread(buffer, 1, sz, input);

                    fwrite(buffer, 1, bytes, output);

                    free(buffer);
                    fclose(output);
                }
                fclose(input);

                char cmd[700];
                sprintf(cmd, "rm %s ", curr);
                system(cmd);
            }
        }
        closedir(dir);
    }
    ```
    Fungsi split yang Anda berikan adalah sebuah rekursi yang bertujuan untuk membagi file menjadi beberapa bagian dengan ukuran tertentu dan menghapus file asli setelah pembagian selesai.

2. ```void merge```
    ```bash
    void merge(char *path){
        DIR *dir = opendir(path);
        struct dirent *ent;
        struct stat sb;

        while ((ent = readdir(dir)) != NULL){
            if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..")) continue;

            char curr[300];
            sprintf(curr, "%s/%s", path, ent->d_name);
            if (stat(curr, &sb) == -1) continue;

            if (S_ISDIR(sb.st_mode)){
                merge(curr);
            }
            else if (S_ISREG(sb.st_mode) && strlen(curr) > 3 && !strcmp(curr + strlen(curr) - 3, "000")){         
                int count = 0;
                char dest[300], temp[350];
                memset(dest, '\0', sizeof(dest));
                memset(temp, '\0', sizeof(temp));

                strncpy(dest, curr, strlen(curr) - 4);
                while(true){
                    if (count < 10) {
                        sprintf(temp, "%s.00%d", dest, count);
                    }
                    else if (count < 100) {
                        sprintf(temp, "%s.0%d", dest, count);
                    }
                    else {
                        sprintf(temp, "%s.%d", dest, count);
                    }

                    int res = stat(temp, &sb);
                    if (res) break;

                    char cmd[700];
                    sprintf(cmd, "cat %s >> %s", temp, dest);
                    system(cmd);

                    sprintf(cmd, "rm %s", temp);
                    system(cmd);
                    count++;
                }
            }
        }
        closedir(dir);
    }
    ```
    Fungsi merge yang Anda berikan adalah sebuah rekursi yang bertujuan untuk menggabungkan file-file dengan ekstensi "000" menjadi satu file utuh dengan menghapus file-file tersebut setelah digabungkan.

3. Fungsi-fungsi yang digunakan untuk Fuse
    ```bash
    static int xmp_getattr(const char *path, struct stat *stbuf){
        char fpath[1000];
        sprintf(fpath, "%s%s", dirpath, path);

        int res = lstat(fpath, stbuf);
        
        if (res == -1) {
            return -errno;
        }

        log_command(&(struct log_info){ "REPORT", "GETATTR", path});
        return 0;
    }

    static int xmp_access(const char *path, int mode){
        char fpath[1000];
        sprintf(fpath, "%s%s", dirpath, path);

        int res = access(fpath, mode);

        if (res == -1) {
            return -errno;
        }

        log_command(&(struct log_info){ "REPORT", "ACCESS", path });

        if (strstr(path, "/module_")){
            merge(fpath);
            strcpy(oldpath, fpath);
        }
        else if (strlen(oldpath)){
            split(oldpath);
            memset(oldpath, '\0', sizeof(oldpath));
        }

        return 0;
    }

    static  int  xmp_readlink(const char *path, char *buf, size_t size){
        char fpath[1000];
        sprintf(fpath, "%s%s", dirpath, path);

        int res = readlink(fpath, buf, size - 1);
        
        if (res == -1) {
            return -errno;
        }

        log_command(&(struct log_info){ "REPORT", "READLINK", path });
        return 0;
    }

    static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi){
        char fpath[1000];
        sprintf(fpath, "%s%s", dirpath, path);

        DIR *dp;
        struct dirent *de;
        (void) offset;
        (void) fi;

        dp = opendir(fpath);
        if (dp == NULL) {
            return -errno;
        }

        while ((de = readdir(dp)) != NULL) {
            struct stat st;

            memset(&st, 0, sizeof(st));

            st.st_ino = de->d_ino;
            st.st_mode = de->d_type << 12;

            if ((filler(buf, de->d_name, &st, 0)) != 0) break;
        }
        closedir(dp);

        log_command(&(struct log_info){ "REPORT", "READDIR", path });
        return 0;
    }

    static int xmp_mknod(const char *path, mode_t mode, dev_t dev){
        char fpath[1000];
        sprintf(fpath, "%s%s", dirpath, path);

        int res = mknod(fpath, mode, dev);
        
        if (res == -1) {
            return -errno;
        }
        
        log_command(&(struct log_info){ "REPORT", "MKNOD", path });
        return 0;
    }

    static int xmp_mkdir(const char *path, mode_t mode){
        char fpath[1000];
        sprintf(fpath, "%s%s", dirpath, path);

        int res = mkdir(fpath, mode);

        if (res == -1) {
            return -errno;
        }
        
        log_command(&(struct log_info){ "REPORT", "MKDIR", path });
        
        return 0;
    }

    static int xmp_symlink(const char *to, const char *from){
        char dest_path[1000], src_path[1000];
        sprintf(dest_path, "%s%s", dirpath, to);
        sprintf(src_path, "%s%s", dirpath, from);

        int res = symlink(dest_path, src_path);
        
        if (res == -1) {
            return -errno;
        }
        
        log_command(&(struct log_info){ "REPORT", "SYMLINK", dest_path });
        
        return 0;
    }

    static int xmp_unlink(const char *path){
        char fpath[1000];
        sprintf(fpath, "%s%s", dirpath, path);

        int res = unlink(fpath);
        
        if (res == -1) {
            return -errno;
        }

        log_command(&(struct log_info){ "FLAG", "UNLINK", path });
        return 0;
    }

    static int xmp_rmdir(const char *path){
        char fpath[1000];
        sprintf(fpath, "%s%s", dirpath, path);

        int res = rmdir(fpath);

        if (res == -1) {
            return -errno;
        }
        
        log_command(&(struct log_info){ "FLAG", "RMDIR", path });
        return 0;
    }

    static int xmp_rename(const char *from, const char *to){
        char src_path[1000], dest_path[1000];
        sprintf(src_path, "%s%s", dirpath, from);
        sprintf(dest_path, "%s%s", dirpath, to);

        int res = rename(src_path, dest_path);

        if (res == -1) {
            return -errno;
        }
        
        log_command(&(struct log_info){ "REPORT", "RENAME", src_path });

        char curr[1000], dup[1000];
        strcpy(curr, dirpath);
        strcpy(dup, to);
        char *token, *old = strrchr(from, '/'), *new = strrchr(to, '/');
        token = strtok(dup, "/");
        
        while(token != NULL) {
            strcat(curr, "/");
            strcat(curr, token);

            struct stat sb;

            if (stat(curr, &sb) == -1) {
                continue;
            }

            if (strlen(token) >= 7 && !strncmp(token, "module_", 7) && S_ISDIR(sb.st_mode)) {
                split(curr);
                break;
            }

            token = strtok(NULL, "/");
        }

        if (strcmp(old, new)) {
            struct stat sb;
            if ((stat(dest_path, &sb) != -1) && S_ISDIR(sb.st_mode) && (strlen(new) < 8 || strncmp(new, "/module_", 8))) merge(dest_path);
        }

        return 0;
    }

    static int xmp_link(const char *from, const char *to){
        char src_path[1000], dest_path[1000];
        sprintf(src_path, "%s%s", dirpath, from);
        sprintf(dest_path, "%s%s", dirpath, to);

        int res = link(src_path, dest_path);

        if (res == -1) {
            return -errno;
        }

        log_command(&(struct log_info){ "REPORT", "LINK", src_path });
        return 0;
    }

    static int xmp_chmod(const char *path, mode_t mode){
        char fpath[1000];
        sprintf(fpath, "%s%s", dirpath, path);

        int res = chmod(fpath, mode);

        if (res == -1) {
            return -errno;
        }
        
        log_command(&(struct log_info){ "REPORT", "CHMOD", path });
        return 0;
    }

    static int xmp_chown(const char *path, uid_t uid, gid_t gid){
        char fpath[1000];
        sprintf(fpath, "%s%s", dirpath, path);

        int res = chown(fpath, uid, gid);

        if (res == -1) {
            return -errno;
        }
        
        log_command(&(struct log_info){ "REPORT", "CHOWN", path });
        return 0;
    }

    static int xmp_truncate(const char *path, off_t size){
        char fpath[1000];
        sprintf(fpath, "%s%s", dirpath, path);

        int res = truncate(fpath, size);

        if (res == -1) {
            return -errno;
        }
        
        log_command(&(struct log_info){ "REPORT", "TRUNCATE", path });
        return 0;
    }

    static int xmp_utimens(const char *path, const struct timespec ts[2]){
        char fpath[1000];
        sprintf(fpath, "%s%s", dirpath, path);

        struct timeval tv[2];
        tv[0].tv_sec = ts[0].tv_sec;
        tv[0].tv_usec = ts[0].tv_nsec / 1000;
        tv[1].tv_sec = ts[1].tv_sec;
        tv[1].tv_usec = ts[1].tv_nsec / 1000;

        int res = utimes(fpath, tv);

        if (res == -1) {
            return -errno;
        }
        
        log_command(&(struct log_info){ "REPORT", "UTIMENS", path });
        return 0;
    }

    static int xmp_open(const char *path, struct fuse_file_info *fi){
        char fpath[1000];
        sprintf(fpath, "%s%s", dirpath, path);

        int res = open(fpath, fi->flags);

        if (res == -1) {
            return -errno;
        }
        
        close(res);

        log_command(&(struct log_info){ "REPORT", "OPEN", path });
        return 0;
    }

    static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
        char fpath[1000];
        sprintf(fpath, "%s%s", dirpath, path);

        int res = 0;
        int fd = 0;
        (void) fi;

        fd = open(fpath, O_RDONLY);
        if (fd == -1) {
            return -errno;
        }

        res = pread(fd, buf, size, offset);
        if (res == -1) {
            return -errno;
        }

        close(fd);

        log_command(&(struct log_info){ "REPORT", "READ", path });
        return res;
    }

    static int xmp_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
        char fpath[1000];
        sprintf(fpath, "%s%s", dirpath, path);

        int fd;
        int res;
        (void) fi;

        fd = open(fpath, O_WRONLY);
        if (fd == -1) {
            return -errno;
        }

        res = pwrite(fd, buf, size, offset);
        if (res == -1) {
            return -errno;
        }

        close(fd);

        log_command(&(struct log_info){ "REPORT", "WRITE", path });
        return res;
    }

    static int xmp_statfs(const char *path, struct statvfs *stbuf){
        char fpath[1000];
        sprintf(fpath, "%s%s", dirpath, path);

        int res = statvfs(fpath, stbuf);

        if (res == -1) {
            return -errno;
        }
        
        log_command(&(struct log_info){ "REPORT", "STATFS", path });
        return 0;
    }

    static int xmp_create(const char *path, mode_t mode, struct fuse_file_info *fi){
        char fpath[1000];
        sprintf(fpath, "%s%s", dirpath, path);

        int res = open(fpath, fi->flags, mode);

        if (res == -1) {
            return -errno;
        }
        
        log_command(&(struct log_info){ "REPORT", "CREATE", path });
        return res;
    }

    static int xmp_release(const char *path, struct fuse_file_info *fi){
        char fpath[1000];
        sprintf(fpath, "%s%s", dirpath, path);

        (void) fpath;
        (void) fi;

        log_command(&(struct log_info){ "REPORT", "RELEASE", path });
        return 0;
    }

    static int xmp_fsync(const char *path, int isdatasync, struct fuse_file_info *fi){
        char fpath[1000];
        sprintf(fpath, "%s%s", dirpath, path);

        (void) fpath;
        (void) isdatasync;
        (void) fi;

        log_command(&(struct log_info){ "REPORT", "FSYNC", path });
        return 0;
    }

    static int xmp_setxattr(const char *path, const char *name, const char *value, size_t size, int flags){
        char fpath[1000];
        sprintf(fpath, "%s%s", dirpath, path);

        int res = lsetxattr(fpath, name, value, size, flags);

        if (res == -1) {
            return -errno;
        }
        
        log_command(&(struct log_info){ "REPORT", "SETXATTR", path });
        return 0;
    }

    static int xmp_getxattr(const char *path, const char *name, char *value,size_t size){
        char fpath[1000];
        sprintf(fpath, "%s%s", dirpath, path);

        int res = lgetxattr(fpath, name, value, size);

        if (res == -1) {
            return -errno;
        }
        
        log_command(&(struct log_info){ "REPORT", "GETXATTR", path });
        return res;
    }

    static int xmp_listxattr(const char *path, char *list, size_t size){
        char fpath[1000];
        sprintf(fpath, "%s%s", dirpath, path);

        int res = llistxattr(fpath, list, size);

        if (res == -1) {
            return -errno;
        }
        
        log_command(&(struct log_info){ "REPORT", "LISTXATTR", path });
        return res;
    }

    static int xmp_removexattr(const char *path, const char *name){
        char fpath[1000];
        sprintf(fpath, "%s%s", dirpath, path);

        int res = lremovexattr(fpath, name);

        if (res == -1) {
            return -errno;
        }
        
        log_command(&(struct log_info){ "REPORT", "REMOVEXATTR", path });
        return 0;
    }

    static struct fuse_operations xmp_oper = {
        .getattr = xmp_getattr,
        .access = xmp_access,
        .readlink = xmp_readlink,
        .readdir = xmp_readdir,
        .mknod = xmp_mknod,
        .mkdir = xmp_mkdir,
        .symlink = xmp_symlink,
        .unlink = xmp_unlink,
        .rmdir = xmp_rmdir,
        .rename = xmp_rename,
        .link = xmp_link,
        .chmod = xmp_chmod,
        .chown = xmp_chown,
        .truncate = xmp_truncate,
        .utimens = xmp_utimens,
        .open = xmp_open,
        .read = xmp_read,
        .write = xmp_write,
        .statfs = xmp_statfs,
        .create = xmp_create,
        .release = xmp_release,
        .fsync = xmp_fsync,
        .setxattr = xmp_setxattr,
        .getxattr = xmp_getxattr,
        .listxattr = xmp_listxattr,
        .removexattr = xmp_removexattr,
    };

    int main(int  argc, char *argv[]){
        // inisialisasi log file dan modular prefix
        const char *user = getenv("USER");
        char log_file[256];
        sprintf(log_file, "/home/%s/fs_module.log", user);
        log_file_path = log_file;

        // inisialisasi FUSE
        umask(0);
        int ret = fuse_main(argc, argv, &xmp_oper, NULL);

        // ...

        return ret;
    }
    ```
    Berikut beberapa fungsi yang diperlukan dalam suatu filesystem
    - xmp_getattr: Mengambil atribut file, seperti metadata, ukuran, dan permission.
    - xmp_access: Mengecek akses ke file, seperti read, write, atau execute.
    - xmp_readlink: Membaca isi dari sebuah symlink.
    - xmp_readdir: Membaca isi dari sebuah direktori.
    - xmp_mknod: Membuat sebuah node (file, device, atau symlink baru).
    - xmp_mkdir: Membuat sebuah direktori baru.
    - xmp_symlink: Membuat sebuah symlink.
    - xmp_unlink: Menghapus sebuah file.
    - xmp_rmdir: Menghapus sebuah direktori.
    - xmp_rename: Mengganti nama sebuah file atau direktori.
    - xmp_link: Membuat hard link.
    - xmp_chmod: Mengubah permission sebuah file atau direktori.
    - xmp_chown: Mengubah kepemilikan sebuah file atau direktori.
    - xmp_truncate: Mengubah ukuran sebuah file.
    - xmp_utimens: Mengubah timestamp sebuah file.
    - xmp_open: Membuka sebuah file.
    - xmp_read: Membaca isi dari sebuah file.
    - xmp_write: Menulis data ke dalam sebuah file.
    - xmp_statfs: Mengambil informasi statistik tentang sebuah sistem berkas.
    - xmp_create: Membuat sebuah file baru.
    - xmp_release: Menutup sebuah file.
    - xmp_fsync: Melakukan sinkronisasi data file dengan disk.
    - xmp_setxattr: Mengatur extended attribute sebuah file atau direktori.
    - xmp_getxattr: Mengambil extended attribute sebuah file atau direktori.
    - xmp_listxattr: Membuat daftar extended attribute dari sebuah file atau direktori.
    - xmp_removexattr: Menghapus extended attribute sebuah file atau direktori.

## Dokumentasi
https://github.com/Ikikk/jgndibuka/blob/main/Screenshot%202023-06-03%20211548.png

https://github.com/Ikikk/jgndibuka/blob/main/WhatsApp%20Image%202023-06-03%20at%2021.49.38.jpg


## Kendala
Pada saat praktikum, berhasil untuk mounting namun belum berhasil modularisasi. Pada saat revisi, sudah diberi pembenaran
1. Bingung dengan soalnya, karena kurang dijelaskan file nya kita buat sendiri atau sudah tersedia
2. Kendala pada saat mounting, beberapa kali terjadi error
3. Kendala pada saat membuat modularisasi, awalnya terjadi error yang menyebabkan folder fuse tidak bisa dibuka

## Soal 5
Setelah sukses menjadi pengusaha streaming musik di chapter kehidupan sebelumnya, Elshe direkrut oleh lembaga rahasia untuk membuat sistem rahasia yang terenkripsi.  Kalian perlu membantu Elshe dan membuat program rahasia.c. Pada program rahasia.c, terdapat beberapa hal yang harus kalian lakukan sebagai berikut.	

a. Program rahasia.c merupakan file FUSE yang akan digunakan untuk melakukan mount folder pada Docker Container. Unduh file rahasia.zip kemudian lakukan unzip pada file rahasia.zip menjadi folder rahasia.

b. Seperti soal a, folder rahasia akan di-mount pada Docker Container dengan image bernama rahasia_di_docker_<Kode Kelompok> pada direktori /usr/share. Gunakan Ubuntu Focal Fossa sebagai base image pada Dockerfile.
Setelah melakukan mount, buatlah register system yang menyimpan kredensial berupa username dan password. Agar lebih aman, password disimpan dengan menggunakan hashing MD5. Untuk mempermudah kalian, gunakan system()  serta gunakan built-in program untuk melakukan hashing MD5 pada Linux (tidak wajib). Username dan password akan disimpan dengan format <username>;<password>. Tidak boleh ada user yang melakukan register dengan username yang sama. Kemudian, 
c. Buatlah login system yang mencocokkan kredensial antara username dan password.
Contoh:
kyunkyun;fbc5ccdf7c34390d07b1f4b74958a9ce
Penjelasan:
kyunkyun adalah username.
fbc5ccdf7c34390d07b1f4b74958a9ce adalah MD5 Hashing dari Subarukun, user kyunkyun menginputkan Subarukun sebagai password. Untuk lebih jelas, kalian bisa membaca dokumentasinya pada https://www.md5hashgenerator.com/. 
Catatan: 
Tidak perlu ada password validation untuk mempermudah kalian.	

d. Folder tersebut hanya dapat diakses oleh user yang telah melakukan login. User yang login dapat membaca folder dan file yang di-mount kemudian dengan menggunakan FUSE lakukan rename pada folder menjadi <Nama_Folder>_<Kode_Kelompok> dan <Kode_Kelompok>_<Nama_File>.<ext>.
Contoh:
A01_a.pdf
xP4UcxRZE5_A01

e. List seluruh folder, subfolder, dan file yang telah di-rename dalam file result.txt menggunakan tree kemudian hitung file tersebut berdasarkan extension dan output-kan menjadi extension.txt.

Struktur file :
---soal5:
        ---rahasia.c
		---docker-compose.yml

## Jawaban

### 5.a
```bash
void DownloadFile(const char *url, const char *zipPath) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("Failed to fork\n");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        char *command = malloc(1024 * sizeof(char));
        snprintf(command, 1024, "wget --load-cookies /tmp/cookies.txt \"https://docs.google.com/uc?export=download&confirm=$(wget --quiet --save-cookies /tmp/cookies.txt --keep-session-cookies --no-check-certificate 'https://docs.google.com/uc?export=download&id=%s' -O- | sed -rn 's/.*confirm=([0-9A-Za-z_]+).*/\\1\\n/p')&id=%s\" -O %s && rm -rf /tmp/cookies.txt", url, url, zipPath);
        printf("Downloading file: %s\n", url);
        execl("/bin/sh", "sh", "-c", command, (char*)NULL);
        free(command);
    }
    else {
        int status;
        waitpid(pid, &status, 0);
    }
}
```
### Penjelasan 
Fungsi DownloadFile digunakan untuk mengunduh file dari URL tertentu dan menyimpannya dalam path yang ditentukan. Berikut adalah penjelasan singkat tentang 

Fungsi ini menggunakan `fork()` untuk membuat proses baru.
Jika `fork()` gagal, maka akan muncul pesan error dan program akan keluar dengan kode kesalahan.
Jika `fork()` mengembalikan nilai 0, maka ini adalah proses anak yang baru dibuat. Pada bagian ini, perintah wget dibuat dengan menggunakan URL yang diberikan. File akan diunduh menggunakan cookies untuk mengonfirmasi unduhan. File unduhan akan disimpan ke zipPath yang ditentukan. Setelah selesai, file cookies akan dihapus. Perintah tersebut dieksekusi menggunakan `execl()`.
Jika `fork()` mengembalikan nilai yang lebih besar dari 0, ini adalah proses induk. Pada bagian ini, proses induk menunggu proses anak selesai menggunakan waitpid().

```bash
void unzipping(const char *zipPath)
{

    pid_t pidUnzip = fork();
    if(pidUnzip == 0){
        printf("Unzipping file : %s\n", zipPath);
        execl("/bin/unzip", "unzip", zipPath, NULL);
        exit(0);
    }
    else if (pidUnzip < 0)
    {
        perror("Failed to fork");
        exit(EXIT_FAILURE);
    } 
    else 
    {
        int status;
        waitpid(pidUnzip, &status, 0);

    }
} 
```
### Penjelasan 
Fungsi ini menggunakan `fork()` untuk membuat proses baru.
Jika `fork()` mengembalikan nilai 0, maka ini adalah proses anak yang baru dibuat. Pada bagian ini, perintah unzip digunakan untuk mengekstrak file yang ada di zipPath. Perintah tersebut dieksekusi menggunakan `execl()`.
Jika `fork()` mengembalikan nilai yang lebih besar dari 0, ini adalah proses induk. Pada bagian ini, proses induk menunggu proses anak selesai menggunakan `waitpid()`.

### 5.b

### docker-compose.yml
```bash
version: '3'
services:
  myservice:
    image: rahasia_di_docker_e01
    container_name : modul4_myservice_1
    volumes:
      - ./rahasia:/usr/share/rahasia
    command: tail -f /dev/null
```

### Penjelasan 
Bagian services dalam file Docker Compose mendefinisikan layanan-layanan yang akan dijalankan.

`myservice`: Nama yang diberikan untuk layanan ini. Ini akan digunakan sebagai nama kontainer.
`image: rahasia_di_docker_e01`: Image bernama "rahasia_di_docker_e01" akan digunakan.
`container_name: modul4_myservice_1`: Menetapkan nama kontainer menjadi "modul4_myservice_1".
`volumes`: Direktori "./rahasia" di host akan ditempelkan ke direktori "/usr/share/rahasia" di dalam kontainer.
`command: tail -f /dev/null`: Menjalankan perintah "tail -f /dev/null" di dalam kontainer. Perintah ini tidak melakukan apa pun, tetapi menjaga kontainer tetap berjalan dan mencegahnya dari berhenti.

### Dockerfile
```bash
FROM ubuntu:focal

RUN apt-get update && apt-get install -y 

WORKDIR /usr/share

COPY credentials.txt credentials.txt

COPY rahasia.c /usr/share

CMD ["/usr/share/rahasia.c"]
```
### Penjelasan 
Dockerfile di atas digunakan untuk membuat image Docker yang berbasis Ubuntu Focal dan melakukan beberapa konfigurasi. Image ini akan menjalankan file rahasia.c dan memasukkan file credentials.txt ke dalam container.

### 5.c
```bash
// Fungsi untuk melakukan hashing MD5 terhadap string input
void md5Hash(const char *input, char *output) {
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5((unsigned char*)input, strlen(input), digest);
    for(int i = 0; i < MD5_DIGEST_LENGTH; i++)
        sprintf(&output[i*2], "%02x", (unsigned int)digest[i]);
}
```
### Penjelasan 
`md5Hash`: Fungsi ini digunakan untuk menghasilkan hash MD5 dari sebuah string input. 
Hash MD5 adalah algoritma hash kriptografis yang menghasilkan representasi numerik unik (digest) dari string input. Fungsi ini mengambil string input, menghitung hash MD5 menggunakan fungsi MD5 yang disediakan oleh pustaka OpenSSL, dan menyimpan hasil hash dalam bentuk string heksadesimal pada variabel output.

```bash
int isUsernameExists(const char *username) {
    // Buka file kredensial
    FILE *file = fopen("credentials.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 0;
    }

    // Cari kredensial sesuai username
    char line[MAX_CREDENTIALS_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        // Pisahkan username dan password dari kredensial
        char storedUsername[MAX_CREDENTIALS_LENGTH];
        sscanf(line, "%[^;];", storedUsername);

        // Cocokkan username dengan kredensial yang tersimpan
        if (strcmp(storedUsername, username) == 0) {
            fclose(file);
            return 1; // Username already exists
        }
    }

    fclose(file);
    return 0; // Username does not exist
}
```
### Penjelasan
`isUsernameExists`: Fungsi ini digunakan untuk memeriksa apakah username yang diberikan sudah ada dalam file `credentials.txt`. Fungsi membuka file `credentials.txt` dalam mode baca ("r"), dan membaca baris per baris menggunakan fungsi fgets. Setiap baris dipecah menjadi username dan password menggunakan fungsi sscanf. Jika username yang diberikan cocok dengan username yang ada dalam file, fungsi mengembalikan nilai 1 yang menandakan username sudah ada; jika tidak, fungsi mengembalikan nilai 0.

```bash
// Fungsi untuk melakukan registrasi user baru dan menyimpannya dalam file
int registerUser(const char *username, const char *password) {
    // Periksa apakah username sudah ada
    if (isUsernameExists(username)) {
        printf("Username already exists.\n");
        return -1;
    }

    
    // Buat string kredensial dengan format "<username>;<password>"
    char cred[MAX_CREDENTIALS_LENGTH];
    snprintf(cred, sizeof(cred), "%s;%s\n", username, password);

    // Buka file untuk menambahkan kredensial baru
    FILE *file = fopen("credentials.txt", "a");
    if (file == NULL) {
        printf("Error opening file.\n");
        return -1;
    }

    // Tulis kredensial ke dalam file
    fputs(cred, file);

    // Tutup file
    fclose(file);

    return 0;
}
```

### Penjelasan
`registerUser`: Fungsi ini digunakan untuk mendaftarkan pengguna baru. Fungsi memeriksa terlebih dahulu apakah username yang diberikan sudah ada menggunakan fungsi isUsernameExists. Jika username belum ada, fungsi membentuk string kredensial dengan format` <username>;<password>`, dan membuka file credentials.txt dalam mode append ("a"). Kemudian, kredensial baru ditulis ke dalam file menggunakan fungsi fputs. Setelah selesai, file ditutup.

```bash
// Fungsi untuk memvalidasi kredensial user saat login
int validateCredentials(const char *username, const char *password) {
    // Buka file kredensial
    FILE *file = fopen("credentials.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 0;
    }

    // Cari kredensial sesuai username
    char line[MAX_CREDENTIALS_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        // Pisahkan username dan password dari kredensial
        char storedUsername[MAX_CREDENTIALS_LENGTH];
        char storedPassword[MAX_CREDENTIALS_LENGTH];
        sscanf(line, "%[^;];%s", storedUsername, storedPassword);

        // Cocokkan username dengan kredensial yang tersimpan
        if (strcmp(storedUsername, username) == 0) {
            // Cocokkan password yang diinput dengan password yang tersimpan
            if (strcmp(storedPassword, password) == 0) {
                fclose(file);
                return 1; // Kredensial valid
            } else {
                fclose(file);
                return 0; // Kredensial tidak valid (password salah)
            }
        }
    }

    fclose(file);
    return 0; // Kredensial tidak valid (username tidak ditemukan)
}
```
### Penjelasan
`validateCredentials`: Fungsi ini digunakan untuk memvalidasi kredensial pengguna saat login. Fungsi membuka file `credentials.txt `dalam mode baca `("r")`, dan melakukan pencarian kredensial sesuai dengan username yang diberikan. Setiap baris dipecah menjadi username dan password menggunakan fungsi `sscanf`. Jika username yang diberikan cocok dengan username yang ada dalam file, fungsi membandingkan password yang diberikan dengan password yang ada dalam file. Jika password cocok, fungsi mengembalikan nilai 1 yang menandakan kredensial valid. Jika tidak cocok, fungsi mengembalikan nilai 0. Jika username tidak ditemukan, fungsi juga mengembalikan nilai 0.

### 5.d
```bash
int  main(int  argc, char *argv[])
{

    //https://drive.google.com/uc?export=download&id=1oDgj5kSiDO0tlyS7-20uz7t20X3atwrq 
    const char *zipPath = "rahasia.zip";
    //DownloadFile("18YCFdG658SALaboVJUHQIqeamcfNY39a", "rahasia.zip");
    //unzipping(zipPath);
    
    snprintf(resultFilePath, sizeof(resultFilePath), "%s/%s", dirpath, "result.txt");

    umask(0);
    
    int choice;
    
    printf("1. Register\n");
    printf("2. Login\n");
    
    printf("Enter your choice: ");
    scanf("%d", &choice);
    
    if(choice == 1) 
    {
    	    printf("Enter username: ");
            scanf("%s", username);
            printf("Enter password: ");
            scanf("%s", password);

            // Hashing password
            md5Hash(password, hashedPassword);

            // Register user
            if (registerUser(username, hashedPassword) == 0) {
                printf("Registration successful!\n");
            } else {
                printf("Registration failed.\n");
            }
    } else if (choice == 2 )
    {
	    printf("Enter username: ");
            scanf("%s", username);
            printf("Enter password: ");
            scanf("%s", password);

            // Hashing password
            md5Hash(password, hashedPassword);

            // Validate credentials
            if (validateCredentials(username, hashedPassword)) {
                printf("Login successful!\n");
                
                system("sudo docker-compose up -d");
                //printf("Docker-compose successful!\n");

                system("sudo docker start modul4_myservice_1");
                //printf("Docker Start successful!\n");
                count_files(pathFile);


                fuse_main(argc, argv, &xmp_oper, NULL);
                //printf("Fuse successful!\n");

                system("sudo docker exec -it modul4_myservice_1 /bin/bash");
            

            } else {
                printf("Invalid credentials.\n");
            }
            
            
    } else {
    	    printf("Invalid choice.\n");
    }
    
}
```
### Penjelasan
Pertama, program menampilkan menu dengan opsi "1. Register" dan "2. Login" menggunakan fungsi printf. Pengguna diminta untuk memasukkan pilihan dengan menggunakan fungsi `scanf` dan nilai tersebut disimpan dalam variabel choice.

Jika choice sama dengan 1, artinya pengguna memilih opsi "Register". Selanjutnya, program meminta pengguna memasukkan username dan password menggunakan fungsi scanf. Password yang dimasukkan akan di-hash menggunakan fungsi md5Hash, dan hasilnya disimpan dalam variabel hashedPassword. Selanjutnya, program memanggil fungsi registerUser untuk mendaftarkan pengguna baru dengan username dan password yang telah di-hash. Jika pendaftaran berhasil, program akan mencetak pesan "Registration successful!".

Jika choice sama dengan 2, artinya pengguna memilih opsi "Login". Pengguna diminta memasukkan username dan password seperti pada langkah sebelumnya. Password yang dimasukkan akan di-hash menggunakan fungsi md5Hash, dan hasilnya disimpan dalam variabel hashedPassword. Selanjutnya, program memanggil fungsi validateCredentials untuk memvalidasi kredensial yang dimasukkan. Jika kredensial valid, program akan mencetak pesan "Login successful!".

Setelah login berhasil, program akan menjalankan beberapa perintah terkait Docker menggunakan fungsi system. Pertama, program akan menjalankan perintah `sudo docker-compose up -d` untuk mengaktifkan layanan `Docker`. Kemudian, program akan menjalankan perintah` sudo docker start modul4_myservice_1` untuk memulai kontainer Docker. Selanjutnya, program akan memanggil fungsi `count_files` untuk menghitung jumlah file dalam suatu direktori yang ditentukan oleh variabel pathFile. Setelah itu, program akan memanggil fungsi fuse_main untuk menjalankan file system FUSE yang didefinisikan oleh variabel `xmp_oper`.

Jika choice tidak sama dengan 1 atau 2, program akan mencetak pesan "Invalid choice."

```bash
static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) 
{
    char fpath[5000]; 
    char extension[256];
    if (strcmp(path, "/") == 0) { 
        path = pathFile;
        snprintf(fpath, sizeof(fpath), "%s", path);
    } else {
        snprintf(fpath, sizeof(fpath), "%s%s", pathFile, path);
    }

    int res = 0; 

    DIR *dp;
    struct dirent *de;
    (void) offset;
    (void) fi;

    dp = opendir(fpath); 

    if (dp == NULL) return -errno;
    
    FILE *resultFile = fopen(resultFilePath, "w");
    if (resultFile == NULL) {
        perror("Failed to create result file");
        closedir(dp);
        return -errno;
    }

    fprintf(resultFile, "%s\n", fpath);
    traverseDirectory(fpath, resultFile, 0);

    fclose(resultFile);

    while ((de = readdir(dp)) != NULL) { 
        struct stat st; 

        memset(&st, 0, sizeof(st));

        st.st_ino = de->d_ino; 
        st.st_mode = de->d_type << 12;

        if ((strcmp(de->d_name, ".") != 0) && (strcmp(de->d_name, "..") != 0)) { 
            if (S_ISDIR(st.st_mode)) { 
                if (strstr(de->d_name, "E01") == NULL) {
                    char oldFilePath[5000];
                    char newFilePath[5000];
                    sprintf(oldFilePath, "%s/%s", fpath, de->d_name);
                    sprintf(newFilePath, "%s/%s_E01", fpath, de->d_name);
                    if (access(newFilePath, F_OK) == -1) {
                        if (rename(oldFilePath, newFilePath) == -1) {
                            perror("Failed to rename directory");
                        }
                    }
                }
            } else if (S_ISREG(st.st_mode)) { 
                if (strstr(de->d_name, "E01") == NULL) {
                    char *ext = strrchr(de->d_name, '.');
                    if (ext != NULL) {
                        char oldFilePath[5000];
                        char newFilePath[5000];
                        sprintf(oldFilePath, "%s/%s", fpath, de->d_name);
                        sprintf(newFilePath, "%s/E01_%s", fpath, de->d_name);
                        if (access(newFilePath, F_OK) == -1) {
                            if (rename(oldFilePath, newFilePath) == -1) {
                                perror("Failed to rename file");
                            }
                        }
                    }
                }
            }
            else{
                printf("Error di s_is %s\n", de->d_name);
            }
        }
        else{
            // printf("Error %s di check dir\n", de->d_name);
        }

        res = filler(buf, de->d_name, &st, 0); 
        if (res != 0) break;
    }
    closedir(dp);
    return 0;
}

```

```bash
static int xmp_rename(const char *oldpath, const char *newpath) 
{
    char fpath[1000], fnewpath[1000]; 

    sprintf(fpath, "%s%s", pathFile, oldpath);
    sprintf(fnewpath, "%s%s", pathFile, newpath);

    int res = rename(fpath, fnewpath); 

    if (res == -1) return -errno;

    return 0;
}
```
### Penjelasan
Fungsi `xmp_readdir`:

Fungsi ini digunakan untuk membaca isi dari suatu direktori.
- Pertama, dilakukan pengecekan apakah path merupakan root direktori ("/") atau bukan. Jika iya, maka path akan diubah menjadi pathFile.
- Kemudian, dilakukan pembukaan direktori menggunakan opendir dengan path fpath. Jika pembukaan direktori gagal, akan dikembalikan nilai -errno yang menunjukkan adanya error.
- Setelah itu, fungsi mencoba membuat file resultFile sebagai file output untuk hasil traversing direktori. Jika pembuatan file gagal, dicetak pesan error, direktori ditutup, dan dikembalikan nilai -errno.
- Path direktori fpath kemudian ditulis ke dalam resultFile menggunakan fprintf.
- Fungsi `traverseDirectory` dipanggil untuk melakukan traversing direktori dan mencetak hasilnya ke dalam resultFile.
- Selanjutnya, menggunakan readdir, entri-entri dalam direktori yang dibuka dibaca satu per satu.
- Jika entri tidak sama dengan "." atau "..", dilakukan pengolahan terhadap entri tersebut.
- Jika entri adalah direktori dan tidak mengandung string "E01", maka direktori tersebut diubah namanya dengan menambahkan "_E01" di bagian akhirnya menggunakan rename.
- Jika entri adalah file dan tidak mengandung string "E01", ekstensi file diambil menggunakan strrchr, kemudian file diubah namanya dengan menambahkan "E01_" di bagian depannya menggunakan rename.
- Terakhir, entri direktori diisi ke dalam buf menggunakan fungsi filler. Jika pengisian entri tidak berhasil, loop dihentikan dan direktori ditutup. Fungsi mengembalikan nilai 0 sebagai indikasi sukses.

Fungsi `xmp_rename`:

Fungsi ini digunakan untuk mengubah nama (rename) suatu file atau direktori.
- Pertama, path lama (oldpath) dan path baru (newpath) digabungkan dengan pathFile menggunakan sprintf untuk membentuk path lengkap (fpath dan fnewpath).
- Kemudian, fungsi rename dipanggil dengan menggunakan path lama dan path baru.
- Jika proses rename gagal, fungsi mengembalikan nilai -errno yang menunjukkan adanya error.
- Jika proses rename berhasil, fungsi mengembalikan nilai 0 sebagai indikasi sukses.

```bash
void traverseDirectory(const char *path, FILE *file, int depth) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("Failed to open directory");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Print indentation based on depth
        for (int i = 0; i < depth; i++) {
            fprintf(file, "|  ");
        }

        // Print entry name
        fprintf(file, "|- %s\n", entry->d_name);

        char entryPath[MAX_PATH_LENGTH];
        snprintf(entryPath, sizeof(entryPath), "%s/%s", path, entry->d_name);

        struct stat st;
        if (lstat(entryPath, &st) == -1) {
            perror("Failed to get file status");
            continue;
        }

        if (S_ISDIR(st.st_mode)) {
            // Recursive call for directories
            traverseDirectory(entryPath, file, depth + 1);
        }
    }

    closedir(dir);
}
```
### Penjelasan
Fungsi ini menerima tiga parameter: path yang merupakan path dari direktori yang akan ditelusuri, file yang merupakan pointer ke file output, dan depth yang merupakan level kedalaman traversal saat ini.
- Pertama, direktori dengan path path dibuka menggunakan opendir. Jika pembukaan direktori gagal, dicetak pesan error menggunakan perror dan fungsi langsung dihentikan.
- Selanjutnya, menggunakan readdir, entri-entri dalam direktori tersebut dibaca satu per satu.
- Jika entri memiliki nama "." atau "..", maka entri tersebut diabaikan dan proses traversal dilanjutkan ke entri berikutnya.
- Dalam loop traversal, terdapat bagian untuk mencetak indentasi berdasarkan kedalaman traversal (depth) ke dalam file menggunakan fprintf. Indentasi ini digunakan untuk memperjelas struktur direktori yang ditampilkan.
- Setelah mencetak indentasi, nama entri (entry->d_name) dicetak ke dalam file menggunakan fprintf.
- Kemudian, path lengkap entri (entryPath) dibentuk dengan menggabungkan path dengan nama entri menggunakan snprintf.
- Menggunakan lstat, status file entri (entryPath) diperoleh dan disimpan dalam struct st. Jika operasi lstat mengalami kegagalan, dicetak pesan error menggunakan perror dan proses traversal dilanjutkan ke entri berikutnya.
- Jika entri merupakan direktori berdasarkan status file (S_ISDIR(st.st_mode)), maka dilakukan pemanggilan rekursif terhadap fungsi traverseDirectory dengan parameter entryPath, file yang sama, dan peningkatan nilai depth.
Setelah selesai traversal, direktori yang telah dibuka ditutup menggunakan closedir.

### 5.e
```bash
void count_files(const char* directory) { // This line declares a function count_files that takes a const char* parameter directory.
    FILE* file = fopen("extension.txt", "w"); // This line opens a file named "extension.txt" in write mode and assigns the file pointer to the variable file.
    DIR* dp; // These lines declare a directory pointer variable dp and open the directory specified by the directory parameter.
    dp = opendir(directory);
    if (dp == NULL) {
        printf("Failed to open directory: %s\n", directory);
        return;
    }

    struct dirent* de; // These lines declare a dirent pointer variable de and a character array extension.
    char extension[256];

    while ((de = readdir(dp)) != NULL) { // This line starts a loop that iterates through each directory entry in the opened directory.
        struct stat st; // These lines declare a stat structure st and initialize it with zeros.
        memset(&st, 0, sizeof(st));

        st.st_ino = de->d_ino; // These lines assign the inode number and file type information from the directory entry to the stat structure.
        st.st_mode = de->d_type << 12;
    
        if ((strcmp(de->d_name, ".") != 0) && (strcmp(de->d_name, "..") != 0)) { // This condition checks if the current directory entry is not "." or "..".
            if (S_ISREG(st.st_mode)) { // This condition checks if the current entry is a regular file.
                char* ext = strrchr(de->d_name, '.');
                if (ext != NULL) { // This line finds the last occurrence of the '.' character in the file name and assigns the pointer to ext.
                    strcpy(extension, ext + 1); // This condition checks if a file extension is found.

                    int found = 0; // This line copies the file extension (excluding the '.') to the extension array.
                    for (int i = 0; i < numExtensions; i++) {
                        if (strcmp(extcnt[i].extension, extension) == 0) {
                            extcnt[i].count++;
                            found = 1;
                            break;
                        }
                    }
                    if (!found) { // These lines search for the extension in the extcnt array and increment its count if found. If not found, it adds a new entry to the array.
                        strcpy(extcnt[numExtensions].extension, extension);
                        extcnt[numExtensions].count = 1;
                        (numExtensions)++;
                    }
                }
            } else if (S_ISDIR(st.st_mode)) { // These lines add a new extension entry to the extcnt array if it was not found previously.
                char subdir[256];
                sprintf(subdir, "%s/%s", directory, de->d_name);
                count_files(subdir);
            }
        }
    }

    for (int i = 0; i < numExtensions; i++) { // This condition checks if the current entry is a directory. If so, it recursively calls the count_files function on the subdirectory.
        fprintf(file, "%s: %d\n", extcnt[i].extension, extcnt[i].count);
    }
    fclose(file); // This loop writes the extension count information to the opened file.
    closedir(dp);
}
```
### Penjelasan 
Fungsi count_files menerima parameter directory yang merupakan path direktori yang akan dihitung.
- Fungsi membuka file "extension.txt" dalam mode tulis dan menyimpan file pointer-nya dalam variabel file.
- Direktori yang akan dihitung dibuka menggunakan opendir dan pointer direktori disimpan dalam variabel dp.
- Selama ada entri direktori, fungsi akan melakukan iterasi menggunakan readdir.
 Untuk setiap entri, fungsi memeriksa apakah itu file biasa atau direktori menggunakan S_ISREG dan S_ISDIR.
- Jika entri adalah file biasa, fungsi mengekstrak ekstensi file menggunakan strrchr dan menyimpannya dalam variabel extension.
- Selanjutnya, fungsi memeriksa apakah ekstensi tersebut sudah ada dalam array extcnt menggunakan perulangan for.
- Jika ekstensi sudah ada, jumlahnya ditambahkan. Jika tidak, ekstensi baru ditambahkan ke array extcnt dengan jumlah awal 1.
- Jika entri adalah direktori, fungsi akan melakukan pemanggilan rekursif terhadap count_files pada subdirektori.
- Setelah selesai mengiterasi semua entri, fungsi menulis informasi jumlah ekstensi ke file "extension.txt" menggunakan fprintf.
- Fungsi menutup file dan direktori yang dibuka sebelum mengakhiri eksekusi.


## Dokumentasi Soal 5
### 5.a
![Screenshot (1300)](https://github.com/RuleLuluDamara/Images/assets/105763198/516029a0-00a5-4dd7-809e-e262071f1226)
### 5.b
![Screenshot (1302)](https://github.com/RuleLuluDamara/Images/assets/105763198/c5be1e7b-c3f6-4ceb-9eef-1c8e4cebe582)
![Screenshot (1305)](https://github.com/RuleLuluDamara/Images/assets/105763198/878992a8-8e86-47be-b2b5-e1dc3e6c463a)
### 5.c
![Screenshot (1307)](https://github.com/RuleLuluDamara/Images/assets/105763198/4431aeaf-868b-419a-adba-f9b5f1284043)
### 5.d
![Screenshot (1308)](https://github.com/RuleLuluDamara/Images/assets/105763198/d07390eb-c88a-4887-aa1f-e136a5990862)
### 5.e
![Screenshot (1309)](https://github.com/RuleLuluDamara/Images/assets/105763198/3bbc2cbc-450d-4bf8-9fe4-da625a169ce9)

## Revisi Soal %
- Perbaikan syarat user yang login dapat membaca folder dan file yang di-mount 
- Penambahan features menggunakan FUSE lakukan rename pada folder
- Penambahan code agar List seluruh folder, subfolder, dan file yang telah di-rename dalam file result.txt menggunakan tree
- Penambahan code untuk menghitung file tersebut berdasarkan extension dan output-kan menjadi extension.txt.
