#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main(int argc, char const *argv[])
{
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    char username[1024];
    int uid = getuid();
    char struid[10];
    sprintf(struid, "%d", uid);
    send(sock, struid, strlen(struid), 0);
    // printf("%s\n", struid);
    char buffer[10001] = {0};

    if (uid != 0)
    {
        char buffer2[1024];
        read(sock, buffer2, 1024);
        // printf("%s\n", buffer2);
        bzero(buffer2, 1024);
        sprintf(buffer2, "%s,%s\n", argv[2], argv[4]);
        send(sock, buffer2, strlen(buffer2), 0);
        // printf("%s\n", buffer2);
    }

    if (uid == 0)
        strcpy(username, "Root");
    else
        strcpy(username, argv[2]);
    char buffer3[1024] = {0};
    valread = read(sock, buffer3, 1024);
    printf("%s\n", buffer3);
    if (strcmp(buffer, "Wrong username or password!") == 0)
    {
        return 0;
    }
    // printf("%d\n", argc);
    // printf("%s\n", argv[6]);
    if (argc == 9)
    {
        char command1[1024];
        sprintf(command1, "USE %s;", argv[6]);
        send(sock, command1, strlen(command1), 0);
        char buffer4[1024] = {0};
        valread = read(sock, buffer4, 1024);
        if (strcmp(buffer4, "Access denied!") == 0)
        {
            return 0;
        }
        if (strcmp(buffer4, "Database does not exist!") == 0)
        {
            return 0;
        }
        printf("%s\n", buffer4);
        char command2[1024];
        sprintf(command2, "/home/leo/sisop/FP/client/%s", argv[8]);
        FILE *file = fopen(command2, "r");
        char line[1024];
        int found = 0;
        while (fgets(line, 1024, file))
        {
            // printf("%s", line);
            size_t length = strlen(line);
            if (length > 0 && line[length - 1] == '\n')
            {
                strncpy(line + length - 1, "\0", 1);
            }
            send(sock, line, strlen(line), 0);
            char buffer5[1024] = {0};
            valread = read(sock, buffer5, 1024);
            printf("%s\n", buffer5);
        }
        fclose(file);
    }
    else
    {
        while (1)
        {
            char hello[1024];
            printf("[%s] Enter message (or 'q' to quit): \n", username);
            scanf(" %[^\n]%*c", hello);

            if (strcmp(hello, "q") == 0)
            {
                break;
            }

            send(sock, hello, strlen(hello), 0);
            // printf("Message sent\n");

            valread = read(sock, buffer, 10001);
            printf("%s\n", buffer);
        }
    }

    close(sock);

    return 0;
}
