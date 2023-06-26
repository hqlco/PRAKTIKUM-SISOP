#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <time.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <dirent.h>

#define PORT 8080
#define MAX_LINE_LENGTH 100
#define MAX_COLUMNS 100
typedef struct mencoba
{
    char name[50];
    int tanda;
    int hasil;
} Coba11;

int folderExists(const char *folderPath)
{
    DIR *dir = opendir(folderPath);
    if (dir)
    {
        // Folder exists
        closedir(dir);
        return 1;
    }
    else
    {
        // Folder does not exist or cannot be accessed
        return 0;
    }
}
int fileExists(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file)
    {
        fclose(file);
        return 1;
    }
    return 0;
}
// void removeNewline(char *str)
// {
//     size_t length = strlen(str);
//     if (length > 0 && str[length - 1] == '\n')
//     {
//         str[length - 1] = '\0';
//     }
// }
int countOccurrences(const char *string, char target)
{
    int count = 0;

    while (*string)
    {
        if (*string == target)
        {
            count++;
        }

        string++;
    }

    return count;
}
int validateCredentials(char *user_pswd)
{
    FILE *file = fopen("databases/database_admin/user.csv", "r");
    if (file == NULL)
    {
        printf("Failed to open credentials file.\n");
        return 0;
    }
    // printf("%s\n", user_pswd);
    char line[1024];
    while (fgets(line, 1024, file) != NULL)
    {
        // printf("%s\n", line);
        // removeNewline(line);
        if (strcmp(line, user_pswd) == 0)
        {
            // printf("Credentials are valid.\n");
            fclose(file);
            return 1; // Credentials are not valid
        }
    }
    fclose(file);
    return 0; // Credentials are not valid
}

void createLog(const char *user, const char *command)
{
    time_t currentTime;
    struct tm *timeInfo;
    char timestamp[20];

    // Get the current time
    time(&currentTime);
    timeInfo = localtime(&currentTime);

    // Format the timestamp as "yyyy-mm-dd hh:mm:ss"
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeInfo);

    // Open the log file in append mode
    FILE *file = fopen("log.txt", "a");

    if (file != NULL)
    {
        // Write the log entry to the file
        fprintf(file, "%s:%s:%s\n", timestamp, user, command);

        // Close the file
        fclose(file);
    }
    else
    {
        printf("Error opening log file.\n");
    }
}

