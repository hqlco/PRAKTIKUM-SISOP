#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <wait.h>
#include <time.h>

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
