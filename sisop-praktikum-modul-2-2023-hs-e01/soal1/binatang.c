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

void cekDir(const char *folderPath){
    DIR* dir;
    dir = opendir(folderPath);
    if (dir == NULL) 
    {
        printf("No Directory Named '%s'\n", folderPath);
    }
}

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
    while ((imagesFile = readdir(dir)) != NULL) 
    {
        if (strstr(imagesFile->d_name, ".jpg") != NULL) 
        {
            filenames[imgCount] = strdup(imagesFile->d_name);
            imgCount++;
        }
    }

    closedir(dir);

    if (imgCount == 0) 
    {
        printf("No image files found in directory '%s'\n", folderPath);
    }

    gachaIndex = rand() % imgCount;
    gachaImageName = filenames[gachaIndex];
    printf("Gacha : %s\n", gachaImageName);

    for (int i = 0; i < imgCount; i++) 
    {
        free(filenames[i]);
    }
}

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

void createFolder(char *dirName)
{
    int status = 0;
    pid_t pid = fork();
    if (pid == 0)
    {
        char *argv[] = {"mkdir", "-p", dirName, NULL};
        execv("/bin/mkdir", argv);
    }
    while (wait(&status) > 0);
}

void removeFolder(const char *dirName) {
    pid_t pid = fork();
    if (pid < 0) 
    {
        printf("Failed to fork!\n");
        exit(EXIT_FAILURE);
    } 
    else if (pid == 0)
    {
        execl("/bin/rm", "rm", "-rf", dirName, NULL);
        exit(EXIT_FAILURE);
    } 
    else 
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {} 
    }
}

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