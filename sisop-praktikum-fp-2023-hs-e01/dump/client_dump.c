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
    char buffer[1024] = {0};

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
    // printf("%s\n", buffer3);
    if (strcmp(buffer, "Wrong username or password!") == 0)
    {
        return 0;
    }
    // printf("%d\n", argc);

    // printf("%s\n", argv[5]);
    if (argc == 6)
    {
        char query[1024];
        if (getuid() == 0) {  // root
            sprintf(query, "DUMP %s;", argv[2]);
        } else {
            sprintf(query, "DUMP %s;", argv[5]);
        }

        send(sock, query, strlen(query), 0);
        bzero(buffer, sizeof(buffer));

        int receivenum;
        char tablecount[sizeof(int)];

        valread = read(sock, tablecount, 1024);
        if (valread <= 0) {
            printf("gagal");
        }

        memcpy(&receivenum, tablecount, sizeof(int));

        // terima respon dari database
        char receivedata[5000] = {0};
        for (int i=0 ; i<receivenum ; i++) {
            // drop table
            bzero(receivedata, sizeof(receivedata));
            valread = read(sock, receivedata, sizeof(receivedata));
            if (valread <= 0) {
                printf("gagal dapet droptable\n");
            }
            printf("%s\n", receivedata);
            send(sock, "droptable", 9, 0);

            // create table
            bzero(receivedata, sizeof(receivedata));
            valread = read(sock, receivedata, sizeof(receivedata));
            if (valread <= 0) {
                printf("gagal dapet createtable\n");
            }
            printf("%s\n", receivedata);
            send(sock, "createtable", 11, 0);

            // insert into
            bzero(receivedata, sizeof(receivedata));
            valread = read(sock, receivedata, sizeof(receivedata));
            if (valread <= 0) {
                printf("gagal dapet insertinto\n");
            }
            printf("%s\n", receivedata);
            send(sock, "insertinto", 10, 0);

            printf("\n");
        }
    }

    close(sock);

    return 0;
}
