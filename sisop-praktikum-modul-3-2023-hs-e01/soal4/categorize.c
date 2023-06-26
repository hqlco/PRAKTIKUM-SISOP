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

#define MAX_PATH_LENGTH 1024

#define log_file "log.txt"
#define list_file "lists.txt"
#define ext_file "/home/ikikk/sisop-praktikum-modul-3-2023-hs-e01/soal4/hehe/extensions.txt"
#define max_file  "/home/ikikk/sisop-praktikum-modul-3-2023-hs-e01/soal4/hehe/max.txt"
#define HOME_PATH "/home/ikikk/sisop-praktikum-modul-3-2023-hs-e01/soal4/hehe"
#define FILES_PATH "/home/ikikk/sisop-praktikum-modul-3-2023-hs-e01/soal4/hehe/files"
#define FILES "./files"
// pthread_t threads[2];

int max_values = 0;
char extensions[MAX_PATH_LENGTH][10];
int ext_dir[MAX_PATH_LENGTH];
char log_time[100];
char logname[100];
char log_name[100];
char filename[MAX_PATH_LENGTH];
int ext_count = 0;

// cek ekstensi di nama file
const char *check_name(char *dir){
    const char *name = strrchr(dir, '.');
    if(!name || name == dir)
        return "";
    return name + 1;
}

// compare ekstensi yang ada di extensions.txt dengan ekstensi yang ada di nama file
int check_extension(char ext[]) {
    for(int i=0; i<ext_count;i++){
        if(strcmp(ext, extensions[i]) == 0)
            return i;
    }
    return -1;
}

// timestamp
char get_log_time(char *buffer) {
    time_t now = time(NULL);
    strftime(buffer, 20, "%d-%m-%Y %H:%M:%S", localtime(&now));
}

// dapetin ekstensi dari extensions.txt trus bikin folder sekalian
void* get_extension() { 
    // buka file extensions.txt
    FILE *filePointer = fopen(ext_file, "r");

    // kalo null bakal exit
    if (filePointer == NULL) {
        printf("Gagal membuka file!");
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_PATH_LENGTH];
    char **lines = NULL;
    // int num_lines = 0;
    int index=0;

    // baca tiap baris
    while (fgets(buffer, MAX_PATH_LENGTH, filePointer)) {
        lines = realloc(lines, sizeof(char*) * (ext_count + 1));
        lines[ext_count] = malloc(sizeof(char) * (strlen(buffer) + 1));

        // copy isi tiap baris ke lines[] + hilangin spasi
        strncpy(lines[index], buffer, strlen(buffer) - 2);
        ext_dir[index] = 0;

        // copy ke variabel global
        strcpy(extensions[index], lines[index]);
        index++;
        ext_count++;
    }
    // tutup file extensions.txt
    fclose(filePointer);

    // change directory ke home
    chdir(HOME_PATH);

    // simpan log di direktori
    strcpy(logname, HOME_PATH);
    strcat(logname, "/log");

    // beri ekstensi .txt
    strcpy(log_name, logname);
    strcat(log_name, ".txt");

    // make directory categorized di home
    sprintf(filename, "categorized");
    mkdir(filename, 0777);

    // buka file log.txt
    filePointer = fopen(log_name, "a+");

    // tulis lognya pake timestamp
    get_log_time(log_time);
    fprintf(filePointer, "%s MADE %s\n", log_time, filename);

    // tutup file log.txt
    fclose(filePointer);
    
    // buat folder per ekstensi
    for (int i = 0; i < ext_count; i++) {

        // buat foldernya
        sprintf(filename, "categorized/%s", extensions[i]);
        mkdir(filename, 0777);

        // tulis lognya
        filePointer = fopen(log_name, "a+");
        get_log_time(log_time);
        fprintf(filePointer, "%s MADE %s\n", log_time, filename);

        // janlup ditutup filenya
        fclose(filePointer);

    }

    // buat folder other buat file dengan ekstensi yang gaada di list extensions.txt
    sprintf(filename, "categorized/other");
    mkdir(filename, 0777);

    // janlup tulis lognya juga
    filePointer = fopen(log_name, "a+");
    get_log_time(log_time);
    fprintf(filePointer, "%s MADE %s\n", log_time, filename);

    // janlup ditutup
    fclose(filePointer);
}

void *scan_files(void *arg) {
    DIR *dir;
    struct dirent *ent;
    char path[MAX_PATH_LENGTH];
    char cmd[9000];

    // open direktori files
    if ((dir = opendir((char*) arg)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            // kalo tipenya direktori
            if (ent->d_type == DT_DIR) {  // directory file
                // di cek masih ada lanjutan folder ato engga
                if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {  // directory  
                    continue;        
                } 

                // tulis path direktori
                sprintf(path, "%s/%s", (char*) arg, ent->d_name);

                // tulis path direktorinya ke log
                get_log_time(log_time);
                sprintf(cmd, "echo '%s ACCESSED %s' >> %s", log_time, path, log_name);
                system(cmd);

                // direkursif buat scan yg lain

                pthread_t tid;
                pthread_create(&tid, NULL, scan_files, (void*) path);
                pthread_join(tid, NULL);

            }
            else {
                // kalo tipenya file reguler, taruh di file temporary, ntar janlup dihapus
                sprintf(cmd, "echo '%s/%s' >> lists.txt", (char*) arg,ent->d_name);
                system(cmd);    
            }
        }

        // tutup
        closedir(dir);
    }
}

