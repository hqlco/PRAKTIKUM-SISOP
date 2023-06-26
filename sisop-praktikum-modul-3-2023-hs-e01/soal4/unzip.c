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
#include <pthread.h>

#define FOLDER_PATH "/home/ikikk/sisop-praktikum-modul-3-2023-hs-e01/soal4"
#define ZIP_URL "https://drive.google.com/uc?id=1rsR6jTBss1dJh2jdEKKeUyTtTRi_0fqp&export=download"


void download_zip(char *zipPath){
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
        printf("Downloading file : %s\n", ZIP_URL);
        char *argv[] = {"wget", "-qO", zipPath, ZIP_URL, NULL};
        execv("/bin/wget", argv);
        // execl("/bin/wget", "wget", "--no-check-certificate", url_new, "-O", zipPath, NULL);
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

void unzipping(char *zipPath)
{
    char *foldername = "hehe";

    pid_t pidUnzip = fork();
    if(pidUnzip == 0){
        printf("Unzipping file : %s\n", zipPath);
        // execl("/bin/unzip", "unzip", "-d", "binatang", zipPath, NULL);
        char *argv[] = {"unzip", "-q", "-n", zipPath, "-d", foldername, NULL};
        execv("/bin/unzip", argv);
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

int main()
{
    char *zipPath = "hehe.zip";
    download_zip(zipPath);
    unzipping(zipPath);
}