int main(int argc, char *argv[])
{
    int isRoot = 0;
    char username[1024];
    char password[1024];
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

    // Change the current working directory
    chdir("/home/leo/sisop/FP/database");

    // Close stdin. stdout and stderr
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    open("/dev/null", O_RDONLY);
    open("/dev/null", O_RDWR);
    open("/dev/null", O_RDWR);

    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // validation

    char buffer2[1024] = {0};
    valread = read(new_socket, buffer2, 1024);
    if (!strcmp(buffer2, "0"))
    {
        send(new_socket, "Successfully connected as root!", 1024, 0);
        strcpy(username, "Root");
        strcpy(password, "");
        isRoot = 1;
    }
    else
    {
        char buffer3[1024] = {0};
        send(new_socket, "HAYO", 10, 0);
        // bzero(buffer3, 1024);
        valread = read(new_socket, buffer3, 1024);
        char uname[1024];
        char upassword[1024];
        char strbackup[1024];
        bzero(uname, 1024);
        bzero(upassword, 1024);
        bzero(strbackup, 1024);
        strcpy(strbackup, buffer3);
        char *ptr = strbackup;
        char *token;

        int i;
        for (i = 0; token = strtok_r(ptr, ",", &ptr); i++)
        {
            if (i == 0)
            {
                strcpy(uname, token);
            }
            else if (i == 1)
            {
                strcpy(upassword, token);
            }
        }

        int valid = validateCredentials(buffer3);
        // char struid[10];
        // sprintf(struid, "%d", valid);
        // send(new_socket, struid, strlen(struid), 0);
        // send(new_socket, struid, strlen(struid), 0);
        if (valid == 1)
        {
            char command1[1024];
            bzero(username, 1024);
            bzero(password, 1024);
            strcpy(command1, "Successfully connected! as ");
            strcat(command1, uname);
            send(new_socket, command1, 1024, 0);
            strcpy(username, uname);
            strcpy(password, upassword);
        }
        else
        {
            send(new_socket, "Wrong username or password!", 1024, 0);
        }
    }

    const char *folderPath1 = "databases";

    if (!folderExists(folderPath1))
    {
        system("mkdir databases");
    }
    const char *folderPath2 = "databases/database_admin";
    if (!folderExists(folderPath2))
    {
        system("mkdir databases/database_admin");
    }
    const char *folderPath3 = "databases/database_template";
    if (!folderExists(folderPath3))
    {
        system("mkdir databases/database_template");
    }
    if (!fileExists("databases/database_template/user.csv"))
    {
        system("touch databases/database_template/user.csv");
        system("echo \"username\" > databases/database_template/user.csv");
        system("echo \"Root\" >> databases/database_template/user.csv");
    }
    if (!fileExists("databases/database_admin/user.csv"))
    {
        system("touch databases/database_admin/user.csv");
        system("echo \"username,password\" > databases/database_admin/user.csv");
        system("echo \"Root,\" >> databases/database_admin/user.csv");
    }
    char currentDatabase[1024] = "database_template";

    //? main program
    while (1)
    {
        char buffer[1024] = {0};
        // read
        valread = read(new_socket, buffer, 1024);
        createLog(username, buffer);
        int index = strlen(buffer) - 1;
        if (buffer[index] != 59)
        {
            send(new_socket, "Syntax error!", 1024, 0);
            continue;
        }
        else if (!strncmp(buffer, "CREATE USER", 11))
        {
            if (isRoot)
            {
                char uname[1024];
                char upassword[1024];
                bzero(uname, 1024);
                bzero(upassword, 1024);

                char cmd[1024];
                strcpy(cmd, buffer);
                char *cmdptr = cmd;
                char *token;
                int i;
                int tanda = 0;
                for (i = 0; token = strtok_r(cmdptr, " ", &cmdptr); i++)
                {

                    if (i == 2)
                    {
                        strcpy(uname, token);
                    }
                    else if (i == 5)
                    {
                        strncpy(upassword, token, strlen(token) - 1);
                    }
                    else if ((i == 3 && strcmp(token, "IDENTIFIED")) || (i == 4 && strcmp(token, "BY")) || i > 5)
                    {
                        tanda = 1;
                        send(new_socket, "Syntax error!", 1024, 0);
                        break;
                    }
                }
                if (tanda)
                {
                    continue;
                }
                char command1[1024], command2[1024];
                sprintf(command2, "echo \"%s\" >> databases/database_template/user.csv", uname);
                sprintf(command1, "echo \"%s,%s\" >> databases/database_admin/user.csv", uname, upassword);
                system(command1);
                system(command2);
                send(new_socket, "User created successfully!", 1024, 0);
                continue;
            }
            else
            {
                send(new_socket, "You are not Root!", 1024, 0);
                continue;
            }
        }

        else if (!strncmp(buffer, "GRANT PERMISSION", 16))
        {
            if (isRoot)
            {
                char *ptr;
                char dbname[1024];
                char userid[1024];
                bzero(dbname, 1024);
                bzero(userid, 1024);

                int i;
                char parse[1024];
                strcpy(parse, buffer);
                char *parseptr = parse;
                char *token;
                int tanda = 0;
                for (i = 0; token = strtok_r(parseptr, " ", &parseptr); i++)
                {
                    if (i == 2)
                    {
                        strcpy(dbname, token);
                    }
                    else if (i == 4)
                    {
                        strncpy(userid, token, strlen(token) - 1);
                    }
                    else if (i == 3 && strcmp(token, "INTO"))
                    {
                        int tanda = 1;
                        break;
                    }
                }
                if (tanda)
                {
                    send(new_socket, "Syntax error!", 1024, 0);
                    continue;
                }

                FILE *file = fopen("databases/database_admin/user.csv", "r");
                char line[1024];
                char asli[1024];
                int found = 0;

                while (fgets(line, 1024, file))
                {

                    char *saved_username;
                    char *token = strtok_r(line, ",", &saved_username);

                    if (!strncmp(token, userid, strlen(userid)))
                    {
                        strcpy(asli, line);
                        found = 1;
                        break;
                    }
                }
                fclose(file);

                if (!found)
                {
                    send(new_socket, "User not found!", 1024, 0);
                    continue;
                }

                char folderPath[1024];
                sprintf(folderPath, "databases/%s", dbname);
                if (!folderExists(folderPath))
                {
                    send(new_socket, "Database does not exist!", 1024, 0);
                    continue;
                }
                else
                {
                    char command1[1024];
                    sprintf(command1, "echo %s >> databases/%s/user.csv", asli, dbname);
                    system(command1);
                    send(new_socket, "Access granted!", 1024, 0);
                    continue;
                }
            }
            else
            {
                send(new_socket, "You are not Root!", 1024, 0);
                continue;
            }
        }
        else if (!strncmp(buffer, "USE", 3))
        {
            if (isRoot)
            {
                char dbname[1024];
                bzero(dbname, 1024);
                char *dbptr = buffer + 4;
                strncpy(dbname, dbptr, strlen(dbptr) - 1);
                char folderPath[1024];
                sprintf(folderPath, "databases/%s", dbname);
                if (!folderExists(folderPath))
                {
                    send(new_socket, "Database does not exist!", 1024, 0);
                    continue;
                }
                else
                {
                    strcpy(currentDatabase, dbname);
                    char asli[1024];
                    sprintf(asli, "Database successfully accessed %s", dbname);
                    send(new_socket, asli, 1024, 0);
                    continue;
                }
            }
            else
            {
                char dbname[1024];
                bzero(dbname, 1024);
                char *dbptr = buffer + 4;
                strncpy(dbname, dbptr, strlen(dbptr) - 1);
                char folderPath[1024];
                sprintf(folderPath, "databases/%s", dbname);
                if (!folderExists(folderPath))
                {
                    send(new_socket, "Database does not exist!", 1024, 0);
                    continue;
                }
                else
                {
                    if (strcmp(dbname, "database_admin") == 0)
                    {
                        send(new_socket, "Access denied!", 1024, 0);
                        continue;
                    }

                    char command1[1024];
                    sprintf(command1, "databases/%s/user.csv", dbname);
                    FILE *file = fopen(command1, "r");
                    char line[1024];
                    int found = 0;
                    while (fgets(line, 1024, file))
                    {
                        if (!strncmp(line, username, strlen(username)))
                        {
                            found = 1;
                            break;
                        }
                    }
                    fclose(file);
                    if (found)
                    {
                        strcpy(currentDatabase, dbname);
                        char asli[1024];
                        sprintf(asli, "Database successfully accessed %s", dbname);
                        send(new_socket, asli, 1024, 0);
                        continue;
                    }
                    else
                    {
                        send(new_socket, "Access denied!", 1024, 0);
                        continue;
                    }
                }
            }
        }

        else if (!strncmp(buffer, "CREATE DATABASE", 15))
        {

            char dbname[1024];
            bzero(dbname, 1024);

            int i;
            char parse[1024];
            strcpy(parse, buffer);
            char *parseptr = parse;
            char *token;

            for (i = 0; token = strtok_r(parseptr, " ", &parseptr); i++)
            {
                if (i == 2)
                {
                    strncpy(dbname, token, strlen(token) - 1);
                }
            }
            char command1[1024], command2[1024], command3[1024], command4[1024];
            sprintf(command1, "mkdir databases/%s", dbname);
            system(command1);
            sprintf(command2, "touch databases/%s/user.csv", dbname);
            sprintf(command3, "echo \"username\" > databases/%s/user.csv", dbname);
            sprintf(command4, "echo %s >> databases/%s/user.csv", username, dbname);
            system(command2);
            system(command3);
            system(command4);
            send(new_socket, "Database successfully created!", 1024, 0);
            continue;
        }
        else if (!strncmp(buffer, "CREATE TABLE", 12))
        {
            // char buffer[1024] = "CREATE TABLE mahasiswa (nama string, nim int, nilai int)";
            char tableName[1024];
            int i = 0;
            char cmd[1024];
            strcpy(cmd, buffer);
            char *cmdptr = cmd;
            char *token;

            for (i = 0; token = strtok_r(cmdptr, " ", &cmdptr); i++)
            {
                if (i == 2)
                {
                    strcpy(tableName, token);
                }
            }
            strcpy(cmd, buffer);
            cmdptr = strchr(cmd, '(') + 1;
            cmd[strlen(cmd) - 1] = '\0';
            cmd[strlen(cmd) - 1] = '\0'; // Remove last character (')'

            strncpy(cmd, cmdptr, strlen(cmd));
            char temp[1024];
            strcpy(temp, cmd);

            char table_path[1024];
            sprintf(table_path, "databases/%s/%s.csv", currentDatabase, tableName);
            FILE *file = fopen(table_path, "w");
            char *token2;
            int j;
            // printf("%s\n", cmdptr);
            char *coba = temp;
            char fix[1024] = {0};
            // bzero(fix);

            for (i = 0; token = strtok_r(coba, ",", &coba); i++)
            {
                if (i == 0)
                {
                    char columnName[1024], dataType[1024];
                    char *columnptr = token;
                    for (j = 0; token2 = strtok_r(columnptr, " ", &columnptr); j++)
                    {
                        if (j == 0)
                        {
                            strcpy(columnName, token2);
                        }
                        if (j == 1)
                        {
                            strcpy(dataType, token2);
                        }
                    }
                    char command1[1024];
                    sprintf(command1, "%s:%s,", columnName, dataType);
                    strcat(fix, command1);
                }
                else
                {
                    char columnName[1024], dataType[1024];
                    char column[1024];
                    memmove(token, token + 1, strlen(token));
                    char *columnptr = token;
                    for (j = 0; token2 = strtok_r(columnptr, " ", &columnptr); j++)
                    {
                        if (j == 0)
                        {
                            strcpy(columnName, token2);
                        }
                        if (j == 1)
                        {
                            strcpy(dataType, token2);
                        }
                    }
                    char command1[1024];
                    sprintf(command1, "%s:%s,", columnName, dataType);
                    strcat(fix, command1);
                }
            }
            size_t length = strlen(fix);
            if (length > 0 && fix[length - 1] == ',')
            {
                strncpy(fix + length - 1, "\0", 1);
            }
            fprintf(file, fix);
            fclose(file);
            send(new_socket, "Table successfully created!", 1024, 0);
            continue;
        }
        else if (!strncmp(buffer, "DROP DATABASE", 13))
        {
            if (isRoot)
            {

                char dbname[1024];
                bzero(dbname, 1024);

                int i;
                char parse[1024];
                strcpy(parse, buffer);
                char *parseptr = parse;
                char *token;

                for (i = 0; token = strtok_r(parseptr, " ", &parseptr); i++)
                {
                    if (i == 2)
                    {
                        strncpy(dbname, token, strlen(token) - 1);
                    }
                }
                char folderPath[1024];
                sprintf(folderPath, "databases/%s", dbname);
                if (!folderExists(folderPath))
                {
                    send(new_socket, "Database does not exist!", 1024, 0);
                    continue;
                }
                else
                {
                    if (strcmp(currentDatabase, dbname) == 0)
                    {
                        send(new_socket, "You are currently using this database!", 1024, 0);
                        continue;
                    }
                    char command1[1024];
                    sprintf(command1, "rm -rf databases/%s", dbname);
                    system(command1);
                    send(new_socket, "Database successfully dropped!", 1024, 0);
                    continue;
                }
            }
            else
            {
                char dbname[1024];
                bzero(dbname, 1024);

                int i;
                char parse[1024];
                strcpy(parse, buffer);
                char *parseptr = parse;
                char *token;

                for (i = 0; token = strtok_r(parseptr, " ", &parseptr); i++)
                {
                    if (i == 2)
                    {
                        strncpy(dbname, token, strlen(token) - 1);
                    }
                }
                char folderPath[1024];
                sprintf(folderPath, "databases/%s", dbname);
                if (!folderExists(folderPath))
                {
                    send(new_socket, "Database does not exist!", 1024, 0);
                    continue;
                }
                else
                {
                    if (strcmp(currentDatabase, dbname) == 0)
                    {
                        send(new_socket, "You are currently using this database!", 1024, 0);
                        continue;
                    }
                    if (strcmp(dbname, "database_admin") == 0)
                    {
                        send(new_socket, "Access denied!", 1024, 0);
                        continue;
                    }
                    char command1[1024];
                    sprintf(command1, "databases/%s/user.csv", dbname);
                    FILE *file = fopen(command1, "r");
                    char line[1024];
                    int found = 0;
                    while (fgets(line, 1024, file))
                    {
                        if (!strncmp(line, username, strlen(username)))
                        {
                            found = 1;
                            break;
                        }
                    }
                    fclose(file);
                    if (found)
                    {
                        char command1[1024];
                        sprintf(command1, "rm -rf databases/%s", dbname);
                        system(command1);
                        send(new_socket, "Database successfully dropped!", 1024, 0);
                        continue;
                    }
                    else
                    {
                        send(new_socket, "Access denied!", 1024, 0);
                        continue;
                    }
                }
            }
        }
        else if (!strncmp(buffer, "DROP TABLE", 10))
        {
            char tableName[1024];
            int i = 0;
            char cmd[1024];
            strcpy(cmd, buffer);
            char *cmdptr = cmd;
            char *token;

            for (i = 0; token = strtok_r(cmdptr, " ", &cmdptr); i++)
            {
                if (i == 2)
                {
                    strcpy(tableName, token);
                }
            }
            size_t length = strlen(tableName);
            if (length > 0 && tableName[length - 1] == ';')
            {
                strncpy(tableName + length - 1, "\0", 1);
            }
            char folderPath[1024];
            sprintf(folderPath, "databases/%s/%s.csv", currentDatabase, tableName);
            // send(new_socket, folderPath, 1024, 0);
            // continue;
            if (!fileExists(folderPath))
            {
                send(new_socket, "Table does not exist!", 1024, 0);
                continue;
            }
            else
            {
                char command1[1024];
                sprintf(command1, "rm databases/%s/%s.csv", currentDatabase, tableName);
                system(command1);
                send(new_socket, "Table successfully dropped!", 1024, 0);
                continue;
            }
        }
        else if (!strncmp(buffer, "DROP COLUMN", 11))
        {
            char tableName[1024], colomtarget[1024];
            int colom_target;
            int i = 0;
            char cmd[1024];
            strcpy(cmd, buffer);
            char *cmdptr = cmd;
            char *token;

            for (i = 0; token = strtok_r(cmdptr, " ", &cmdptr); i++)
            {
                if (i == 2)
                {
                    strcpy(colomtarget, token);
                }
                if (i == 4)
                {
                    strcpy(tableName, token);
                }
            }
            size_t length = strlen(tableName);
            if (length > 0 && tableName[length - 1] == ';')
            {
                strncpy(tableName + length - 1, "\0", 1);
            }
            char folderPath[1024];
            sprintf(folderPath, "databases/%s/%s.csv", currentDatabase, tableName);
            // send(new_socket, folderPath, 1024, 0);
            // continue;
            if (!fileExists(folderPath))
            {
                send(new_socket, "Table does not exist!", 1024, 0);
                continue;
            }
            else
            {
                char command2[1024];
                sprintf(command2, "databases/%s/%s.csv", currentDatabase, tableName);
                FILE *file = fopen(command2, "r");
                char asli[1024] = {0};
                char line[1024];
                while (fgets(line, 1024, file))
                {
                    strcat(asli, line);
                    break;
                }
                fclose(file);
                char *cmdptr2 = asli;
                char *token2;
                int tanda = 0;
                for (i = 0; token2 = strtok_r(cmdptr2, ",", &cmdptr2); i++)
                {
                    char *cmdptr3 = token2;
                    char *token3;
                    for (int j = 0; token3 = strtok_r(cmdptr3, ":", &cmdptr3); j++)
                    {
                        if (strcmp(token3, colomtarget) == 0)
                        {
                            colom_target = i + 1;
                            tanda = 1;
                            break;
                        }
                    }
                    if (tanda)
                    {
                        break;
                    }
                }
                if (!tanda)
                {
                    send(new_socket, "Column does not exist!", 1024, 0);
                    continue;
                }
                char command1[1024], command3[1024], command4[1024];
                sprintf(command1, "cut --complement -f %d -d, databases/%s/%s.csv >> databases/%s/output.csv", colom_target, currentDatabase, tableName, currentDatabase);
                system(command1);
                sprintf(command3, "rm databases/%s/%s.csv", currentDatabase, tableName);
                system(command3);
                sprintf(command4, "mv databases/%s/output.csv databases/%s/%s.csv", currentDatabase, currentDatabase, tableName);
                system(command4);
                send(new_socket, "Table successfully dropped!", 1024, 0);
                continue;
            }
        }

        // Todo poin D
        else if (!strncmp(buffer, "INSERT INTO", 11))
        {
            char tableName[1024];
            char values[1024];
            int i = 0;
            char cmd[1024];
            strcpy(cmd, buffer);
            char *cmdptr = cmd;
            char *token;

            for (i = 0; token = strtok_r(cmdptr, " ", &cmdptr); i++)
            {
                if (i == 2)
                {
                    strcpy(tableName, token);
                }
            }
            strcpy(cmd, buffer);
            cmdptr = strchr(cmd, '(') + 1;
            cmd[strlen(cmd) - 1] = '\0';
            cmd[strlen(cmd) - 1] = '\0'; // Remove last character (')')

            strncpy(values, cmdptr, strlen(cmd));
            char table_path[1024];
            sprintf(table_path, "databases/%s/%s.csv", currentDatabase, tableName);
            FILE *file = fopen(table_path, "a");

            int j = 0;
            for (int i = 0; i < strlen(values); i++)
            {
                if (values[i] != '\'' && values[i] != ' ')
                {
                    values[j] = values[i];
                    j++;
                }
            }
            values[j] = '\0';

            fprintf(file, "\n%s", values);
            fclose(file);
            send(new_socket, "Data successfully inserted!", 1024, 0);
            continue;
        }
        else if (!strncmp(buffer, "UPDATE", 6))
        {
            char tableName[1024];
	    char columnName[1024];
	    char value[1024];
	    char conditionColumnName[1024];
	    char conditionValue[1024];
	    int i = 0;
	    char cmd[1024];
	    strcpy(cmd, buffer);
	    char *cmdptr = cmd;
	    char *token;
	    int w = 0;

            for (i = 0; token = strtok_r(cmdptr, " ", &cmdptr); i++)
            {
                if (i == 1)
                {
                    strcpy(tableName, token);
                }
                else if (i == 3)
                {
                	w=0;
                    char *colPtr = strtok(token, "=");
                    strcpy(columnName, colPtr);

                    char *valuePtr = strtok(NULL, "=");
                    if (valuePtr != NULL)
                    {
                        int len = strlen(valuePtr);
                        if (len > 0 && valuePtr[0] == '\'')
                        {
                            memmove(valuePtr, valuePtr + 1, len - 1); // Menghapus tanda petik tunggal di awal
                            len--;
                        }
                        if (len > 0 && valuePtr[len - 1] == ';')
                        {
                            valuePtr[len - 1] = '\0'; // Menghapus titik koma di akhir
                            len--;
                        }
                        if (len > 0 && valuePtr[len - 1] == '\'')
                        {
                            valuePtr[len - 1] = '\0'; // Menghapus tanda petik tunggal di akhir
                            len--;
                        }
                        strcpy(value, valuePtr);
                    }
                }
                else if (i == 5)
		{
			w=1;
		    char *conditionColPtr = strtok(token, "=");
		    strcpy(conditionColumnName, conditionColPtr);

		    char *conditionValuePtr = strtok(NULL, "=");
		    if (conditionValuePtr != NULL)
		    {
		        int len = strlen(conditionValuePtr);
		        if (len > 0 && conditionValuePtr[0] == '\'')
		        {
		            memmove(conditionValuePtr, conditionValuePtr + 1, len - 1); // Menghapus tanda petik tunggal di awal
		            len--;
		        }
		        if (len > 0 && conditionValuePtr[len - 1] == ';')
		        {
		            conditionValuePtr[len - 1] = '\0'; // Menghapus titik koma di akhir
		            len--;
		        }
		        if (len > 0 && conditionValuePtr[len - 1] == '\'')
		        {
		            conditionValuePtr[len - 1] = '\0'; // Menghapus tanda petik tunggal di akhir
		            len--;
		        }
		        strcpy(conditionValue, conditionValuePtr);
		    }
		}
            }

            char table_path[1024];
            sprintf(table_path, "databases/%s/%s.csv", currentDatabase, tableName);
            FILE *file = fopen(table_path, "r");
            char tempPath[1024];
            sprintf(tempPath, "databases/%s/temp.csv", currentDatabase);
            FILE *tempFile = fopen(tempPath, "w");

            if (file == NULL)
            {
                send(new_socket, "Table not found!", 1024, 0);
                continue;
            }

            // Membaca header kolom dari file CSV dan menuliskannya ke file temporary
            char header[1024];
            fgets(header, sizeof(header), file);
            fprintf(tempFile, "%s", header);

            // Mendapatkan indeks kolom yang akan diupdate
            int targetColumnIndex = -1;
	    int conditionColumnIndex = -1;
	    char *columnPtr = strtok(header, ",");
	    int columnIndex = 0;
	    if(w==1){
	    while (columnPtr != NULL)
	    {
            // Menghapus whitespace pada nama kolom
		char *trimmedColumn = columnPtr;
		while (*trimmedColumn == ' ' || *trimmedColumn == '\t')
		{
		    trimmedColumn++;
		}

		char *colonPtr = strchr(trimmedColumn, ':');
		if (colonPtr != NULL)
		{
		    *colonPtr = '\0'; // Menghapus tipe data kolom setelah ':'
		}
        	//printf("trimmedColumn : %s\n", trimmedColumn);  
		if (strcmp(trimmedColumn, columnName) == 0)
		{
		    targetColumnIndex = columnIndex;
		}
		if (strcmp(trimmedColumn, conditionColumnName) == 0)
		{
		    conditionColumnIndex = columnIndex;
		}
        		
		if (targetColumnIndex != -1 && conditionColumnIndex != -1)
		{
		    break;
		}
		columnPtr = strtok(NULL, ",");
		columnIndex++;
	    
		//printf("targetColumnIndex : %d\n", targetColumnIndex);
		//printf("columnIndex : %d\n", columnIndex);
		//printf("conditionColumnIndex : %d\n", conditionColumnIndex);
		//printf("trimmedColumn : %s\n", trimmedColumn);
		}

            if (targetColumnIndex == -1)
            {
                send(new_socket, "Column not found!", 1024, 0);
                fclose(file);
                fclose(tempFile);
                remove(tempPath);
                continue;
            }

            // Menyalin data dari file CSV ke file temporary dengan melakukan update pada kolom yang ditentukan
            char line[1024];
	    while (fgets(line, sizeof(line), file))
	    {
		char *tokenPtr = strtok(line, ",");
		columnIndex = 0;
		int conditionSatisfied = 0; // Menandakan apakah kondisi terpenuhi pada baris ini
		while (tokenPtr != NULL)
		{
		    // Menghapus whitespace pada nilai dalam kolom
		    char *trimmedToken = tokenPtr;
		    while (*trimmedToken == ' ' || *trimmedToken == '\t')
		    {
		        trimmedToken++;
		    }
		    

		    if (columnIndex == conditionColumnIndex)
		    {
		        // Memeriksa apakah nilai kolom kondisi sama dengan nilai kondisi yang diinginkan
		        if (strcmp(trimmedToken, conditionValue) == 0)
		        {
		            conditionSatisfied = 1; // Kondisi terpenuhi, perlu mengupdate kolom target
		        }
		        else
		        {
		            conditionSatisfied = 0; // Kondisi tidak terpenuhi, tidak perlu mengupdate kolom target
		            
		        }
		    }
        		//printf("conditionSatisfied : %d\n", conditionSatisfied);  
        		//printf("conditionColumnIndex : %d\n", conditionColumnIndex);  
        		//printf("columnIndex : %d\n", columnIndex);  
        		//printf("trimmedToken : %s\n\n", trimmedToken);  
        		
        		//printf("targetColumnIndex : %d\n", targetColumnIndex);  
		    if (columnIndex == targetColumnIndex && conditionSatisfied)
		    {
		        fprintf(tempFile, "%s,", value); // Mengganti nilai dengan value yang baru		
			
			//printf("value : %s\n", value);  
		    }
		    else
		    {
		        fprintf(tempFile, "%s,", trimmedToken); // Menyalin nilai kolom yang lain tanpa perubahan
		    }
		    tokenPtr = strtok(NULL, ",");
		    columnIndex++;
		}
		fseek(tempFile, -1, SEEK_CUR); // Menghapus trailing comma pada setiap baris
		fprintf(tempFile, "\n");
	    }
	    
	}
	else if (w == 0) {
		while (columnPtr != NULL)
		    {
		        // Menghapus whitespace pada nama kolom
		        char *trimmedColumn = columnPtr;
		        while (*trimmedColumn == ' ' || *trimmedColumn == '\t')
		        {
		            trimmedColumn++;
		        }

		        char *colonPtr = strchr(trimmedColumn, ':');
		        if (colonPtr != NULL)
		        {
		            *colonPtr = '\0'; // Menghapus tipe data kolom setelah ':'
		        }

		        if (strcmp(trimmedColumn, columnName) == 0)
		        {
		            targetColumnIndex = columnIndex;
		            // char output[1024];
		            // sprintf(output, "Sedang membandingkan kolom: %s", trimmedColumn);
		            // send(new_socket, output, 1024, 0); // Mengirim nama kolom yang sedang dibandingkan
		            break;
		        }
		        columnPtr = strtok(NULL, ",");
		        columnIndex++;
		    }

		    if (targetColumnIndex == -1)
		    {
		        send(new_socket, "Column not found!", 1024, 0);
		        fclose(file);
		        fclose(tempFile);
		        remove(tempPath);
		        continue;
		    }

		    // Menyalin data dari file CSV ke file temporary dengan melakukan update pada kolom yang ditentukan
		    char line[1024];
		    while (fgets(line, sizeof(line), file))
		    {
		        char *tokenPtr = strtok(line, ",");
		        columnIndex = 0;
		        while (tokenPtr != NULL)
		        {
		            // Menghapus whitespace pada nilai dalam kolom
		            char *trimmedToken = tokenPtr;
		            while (*trimmedToken == ' ' || *trimmedToken == '\t')
		            {
		                trimmedToken++;
		            }

		            if (columnIndex == targetColumnIndex)
		            {
		                fprintf(tempFile, "%s,", value); // Mengganti nilai dengan value yang baru
		            }
		            else
		            {
		                fprintf(tempFile, "%s,", trimmedToken); // Menyalin nilai kolom yang lain tanpa perubahan
		            }
		            tokenPtr = strtok(NULL, ",");
		            columnIndex++;
		        }
		        fseek(tempFile, -1, SEEK_CUR); // Menghapus trailing comma pada setiap baris
		        fprintf(tempFile, "\n");
		    }

	
	}
            fclose(file);
            fclose(tempFile);

            // Mengganti file CSV asli dengan file temporary yang sudah diupdate
            remove(table_path);
            rename(tempPath, table_path);

            // Menghilangkan baris kosong antara setiap baris
            FILE *finalFile = fopen(table_path, "r");
            fseek(finalFile, 0, SEEK_END);
            long fileLength = ftell(finalFile);
            fseek(finalFile, 0, SEEK_SET);
            char *fileContent = malloc(fileLength + 1);
            fread(fileContent, 1, fileLength, finalFile);
            fileContent[fileLength] = '\0';
            fclose(finalFile);

            char *linePtr = strtok(fileContent, "\n");
            FILE *outputFile = fopen(table_path, "w");
            while (linePtr != NULL)
            {
                if (strlen(linePtr) > 0)
                {
                    fprintf(outputFile, "%s\n", linePtr);
                }
                linePtr = strtok(NULL, "\n");
            }
            fclose(outputFile);
            free(fileContent);

            send(new_socket, "Data successfully updated!", 1024, 0);
            continue;
        }
        else if (!strncmp(buffer, "DELETE FROM", 11))
        {
            char tableName[1024];
            int i = 0;
            char cmd[1024];
            strcpy(cmd, buffer);
            char *cmdptr = cmd;
            char *token;
            char whereColumnName[1024];
            char whereCondition[1024];
            int deleteWhere = 0;

            for (i = 0; token = strtok_r(cmdptr, " ", &cmdptr); i++)
            {
                if (i == 2)
                {
                    strcpy(tableName, token);
                }
                else if (i == 4)
                {
                    deleteWhere = 1;
                    char *colWherePtr = strtok(token, "=");
                    strcpy(whereColumnName, colWherePtr);

                    char *valueWherePtr = strtok(NULL, "=");
                    // Menghapus tanda petik tunggal di awal dan akhir kondisi WHERE jika ada
                    int len = strlen(valueWherePtr);
                    if (len > 0 && valueWherePtr[0] == '\'')
                    {
                        memmove(valueWherePtr, valueWherePtr + 1, len - 1);
                        len--;
                    }
                    if (len > 0 && valueWherePtr[len - 1] == ';')
                    {
                        valueWherePtr[len - 1] = '\0';
                        len--;
                    }
                    if (len > 0 && valueWherePtr[len - 1] == '\'')
                    {
                        valueWherePtr[len - 1] = '\0';
                        len--;
                    }
                    strcpy(whereCondition, valueWherePtr);
                }
            }

            size_t length = strlen(tableName);
            if (length > 0 && tableName[length - 1] == ';')
            {
                strncpy(tableName + length - 1, "\0", 1);
            }
            char filePath[1024];
            sprintf(filePath, "databases/%s/%s.csv", currentDatabase, tableName);

            FILE *file = fopen(filePath, "r");
            if (file == NULL)
            {
                send(new_socket, "Table does not exist!", 1024, 0);
                continue;
            }

            // Baca baris header dan simpan di file temporary
            char header[1024];
            fgets(header, sizeof(header), file);
            FILE *tempFile = fopen("temp.csv", "w");
            fprintf(tempFile, "%s", header);

            // Hapus baris yang memenuhi kondisi WHERE (jika ada)
            if (deleteWhere == 1)
            {
                char row[1024];
                int whereColumnIndex = -1;
                char *columnPtr = strtok(header, ",");
                int columnIndex = 0;
                char message[1024];

                while (columnPtr != NULL)
                {
                    // Menghapus whitespace pada nama kolom
                    char *trimmedColumn = columnPtr;
                    while (*trimmedColumn == ' ' || *trimmedColumn == '\t')
                    {
                        trimmedColumn++;
                    }
                    char *colonPtr = strchr(trimmedColumn, ':');
                    if (colonPtr != NULL)
                    {
                        *colonPtr = '\0'; // Menghapus tipe data kolom setelah ':'
                    }

                    if (strcmp(trimmedColumn, whereColumnName) == 0)
                    {
                        whereColumnIndex = columnIndex;
                        // sprintf(message, "Invalid WHERE condition! Column %s index %d ptr %s condition %s does not exist.", trimmedColumn,whereColumnIndex, whereCondition);
                        break;
                    }
                    columnPtr = strtok(NULL, ",");
                    columnIndex++;
                }

                if (whereColumnIndex == -1 && strcmp(whereColumnName, "") != 0)
                {
                    fclose(file);
                    fclose(tempFile);

                    sprintf(message, "Invalid WHERE condition! Column %s does not exist.", whereColumnName);
                    send(new_socket, message, 1024, 0);
                    continue;
                }

                int rowsDeleted = 0; // Jumlah baris yang dihapus

                while (fgets(row, sizeof(row), file))
                {
                    // Cek apakah baris memenuhi kondisi WHERE
                    char *valuePtr = strtok(row, ",");
                    int colIndex = 0;
                    int w = 0;
                    char columnValues[1024][1024];
                    while (valuePtr != NULL)
                    {
                        if (colIndex == whereColumnIndex)
                        {
                            // Menghapus whitespace pada nilai kolom
                            char *trimmedValue = valuePtr;
                            while (*trimmedValue == ' ' || *trimmedValue == '\t')
                            {
                                trimmedValue++;
                            }
                            if (strcmp(trimmedValue, whereCondition) == 0)
                            {
                                w = 1; // Baris memenuhi kondisi WHERE
                                break;
                            }
                        }
                        strcpy(columnValues[colIndex], valuePtr);

                        valuePtr = strtok(NULL, ",");
                        colIndex++;
                    }
                    if (!w)
                    {
                        // Menyalin baris ke file temporary jika tidak memenuhi kondisi WHERE
                        for (int i = 0; i < colIndex; i++)
                        {
                            fprintf(tempFile, "%s", columnValues[i]);
                            if (i < colIndex - 1)
                                fprintf(tempFile, ",");
                        }
                        fprintf(tempFile, "");
                    }
                    else
                    {
                        rowsDeleted++; // Baris terhapus jika memenuhi kondisi WHERE
                    }
                }

                fclose(file);
                fclose(tempFile);

                // Mengganti file asli dengan file temporary
                remove(filePath);
                rename("temp.csv", filePath);

                if (rowsDeleted > 0)
                {
                    char message[1024];
                    sprintf(message, "Deleted %d row(s)!", rowsDeleted);
                    send(new_socket, message, 1024, 0);
                }
                else
                {
                    send(new_socket, "No rows deleted!", 1024, 0);
                }
                continue;
            }
            else
            {
                fclose(file);
                fclose(tempFile);
                send(new_socket, "Delete where successfully!", 1024, 0);


                // Mengganti file asli dengan file temporary
                remove(filePath);
                rename("temp.csv", filePath);
            }
        }
        else if (!strncmp(buffer, "SELECT", 6))
        {
            char str[1024] = {0};
            strcpy(str, buffer);

            // // Mengirim header kolom ke klien
            // char header[1024];
            // fgets(header, sizeof(header), file);
            // send(new_socket, header, 1024, 0);

            // Memeriksa apakah harus mengirim semua kolom atau hanya kolom tertentu
            if (strstr(str, "WHERE") == NULL)
            {
                char tableName[1024];
                char columns[1024];
                int i = 0;
                char cmd[1024];
                strcpy(cmd, buffer);
                char cek_where[1024] = {0};
                char *cmdptr = cmd;
                char *token;
                char whereColumnName[1024] = {0};
                char whereCondition[1024] = {0};

                for (i = 0; token = strtok_r(cmdptr, " ", &cmdptr); i++)
                {
                    if (i == 1)
                    {
                        strcpy(columns, token);
                    }
                    else if (i == 3)
                    {
                        strcpy(tableName, token);
                    }
                }

                size_t length = strlen(tableName);
                if (length > 0 && tableName[length - 1] == ';')
                {
                    strncpy(tableName + length - 1, "\0", 1);
                }

                if (strcmp(columns, "*") == 0)
                {
                    char table_path[1024];
                    sprintf(table_path, "databases/%s/%s.csv", currentDatabase, tableName);
                    FILE *file = fopen(table_path, "r");

                    if (file == NULL)
                    {
                        send(new_socket, "Table not found!", 1024, 0);
                        continue;
                    }
                    // Mengirim data baris per baris ke klien
                    char hasil_select[10001] = {0};
                    char line[10001];
                    while (fgets(line, sizeof(line), file))
                    {

                        strcat(hasil_select, line);
                    }

                    fclose(file);
                    send(new_socket, hasil_select, 10001, 0);
                    continue;
                }
                else
                {

                    char new_tableName[1024] = {0};
                    char query[1024] = {0};
                    strcpy(query, buffer);

                    char *fromPos = strstr(query, "FROM");
                    if (fromPos != NULL)
                    {
                        // Move the pointer to the beginning of the table name
                        fromPos += strlen("FROM");

                        // Skip any leading whitespace characters
                        while (*fromPos == ' ' || *fromPos == '\t')
                        {
                            fromPos++;
                        }

                        // Extract the table name

                        sscanf(fromPos, "%s", new_tableName);

                        // printf("Table Name: %s\n", new_tableName);
                    }
                    size_t length_bug = strlen(new_tableName);
                    if (length_bug > 0 && new_tableName[length_bug - 1] == ';')
                    {
                        strncpy(new_tableName + length_bug - 1, "\0", 1);
                    }

                    char table_path[1024];
                    sprintf(table_path, "databases/%s/%s.csv", currentDatabase, new_tableName);

                    FILE *file = fopen(table_path, "r");

                    if (file == NULL)
                    {
                        send(new_socket, "Table not found!", 1024, 0);
                        continue;
                    }

                    char sql_query[1024] = {0};
                    strcpy(sql_query, buffer);
                    char *start_pos = strstr(sql_query, "SELECT ") + strlen("SELECT ");
                    char *end_pos = strstr(start_pos, " FROM");
                    int length = end_pos - start_pos;
                    char columns_string[length + 1];

                    if (start_pos != NULL && end_pos != NULL)
                    {
                        strncpy(columns_string, start_pos, length);
                        columns_string[length] = '\0';
                    }
                    char *columns_bug[MAX_COLUMNS];
                    const char *delimiter = ", ";
                    int columnCount = 0;

                    // Tokenize the columns_string string
                    char *token_bug = strtok(columns_string, delimiter);
                    while (token_bug != NULL && columnCount < MAX_COLUMNS)
                    {
                        columns_bug[columnCount] = token_bug;
                        columnCount++;

                        token_bug = strtok(NULL, delimiter);
                    }

                    Coba11 coba[columnCount + 1];

                    for (int i = 0; i < columnCount; i++)
                    {
                        // bzero(coba[i].name, 1024);
                        strcpy(coba[i].name, columns_bug[i]);
                        coba[i].tanda = 0;
                        coba[i].hasil = 0;
                        // printf("Column %d: %s\n", i + 1, columns_bug[i]);
                    }

                    // print all struct elements
                    for (int i = 0; i < columnCount; i++)
                    {
                        // printf("Column %d: %s\n", i + 1, coba[i].name);
                    }

                    char asli[1024] = {0};
                    char line[1024];
                    while (fgets(line, sizeof(line), file))
                    {
                        strcpy(asli, line);
                        break;
                    }
                    fclose(file);
                    char *cmdptr2 = asli;
                    char *token2 = {0};
                    for (int i = 0; token2 = strtok_r(cmdptr2, ",", &cmdptr2); i++)
                    {
                        char *cmdptr3 = token2;
                        char *token3 = {0};
                        for (int j = 0; token3 = strtok_r(cmdptr3, ":", &cmdptr3); j++)
                        {
                            for (int z = 0; z < columnCount; z++)
                            {
                                if (strcmp(coba[z].name, token3) == 0)
                                {
                                    coba[z].tanda = 1;
                                    coba[z].hasil = i + 1;
                                }
                            }
                        }
                    }
                    int tanda2 = 0;
                    for (int i = 0; i < columnCount; i++)
                    {
                        if (coba[i].tanda == 0)
                        {
                            tanda2 = 1;
                            break;
                        }
                    }
                    if (tanda2)
                    {
                        send(new_socket, "Column not found!", 1024, 0);
                        continue;
                    }
                    // char command10[1024];
                    // sprintf(command10, "databases/%s/temp.csv", currentDatabase, new_tableName);

                    char hasil_fix[1024] = {0};
                    char awalan[1024] = {"awk -F \',\' \'{print "};
                    char tengah[1024] = {0};
                    char akhiran[1024] = {0};
                    sprintf(akhiran, "}\' databases/%s/%s.csv > databases/%s/temp.csv", currentDatabase, new_tableName, currentDatabase);
                    for (int i = 0; i < columnCount; i++)
                    {
                        if (coba[i].tanda == 1)
                        {
                            char str[20];
                            sprintf(str, "%d", coba[i].hasil);
                            strcat(tengah, "$");
                            strcat(tengah, str);
                            strcat(tengah, ",");
                        }
                    }
                    tengah[strlen(tengah) - 1] = '\0';
                    strcat(hasil_fix, awalan);
                    strcat(hasil_fix, tengah);
                    strcat(hasil_fix, akhiran);

                    system(hasil_fix);

                    char temp_path[1024];
                    sprintf(temp_path, "databases/%s/temp.csv", currentDatabase);
                    FILE *file2 = fopen(temp_path, "r");
                    char hasil_select[10001] = {0};
                    char line2[10001];
                    while (fgets(line2, sizeof(line2), file))
                    {
                        strcat(hasil_select, line2);
                    }
                    fclose(file);
                    char command11[1024];
                    sprintf(command11, "rm databases/%s/temp.csv", currentDatabase);
                    system(command11);
                    send(new_socket, hasil_select, 10001, 0);
                    continue;
                }
            }
            else
            {
                char colum_tanda[1024];
                char cmd232[1024];
                strcpy(cmd232, buffer);
                char *cmdptr232 = cmd232;
                char *token232;
                for (int i = 0; token232 = strtok_r(cmdptr232, " ", &cmdptr232); i++)
                {
                    if (i == 1)
                    {
                        strcpy(colum_tanda, token232);
                    }
                }

                if (strcmp(colum_tanda, "*") == 0)
                {
                    char tableName[1024];
                    char columns[1024];
                    char cmd[1024];
                    strcpy(cmd, buffer);
                    char cek_where[1024] = {0};
                    char *cmdptr = cmd;
                    char *token;
                    char whereColumnName[1024] = {0};
                    char whereCondition[1024] = {0};

                    for (int i = 0; token = strtok_r(cmdptr, " ", &cmdptr); i++)
                    {
                        if (i == 1)
                        {
                            strcpy(columns, token);
                        }
                        else if (i == 3)
                        {
                            strcpy(tableName, token);
                        }
                        else if (i == 4)
                        {
                            strcpy(cek_where, token);
                        }
                        else if (i == 5)
                        {
                            // deleteWhere = 1;
                            char *colWherePtr = strtok(token, "=");
                            strcpy(whereColumnName, colWherePtr);

                            char *valueWherePtr = strtok(NULL, "=");
                            // Menghapus tanda petik tunggal di awal dan akhir kondisi WHERE jika ada
                            int len = strlen(valueWherePtr);
                            if (len > 0 && valueWherePtr[0] == '\'')
                            {
                                memmove(valueWherePtr, valueWherePtr + 1, len - 1);
                                len--;
                            }
                            if (len > 0 && valueWherePtr[len - 1] == ';')
                            {
                                valueWherePtr[len - 1] = '\0';
                                len--;
                            }
                            if (len > 0 && valueWherePtr[len - 1] == '\'')
                            {
                                valueWherePtr[len - 1] = '\0';
                                len--;
                            }
                            strcpy(whereCondition, valueWherePtr);
                        }
                    }

                    size_t length = strlen(tableName);
                    if (length > 0 && tableName[length - 1] == ';')
                    {
                        strncpy(tableName + length - 1, "\0", 1);
                    }
                    char table_path[1024];
                    sprintf(table_path, "databases/%s/%s.csv", currentDatabase, tableName);
                    FILE *file = fopen(table_path, "r");

                    if (file == NULL)
                    {
                        send(new_socket, "Table not found!", 1024, 0);
                        continue;
                    }
                    char asli[1024] = {0};
                    char line[1024];
                    while (fgets(line, sizeof(line), file))
                    {
                        strcpy(asli, line);
                        break;
                    }
                    fclose(file);
                    int tanda = 1;
                    int index = 0;
                    char *cmdptr2 = asli;
                    char *token2 = {0};
                    for (int i = 0; token2 = strtok_r(cmdptr2, ",", &cmdptr2); i++)
                    {
                        char *cmdptr3 = token2;
                        char *token3 = {0};
                        for (int j = 0; token3 = strtok_r(cmdptr3, ":", &cmdptr3); j++)
                        {
                            if (strcmp(whereColumnName, token3) == 0)
                            {
                                tanda = 0;
                                index = i + 1;
                                break;
                            }
                        }
                        if (tanda == 0)
                        {
                            break;
                        }
                    }
                    if (tanda)
                    {
                        send(new_socket, "Column not found!", 1024, 0);
                        continue;
                    }
                    char command10[1024];
                    sprintf(command10, "awk -F \',\' \'$%d == \"%s\"\' databases/%s/%s.csv > databases/%s/temp.csv", index, whereCondition, currentDatabase, tableName, currentDatabase);
                    system(command10);
                    char temp_path2[1024];
                    sprintf(temp_path2, "databases/%s/temp.csv", currentDatabase);
                    FILE *file2 = fopen(temp_path2, "r");
                    char hasil_select[10001] = {0};
                    char line2[10001];
                    while (fgets(line2, sizeof(line2), file))
                    {
                        strcat(hasil_select, line2);
                    }
                    fclose(file);
                    char command11[1024];
                    sprintf(command11, "rm databases/%s/temp.csv", currentDatabase);
                    system(command11);
                    send(new_socket, hasil_select, 10001, 0);
                    continue;
                }

                else
                {

                    char new_tableName[1024] = {0};
                    char query[1024] = {0};
                    strcpy(query, buffer);

                    char *fromPos = strstr(query, "FROM");
                    if (fromPos != NULL)
                    {
                        // Move the pointer to the beginning of the table name
                        fromPos += strlen("FROM");

                        // Skip any leading whitespace characters
                        while (*fromPos == ' ' || *fromPos == '\t')
                        {
                            fromPos++;
                        }

                        // Extract the table name

                        sscanf(fromPos, "%s", new_tableName);

                        // printf("Table Name: %s\n", new_tableName);
                    }
                    size_t length_bug = strlen(new_tableName);
                    if (length_bug > 0 && new_tableName[length_bug - 1] == ';')
                    {
                        strncpy(new_tableName + length_bug - 1, "\0", 1);
                    }
                    char table_path[1024];
                    sprintf(table_path, "databases/%s/%s.csv", currentDatabase, new_tableName);

                    FILE *file = fopen(table_path, "r");

                    if (file == NULL)
                    {
                        send(new_socket, "Table not found!", 1024, 0);
                        continue;
                    }

                    char sql_query[1024] = {0};
                    strcpy(sql_query, buffer);
                    char *start_pos = strstr(sql_query, "SELECT ") + strlen("SELECT ");
                    char *end_pos = strstr(start_pos, " FROM");
                    int length = end_pos - start_pos;
                    char columns_string[length + 1];

                    if (start_pos != NULL && end_pos != NULL)
                    {
                        strncpy(columns_string, start_pos, length);
                        columns_string[length] = '\0';
                    }

                    char target = ',';
                    char temp11[1024] = {0};
                    strcpy(temp11, columns_string);
                    int banyak_coma = countOccurrences(temp11, target);

                    char kasus4_tableName[1024];
                    char kauss4_columns[1024];
                    char kasus4_cmd[1024];
                    strcpy(kasus4_cmd, buffer);
                    char cek_where[1024] = {0};
                    char *kasus4_cmdptr = kasus4_cmd;
                    char *kasus4_token;
                    char kasus4_whereColumnName[1024] = {0};
                    char kasus4_whereCondition[1024] = {0};

                    for (int i = 0; kasus4_token = strtok_r(kasus4_cmdptr, " ", &kasus4_cmdptr); i++)
                    {
                        if (i == 1)
                        {
                            strcpy(kauss4_columns, kasus4_token);
                        }
                        else if (i == 3)
                        {
                            strcpy(kasus4_tableName, kasus4_token);
                        }
                        else if (i == 5 + banyak_coma)
                        {
                            // deleteWhere = 1;
                            char *kasus4_colWherePtr = strtok(kasus4_token, "=");
                            strcpy(kasus4_whereColumnName, kasus4_colWherePtr);

                            char *kasus4_valueWherePtr = strtok(NULL, "=");
                            // Menghapus tanda petik tunggal di awal dan akhir kondisi WHERE jika ada
                            int kasus4_len = strlen(kasus4_valueWherePtr);
                            if (kasus4_len > 0 && kasus4_valueWherePtr[0] == '\'')
                            {
                                memmove(kasus4_valueWherePtr, kasus4_valueWherePtr + 1, kasus4_len - 1);
                                kasus4_len--;
                            }
                            if (kasus4_len > 0 && kasus4_valueWherePtr[kasus4_len - 1] == ';')
                            {
                                kasus4_valueWherePtr[kasus4_len - 1] = '\0';
                                kasus4_len--;
                            }
                            if (kasus4_len > 0 && kasus4_valueWherePtr[kasus4_len - 1] == '\'')
                            {
                                kasus4_valueWherePtr[kasus4_len - 1] = '\0';
                                kasus4_len--;
                            }
                            strcpy(kasus4_whereCondition, kasus4_valueWherePtr);
                        }
                    }

                    char asli_buf1[1024] = {0};
                    char line_buf1[1024];
                    while (fgets(line_buf1, sizeof(line_buf1), file))
                    {
                        strcpy(asli_buf1, line_buf1);
                        break;
                    }
                    fclose(file);
                    int tanda_buf1 = 1;
                    int index_buf1 = 0;
                    char *cmdptr2_buf1 = asli_buf1;
                    char *token2_buf1 = {0};
                    for (int i = 0; token2_buf1 = strtok_r(cmdptr2_buf1, ",", &cmdptr2_buf1); i++)
                    {
                        char *cmdptr3_buf1 = token2_buf1;
                        char *token3_buf1 = {0};
                        for (int j = 0; token3_buf1 = strtok_r(cmdptr3_buf1, ":", &cmdptr3_buf1); j++)
                        {
                            if (strcmp(kasus4_whereColumnName, token3_buf1) == 0)
                            {
                                tanda_buf1 = 0;
                                index_buf1 = i + 1;
                                break;
                            }
                        }
                        if (tanda_buf1 == 0)
                        {
                            break;
                        }
                    }
                    if (tanda_buf1)
                    {
                        send(new_socket, "Column not found!", 1024, 0);
                        continue;
                    }

                    char *columns_bug[MAX_COLUMNS];
                    const char *delimiter = ", ";
                    int columnCount = 0;

                    // // Tokenize the columns_string string
                    char *token_bug = strtok(columns_string, delimiter);
                    while (token_bug != NULL && columnCount < MAX_COLUMNS)
                    {
                        columns_bug[columnCount] = token_bug;
                        columnCount++;

                        token_bug = strtok(NULL, delimiter);
                    }

                    Coba11 coba[columnCount + 1];

                    for (int i = 0; i < columnCount; i++)
                    {
                        // bzero(coba[i].name, 1024);
                        strcpy(coba[i].name, columns_bug[i]);
                        coba[i].tanda = 0;
                        coba[i].hasil = 0;
                        // printf("Column %d: %s\n", i + 1, columns_bug[i]);
                    }

                    // // // print all struct elements
                    // // for (int i = 0; i < columnCount; i++)
                    // // {
                    // //     // printf("Column %d: %s\n", i + 1, coba[i].name);
                    // // }
                    char table_path_cbcb[1024];
                    sprintf(table_path_cbcb, "databases/%s/%s.csv", currentDatabase, new_tableName);

                    FILE *file22 = fopen(table_path_cbcb, "r");

                    if (file22 == NULL)
                    {
                        send(new_socket, "Table not found!", 1024, 0);
                        continue;
                    }
                    char asli[1024] = {0};
                    char line22[1024];
                    while (fgets(line22, sizeof(line22), file22))
                    {
                        strcpy(asli, line22);
                        break;
                    }
                    fclose(file22);

                    char *cmdptr2_leo = asli;
                    char *token2_leo = {0};
                    for (int i = 0; token2_leo = strtok_r(cmdptr2_leo, ",", &cmdptr2_leo); i++)
                    {
                        char *cmdptr3_leo = token2_leo;
                        char *token3_leo = {0};
                        for (int j = 0; token3_leo = strtok_r(cmdptr3_leo, ":", &cmdptr3_leo); j++)
                        {
                            for (int z = 0; z < columnCount; z++)
                            {
                                if (strcmp(coba[z].name, token3_leo) == 0)
                                {
                                    coba[z].tanda = 1;
                                    coba[z].hasil = i + 1;
                                }
                            }
                        }
                    }

                    int tanda2 = 0;
                    for (int i = 0; i < columnCount; i++)
                    {
                        if (coba[i].tanda == 0)
                        {
                            tanda2 = 1;
                            break;
                        }
                    }
                    if (tanda2)
                    {
                        send(new_socket, "Column not found!", 1024, 0);
                        continue;
                    }
                    char hasil_fix[1024] = {0};
                    char awalan[1024] = {0};
                    sprintf(awalan, "awk -F \',\' \'$%d == \"%s\" {print ", index_buf1, kasus4_whereCondition);
                    char tengah[1024] = {0};
                    char akhiran[1024] = {0};
                    sprintf(akhiran, "}\' databases/%s/%s.csv > databases/%s/temp.csv", currentDatabase, new_tableName, currentDatabase);
                    for (int i = 0; i < columnCount; i++)
                    {
                        if (coba[i].tanda == 1)
                        {
                            char str[20];
                            sprintf(str, "$%d", coba[i].hasil);
                            strcat(tengah, str);
                            strcat(tengah, ",");
                        }
                    }
                    tengah[strlen(tengah) - 1] = '\0';
                    strcat(hasil_fix, awalan);
                    strcat(hasil_fix, tengah);
                    strcat(hasil_fix, akhiran);
                    system(hasil_fix);
                    char temp_path[1024];
                    sprintf(temp_path, "databases/%s/temp.csv", currentDatabase);
                    FILE *file2 = fopen(temp_path, "r");
                    char hasil_select[10001] = {0};
                    char line2[10001];
                    while (fgets(line2, sizeof(line2), file))
                    {
                        strcat(hasil_select, line2);
                    }
                    fclose(file);
                    char command11[1024];
                    sprintf(command11, "rm databases/%s/temp.csv", currentDatabase);
                    system(command11);
                    send(new_socket, hasil_select, 10001, 0);
                    continue;
                }
            }
        }
        else if (strstr(buffer, "DUMP ") != NULL) 
        {
            printf("otw ngedump\n");
            char DB[1000];
            strncpy(DB, buffer + 5, sizeof(DB) - 1);
            char* namaDB = strtok(DB, ";");

            printf("Nama database: %s\n", namaDB);

            char dirpath[1024];
            char table_path[1024];

            sprintf(dirpath, "databases/%s", namaDB);
            DIR *dir = opendir(dirpath);
            struct dirent *ent;
            if (dir == NULL) {
                perror("Error opening directory");
                printf("Directory: %s\n", dirpath);
                return 0;
            }

            int count = 0;
            char table[1000][1000];

            while ((ent = readdir(dir)) != NULL) {
                struct stat fileInfo;
                char filename[3000];
                sprintf(filename,"%s/%s", dirpath, ent->d_name);

                if (stat(filename, &fileInfo) == -1) {
                    printf("Error in getting file information\n");
                    continue;
                }

                if (S_ISREG(fileInfo.st_mode) && (ent->d_name != "user.csv")) {
                    char tablename[1000];
                    strncpy(tablename, ent->d_name, sizeof(tablename) - 1);
                    tablename[sizeof(tablename) - 1] = '\0'; 
                    
                    char *dot = strchr(tablename, '.');
                    if (dot != NULL) {
                        *dot = '\0';
                    }
                    
                    strcpy(table[count], tablename);
                    count++;
                }
            }

            closedir(dir);

            printf("Jumlah tabel: %d\n", count); //buat debug
            char tablecount[sizeof(int)];
            memcpy(tablecount, &count, sizeof(int));
            send(new_socket, tablecount, strlen(tablecount), 0);
            printf("Jumlah tabel: %d\n", count); //buat debug

            for (int i=0 ; i<count ; i++) {
                printf("Tabel %d\n", i); //buat debug

                char droptable[1000];
                char createtable[1000];
                char insertinto[1000];

                // drop table
                printf("DROP TABLE %s;\n", table[i]);
                strcpy(droptable, "DROP TABLE ");
                strcat(droptable, table[i]);
                strcat(droptable, ";");

                bzero(buffer, sizeof(buffer));
                send(new_socket, droptable, strlen(droptable), 0);
                read(new_socket, buffer, sizeof(buffer)); //buat debug

                strcpy(createtable, "CREATE TABLE ");
                strcat(createtable, table[i]);
                strcat(createtable, " (");

                // open csv
                char line[MAX_LINE_LENGTH];
                char columns[MAX_LINE_LENGTH];
                char columnTypes[MAX_COLUMNS][MAX_LINE_LENGTH];
                int numColumns = 0;
                sprintf(table_path, "%s/%s.csv", dirpath, table[i]);
                FILE *file = fopen(table_path, "r");
                if (file == NULL) {
                    printf("Tidak dapat membuka file\n");
                    return;
                }

                if (fgets(line, sizeof(line), file) != NULL) {
                    line[strcspn(line, "\r\n")] = '\0';
                    strcpy(columns, line);
                    char* token = strtok(columns, ",");
                    // strcat(createtable, table[i]);
                    // strcat(createtable, " (");

                    while (token != NULL) {
                        strcat(createtable, token);
                        token = strtok(NULL, ",");
                        if (token != NULL) {
                            strcat(createtable, ", ");
                        }
                        numColumns++;
                    }

                    strcat(createtable, ");\n");

                    bzero(buffer, sizeof(buffer));
                    send(new_socket, createtable, strlen(createtable), 0);
                    read(new_socket, buffer, sizeof(buffer)); // buat debug
                }

                // insert into
                while (fgets(line, sizeof(line), file) != NULL) {
                    line[strcspn(line, "\r\n")] = '\0'; // Menghapus karakter newline
                    char* token = strtok(line, ",");
                    printf("INSERT INTO %s (", table[i]);
                    strcpy(insertinto, "INSERT INTO ");
                    strcat(insertinto, table[i]);
                    strcat(insertinto, " (");
                    int colCount = 0;
                    while (token != NULL) {
                        printf("'%s'", token);
                        strcat(insertinto, "'");
                        strcat(insertinto, token);
                        strcat(insertinto, "'");
                        
                        if (token != NULL && colCount != numColumns - 1) {
                            printf(", ");
                            strcat(insertinto, ", ");
                        }
                        token = strtok(NULL, ",");
                        colCount++;
                    }
                    printf(");\n");
                    strcat(insertinto, ");\n");

                    bzero(buffer, sizeof(buffer));
                    send(new_socket, insertinto, strlen(insertinto), 0);
                    read(new_socket, buffer, sizeof(buffer)); //buat debug
                }
                fclose(file);
            }
        }
        send(new_socket, "masuk sini", 1024, 0);
    }
    close(new_socket);

    return 0;
}