// buat dapetin jumlah file maksimal dalam folder
void *read_max() {
    FILE *filePointer ;
    char line[255];
    int i = 0;

    // buka max.txt
    filePointer = fopen(max_file, "r");

    // kalo gagal keluar
    if (filePointer == NULL) {
        printf("Gagal membuka file!");
        exit(EXIT_FAILURE);
    }

    // maxnya disimpen di max_values
    while (fgets(line, sizeof(line), filePointer) ) {
        max_values = atoi(line);
    }

    // tutup
    fclose(filePointer);

    // lanjut scan filenya
    scan_files(FILES);
}

// buat mindahin file yg udah di scan trus mau dikelompokin berdasarkan ekstensi
void *move_files() {
    FILE *filePointer;
    char cmd[9000];
    char ext[MAX_PATH_LENGTH];
    char line[MAX_PATH_LENGTH];
    char buffer[5000];

    // buka lists.txt, buat sendiri ni
    filePointer = fopen(list_file, "r");

    // scan tiap baris
    while(fgets(line, MAX_PATH_LENGTH, filePointer)) {
        line[strlen(line)-1] = '\0';

        // tiap baris disimpen di array
        sprintf(ext, "%s", check_name(line));

        // jadiin huruf kecil semua
        for(int i = 0; ext[i]; i++){
            ext[i] = tolower(ext[i]);
        }

        // di cek ekstensinya, kalo beda masuk folder other
        if (check_extension(ext) == -1) {
            // cp tu copy ya
            sprintf(cmd, "cp '%s' ./categorized/other", line);
            system(cmd);

            // tulis lognya
            get_log_time(log_time);
            sprintf(cmd, "echo '%s MOVED other file : %s > ./categorized/other' >> %s", log_time, line, log_name);
            system(cmd);
        }
        else { // kalo sama masuk folder ekstensi masing2
            // folder pertama diisi dulu sampe penuh
            if (ext_dir[check_extension(ext)] < max_values) {
                // filenya di copy ke folder tiap ekstensi
                sprintf(cmd, "cp '%s' ./categorized/%s", line, ext);
                system(cmd);

                // dilanjutin
                ext_dir[check_extension(ext)]++;

                // tulis lognya
                get_log_time(log_time);
                sprintf(cmd, "echo '%s MOVED %s file : %s > ./categorized/%s' >> %s", log_time, ext, line, ext, log_name);
                system(cmd);
            }
            else { // kalo folder pertama udah penuh, bikin folder part lanjutan
                int num_ext = ext_dir[check_extension(ext)] / max_values + 1;
                sprintf(buffer, "./categorized/%s (%d)", ext, num_ext);

                // tulis di log
                if(access(buffer, F_OK) != 0){
                    get_log_time(log_time);
                    sprintf(cmd, "echo '%s MADE %s' >> %s", log_time, buffer, log_file);
                    system(cmd);
                }

                // bikin folder part lanjutan
                mkdir(buffer, 0777);

                // filenya di copy di folder part lanjutan
                sprintf(cmd, "cp '%s' './categorized/%s (%d)'", line, ext, num_ext);
                system(cmd);

                // lanjuttt
                ext_dir[check_extension(ext)]++;

                // tulis log
                get_log_time(log_time);
                sprintf(cmd, "echo '%s MOVED %s file : %s > .%s' >> %s", log_time, ext, line, buffer, log_name);
                system(cmd);
            }
        }
    }
}

// buat ngitung file tiap ekstensi
void *count_extension() {
    char cmd[9000];
    // folder other bakal dicek dulu
    strcpy(extensions[ext_count], "other");
    DIR *dir;
    struct dirent *ent;
    dir = opendir("./categorized/other");

    // janlup lognya
    get_log_time(log_time);
    sprintf(cmd, "echo '%s ACCESSED ./categorized/other' >> %s", log_time, log_name);
    system(cmd);

    while ((ent = readdir(dir)) != NULL) {
        // kalo file reguler,
        if (ent->d_type == DT_REG) {
            //dihitung + 1
            ext_dir[ext_count]++;
        }
    }

    closedir(dir);
    // trus  dilanjut hitung folder selanjutnya
    ext_count++;

    // ini logic sort sih, ada di google
    for(int i=0;i<ext_count-1; i++){
        for(int j=0; j<ext_count-i-1;j++){
            if(ext_dir[j] > ext_dir[j+1]){
                int temp;
                temp = ext_dir[j];
                ext_dir[j] = ext_dir[j+1];
                ext_dir[j+1] = temp;

                strcpy(filename, extensions[j]);
                strcpy(extensions[j], extensions[j+1]);
                strcpy(extensions[j+1], filename);
            }
        }
    }
    
    // di print deh
    for(int i=0; i<ext_count; i++){
        printf("%s : %d\n", extensions[i], ext_dir[i]);
    }
}

int main() {
    pthread_t threads;

    get_extension();

    pthread_create(&threads, NULL, read_max, NULL); 
    pthread_join(threads, NULL);

    pthread_create(&threads, NULL, move_files, NULL);
    pthread_join(threads,NULL);

    pthread_create(&threads, NULL, count_extension, NULL);
    pthread_join(threads,NULL);

    system("rm lists.txt");

    return 0;
}