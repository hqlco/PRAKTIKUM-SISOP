// mengekstrak informasi dari log.txt
// 1. hitung banyak accessed
// 2. list seluruh folder yang telah dibuat dan banyaknya file yang dikumpulkan ke folder tersebut secara ascending
// 3. menghitung total file tiap extension secara ascending

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>
#include <ctype.h>

#define HOME_PATH "/home/ikikk/sisop-praktikum-modul-3-2023-hs-e01/soal4/hehe"
#define log_file "log.txt"
#define ext_file "extensions.txt"

#define MAX_PATH_LENGTH 1024
char dirpath[MAX_PATH_LENGTH][MAX_PATH_LENGTH];
char extpath[MAX_PATH_LENGTH][MAX_PATH_LENGTH];
int dirtotal[MAX_PATH_LENGTH];
int exttotal[MAX_PATH_LENGTH];
char filename[MAX_PATH_LENGTH];
int idxdir = 0;
int idxext = 0;

// ini ngitung total yg diakses berdasarkan log.txt
int accessed() {

    // chdir(HOME_PATH);
    FILE *filePointer;
    char line[MAX_PATH_LENGTH];
    int access = 0;

    // buka log.txt
    filePointer = fopen(log_file, "r");
    // kalo null keluar
    if (filePointer == NULL) {
        printf("Gagal membuka file!");
        exit(EXIT_FAILURE);
    }

    // di scan per barisnya
    while (fgets(line, MAX_PATH_LENGTH, filePointer)) {
        // di cek, kalo ada ACCESSED, access di + 1
        if (strstr(line, "ACCESSED") != NULL) {
            access++;
        }
    }

    // ntar return total aksesnya
    return access;
    fclose(filePointer);
}


// yg ini buat ngitung banyak file tiap folder
void dir_total() {
    FILE *filePointer;
    char line[MAX_PATH_LENGTH];

    // open file log
    filePointer = fopen(log_file, "r");

    // cek isi file, kalo ada lanjut
    if (filePointer == NULL) {
        // kalo kosong exit
        printf("Gagal membuka file!");
        exit(EXIT_FAILURE);
    }

    // dapetin isi per baris
    while (fgets(line, MAX_PATH_LENGTH, filePointer)) {
        line[strlen(line)-1] = '\0';

        // cari log yg ada MADE nya
        if (strstr(line, "MADE") != NULL) {
            // cari karakter / buat dapetin endpoint direktorinya
            char *k = strrchr(line, '/');

            // kalo ada
            if (k != NULL) {
                //total direktori di nol in dulu
                dirtotal[idxdir] = 0;
                // trus endpoint direktorinya disimpen di dirpath[]
                strcpy(dirpath[idxdir++], k+1);
            }
        }

        // cari log yang ada MOVED nya
        if (strstr(line, "MOVED") != NULL) {
            // cari karakter / buat dapetin endpoint direktorinya
            char *k = strrchr(line, '/');

            // kalo ada
            if (k != NULL) {
                // di loop buat dapetin banyak file yg dipindah
                for (int i=0 ; i<idxdir ; i++) {
                    // di cek dulu endpoint direktorinya yg di file log sama ato engga sama yg udh disimpen di dirpath[]
                    if (strcmp(dirpath[i], k+1) == 0) {
                        // kalo sama totalnya di ++
                        dirtotal[i]++;
                    }
                }
            }
        }
        
    }
    
    // buat sorting ascending
    for(int i=0;i<idxdir; i++){
        for(int j=0; j<idxdir-1;j++){
            if(dirtotal[j] > dirtotal[j+1]){
                
                // buat sort direktorinya dulu
                char buffer[MAX_PATH_LENGTH];
                strcpy(buffer, dirpath[j]);
                strcpy(dirpath[j], dirpath[j+1]);
                strcpy(dirpath[j+1], buffer);

                // baru totalnya
                int temp;
                temp = dirtotal[j];
                dirtotal[j] = dirtotal[j+1];
                dirtotal[j+1] = temp;
            }
        }
    }

    // trus di print
    for(int i=0; i<idxdir; i++){
        printf("%s : %d\n", dirpath[i], dirtotal[i]);
    }

    fclose(filePointer);
}

// ini kurleb sama kek di categorize.c
void ext_total() {
    FILE *filePointer;
    char line[MAX_PATH_LENGTH];

    // di buka kek biasa, tp ini buka extensions.txt dulu
    filePointer = fopen(ext_file, "r");
    // kalo gabisa keluar
    if (filePointer == NULL) {
        printf("Gagal membuka file!");
        exit(EXIT_FAILURE);
    }

    // scan per baris
    while (fgets(line, MAX_PATH_LENGTH, filePointer)) {
        line[strlen(line)-2] = '\0';
        // trus dicopy ke variabel global
        strcpy(extpath[idxext], line);
        //total ekstensinya diinisialisasi sm 0 dulu
        exttotal[idxext] = 0;
        //index ekstensinya di + 1
        idxext++;
    }
    // file extensions.txt ditutup karena gaperlu lagi
    fclose(filePointer);

    // pertama itung yg other dulu
    strcpy(extpath[idxext], "other");
    exttotal[idxext]=0;
    idxext++;

    // buka file lognya
    filePointer = fopen(log_file, "r"); 
    // kalo gagal keluar
    if (filePointer == NULL) {
        printf("Gagal membuka file!");
        exit(EXIT_FAILURE);
    }

    // scan per baris
    while(fgets(line, MAX_PATH_LENGTH, filePointer)){

        // cek kalau ada MOVED
        if (strstr(line, "MOVED ") != NULL) {
            line[strlen(line)-1] = '\0';
            // dicek MOVED nya
            char *start = strstr(line, "MOVED ");

            // trus cek ada file : nya
            char *en = strstr(start, " file :");
            
            // di ambil belakangnya aja, alias ekstensinya
            strncpy(filename, start + strlen("MOVED "), en - start - strlen("MOVED "));
            filename[en - start - strlen("MOVED ")] = '\0';

            // di cek ekstensinya
            for (int i=0 ; i<idxdir ; i++) {
                if (strcmp(extpath[i], filename) == 0) {
                    exttotal[i]++;
                }
            }
        }


    }
    fclose(filePointer);

    // logic sort biasa sih
    for(int i=0;i<idxext; i++){
        for(int j=0; j<idxext-1;j++){
            if(exttotal[j] > exttotal[j+1]){
                char buffer[MAX_PATH_LENGTH];
                strcpy(buffer, extpath[j]);
                strcpy(extpath[j], extpath[j+1]);
                strcpy(extpath[j+1], buffer);

                int temp;
                temp = exttotal[j];
                exttotal[j] = exttotal[j+1];
                exttotal[j+1] = temp;
            }
        }
    }

    // trus di print yey
    for(int i=0; i<idxext; i++){
        printf("%s : %d\n", extpath[i], exttotal[i]);
    }
}

int main() {
    chdir(HOME_PATH);
    printf("File Accessed : ");
    printf("%d\n\n", accessed());

    printf("Directory Lists :\n");
    dir_total();
    
    printf("\nExtensions Total : \n");
    ext_total();
    
    return 0;
}
