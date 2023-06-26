#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <time.h>

void unzipping(const char *downloadName) {
    printf("Unzipping file : %s\n", downloadName);
    chdir("/home/rulelulu/Documents/praktikum/modul2/soal3/");
    execl("/bin/unzip", "unzip", downloadName , NULL);
    exit(0);

} 

void removeZip(const char *downloadName) {
    printf("remove zip file : %s\n", downloadName);
    execl("/bin/rm", "rm", downloadName, NULL);
    exit(0);
}

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
                } else {
                    while ((wait(&status4)) > 0);
                }
            } 
        }
    }
}

void cekDir(const char *dirPath){
    DIR* dir;
    dir = opendir(dirPath);
    if (dir == NULL) 
    {
        printf("No Directory Named '%s'\n", dirPath);
    }
}

void createFolder(char *posisi)
{
    int status = 0;
    pid_t pid = fork();
    if (pid == 0)
    {
        char *argv[] = {"mkdir", "-p", posisi, NULL};
        execv("/bin/mkdir", argv);
    }
    while (wait(&status) > 0);
}

// fungsi untuk memindahkan file
void moveFile(char *filename, char *posisi, char *baseFolder)
{
    int status = 0;
    char source[1000], dest[1000];
    snprintf(source, sizeof source, "%s/%s", baseFolder, filename);
    snprintf(dest, sizeof dest, "%s/%s", posisi, filename);
    // printf("dest now :%s\n",dest);

    if (fork() == 0)
    {
        char *argv[] = {"mv", source, dest, NULL};
        execv("/bin/mv", argv);
    }
    while (wait(&status) > 0);
}

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
}

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

void showPlayers(struct Pemain *pemain, int jumlah_pemain) {
    for (int i = 0; i < jumlah_pemain; i++) {
        printf("%s %s %s %d\n", pemain[i].nama, pemain[i].club, pemain[i].posisi, pemain[i].rating);
    }
}

void findPlayers(struct Pemain *pemain, int jumlah_pemain, char *posisi) {
    printf("Pemain pada posisi %s:\n", posisi);
    for (int i = 0; i < jumlah_pemain; i++) {
        if (strcmp(pemain[i].posisi, posisi) == 0) {
            printf("%s %s %d\n", pemain[i].nama, pemain[i].club, pemain[i].rating);
        }
    }
}
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

void buatTim(int jumlah_bek, int jumlah_gelandang, int jumlah_penyerang) {
    tentukanKesebelasan(players, 30, jumlah_bek, jumlah_gelandang, jumlah_penyerang);
}

int main()
{
    //https://drive.google.com/uc?export=download&id=1oDgj5kSiDO0tlyS7-20uz7t20X3atwrq 
    //https://drive.google.com/file/d/1zEAneJ1-0sOgt13R1gL4i1ONWfKAtwBF/view

    const char *downloadName = "players.zip";
    char* folderName = "players";
    const char* url = "1zEAneJ1-0sOgt13R1gL4i1ONWfKAtwBF";
    const char* folderPath = "./players";
    // 3a dan 3b
    //DownloadFile( url, downloadName, folderName);

    // 3c
    //category("/home/rulelulu/Documents/praktikum/modul2/soal3/players");

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
