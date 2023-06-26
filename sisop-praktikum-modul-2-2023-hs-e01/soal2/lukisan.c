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

#define FOLDER_PATH "/home/rosy/sisop/modul2/soal2"
#define PIC_URL "https://picsum.photos/%d"


// 2d dan 2e = Program killer
void coba_Kill(char *tanda2, int pid)
{
    char killname[10000], kill_name[10000];
    // simpan killer di direktori
    strcpy(killname, FOLDER_PATH);
    strcat(killname, "/killer");

    // beri ekstensi .c
    strcpy(kill_name, killname);
    strcat(kill_name, ".c");

    // buka file killer
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

    // 2d. generate program killer dan delete dirinya sendiri
    // compile program killer
    if (fork() == 0)
    {
        char *args[] = {"gcc", kill_name, "-o", killname, NULL};
        execv("/bin/gcc", args);
    }
    wait(NULL);
    // remove program killer
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
        //printf("process_id of child process %d \n", process_id);
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
    chdir("/home/rosy/sisop/modul2/soal2");
    // Close stdin. stdout and stderr
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
            // Dapatkan waktu sekarang
            time(&now);
            timeinfo = localtime(&now);
            DIR *dir;
            // open direktori
            dir = opendir(FOLDER_PATH);
            if ((dir) != NULL)
            {
                // Buat nama folder dari timestamp
                strftime(folder_name, sizeof(folder_name), "%Y-%m-%d_%H:%M:%S", timeinfo);
                // Buat folder
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
                    // ke direktori folder
                    strcpy(img_name, folder_name);
                    strcat(img_name, "/");
                    // buat nama file gambar dengan timestamp
                    strftime(img, sizeof(img), "%Y-%m-%d_%H:%M:%S", timeinfo1);
                    strcat(img_name, img);
                    strcat(img_name, ".jpg");
                    // menyesuaikan ukuran gambar
                    sprintf(url, PIC_URL, (((int)now1) % 1000) + 50);
                    // download gambar
                    char *argv[] = {"wget", "-qO", img_name, url, NULL};
                    execv("/bin/wget", argv);
                }
                // tunggu 5 detik ebelum membuat unduhan berikutnya
                sleep(5);
            }
            wait(NULL);
            // zipping file lalu remove folder
            if (fork() == 0)
            {
                char zip_name[200];
                // buat nama zip, pakai nama foldernya
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