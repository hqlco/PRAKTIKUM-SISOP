# sisop-praktikum-fp-2023-HS-E01
| Nama                      | NRP           |Username           |
|---------------------------|---------------|--------------|
|Moh Rosy Haqqy Aminy       |5025211012     |rosyhaqqy          |
|Rule Lulu Damara           |5025211050     |rulelulu           |
|Rizky Alifiyah Rahma       |5025211209     |ikikk              |

## A. Autentikasi

### LOGIN
code untuk login:
``` bash
// ./database

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

```
penjelasan singkat:
jika login sebagai root maka pasti ```pid``` bernilai 0 dan jika tidak 0 tinggal validasi di database_admin 

``` bash
// ./client
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
```
penjelasan singkat: akan mengirimkan ```pid``` ke server dan akan mengirimkan username dan password
Bukti screenshot hasil:
#### Login as a root
![1 FP sisop](https://github.com/hqlco/sampah/assets/86828535/da0bea5a-659f-4808-9af2-a5dfcec06483)
#### Login as valid user and pasword
![3 fp sisop](https://github.com/hqlco/sampah/assets/86828535/ec319718-6b69-47d5-b0ca-c098a96d9ce7)
#### Login as Non valid user and pasword
![4 fp sisop](https://github.com/hqlco/sampah/assets/86828535/dc409a7d-e05a-46fb-b7f9-218c0c3e7dcf)

### CREATE USER
code untuk CREATE USER:
```bash
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
```
penjelasan singkat: jika root maka bisa create user. user akan dimaksukkan ke database admin difile user.csv
Bukti screenshot hasil:
##### CREATE USER khonsu2
![2 fp sisop](https://github.com/hqlco/sampah/assets/86828535/52cfc6ae-7c98-45bd-8066-4f45bbd24307)

## B. Autorisasi
### USE
code USE:
``` bash
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
```
Penjelasan singkat: jika root dapat memakai semua database. Jika tidak root maka akan dilihat list permission di database tersebut untuk user yang login.(semua user mempunyai database tamplate untuk current database).
bukti screenshot hasil:
#### AKSES DITOLAK
![5 fp sisop](https://github.com/hqlco/sampah/assets/86828535/da53b1f0-9891-4dbc-b16f-73bab57fcd14)

### GRANT PERMISSION 
code untuk GRANT PERMISSION :
```bash
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
```
Penjelasan singkat: jika Root maka bisa memberi permission pada user. pada saat diberi permission user akan di list di suatu tabel untuk melist siapa saja yang berhak akses database tersebut
bukti screenshot hasil:

#### GRANT PERMISSION khonsu2 untuk mengakses database coba
![6 fp sisop](https://github.com/hqlco/sampah/assets/86828535/d3568769-f86f-4053-aaca-654c6dbad412)

## C. Data Definition Language
### a. CREATE DATABASE
code untuk CREATE DATABASE
```bash
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
            s
```

### Penjelasan
Pertama, kode memeriksa permintaan untuk memastikan bahwa permintaan dimulai dengan string "CREATE DATABASE". Jika demikian, kode akan mengambil nama basis data dari token ketiga yang ada dalam permintaan.

Selanjutnya, kode akan membuat sebuah direktori baru menggunakan perintah mkdir dengan menggunakan nama basis data yang telah diperoleh sebelumnya. Hal ini akan menciptakan struktur direktori yang sesuai dengan basis data yang baru.

Selanjutnya, kode akan membuat sebuah file bernama "user.csv" di dalam direktori basis data yang baru dibuat. File ini akan digunakan untuk menyimpan data pengguna terkait basis data.

Kemudian, kode akan menulis baris judul dengan nilai "username" ke dalam file "user.csv". Ini akan menjadi baris pertama dalam file, yang akan mengidentifikasi kolom data yang ada dalam file tersebut.

Terakhir, kode akan menambahkan nama pengguna ke dalam file "user.csv". Dengan melakukan ini, kode menyimpan data pengguna terkait basis data yang baru dibuat.

Setelah langkah-langkah di atas selesai, kode akan mengirimkan pesan konfirmasi "Database successfully created!" ke soket yang sesuai untuk memberi tahu pengguna bahwa basis data telah berhasil dibuat.
### Dokumentasi CREATE DATABASE database1;
![Screenshot (1335)](https://github.com/RuleLuluDamara/Images/assets/105763198/d4e0db15-26dd-40cd-99a3-fb877267e507)

### b. CREATE TABLE
code untuk CREATE TABLE
```bash
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
```
### Penjelasan
Kode ini akan memeriksa permintaan untuk memastikan bahwa permintaan dimulai dengan "CREATE TABLE". Jika permintaan valid, kode akan mengambil nama tabel dari permintaan tersebut.

Selanjutnya, kode akan memproses permintaan untuk mendapatkan nama kolom dan tipe datanya. Kode akan membentuk string yang merepresentasikan kolom dan menambahkannya ke dalam variabel fix.

Setelah semua kolom diproses, kode akan membuat file baru untuk tabel menggunakan nama tabel dan basis data yang sesuai. Kode akan menulis string fix ke dalam file sebagai baris judul tabel.

Terakhir, kode akan mengirim pesan konfirmasi "Table successfully created!" melalui soket untuk memberi tahu pengguna bahwa tabel baru telah berhasil dibuat.

### Dokumentasi CREATE TABLE table1 (kolom1 string, kolom2 int, kolom3 string, kolom4 int);
![Screenshot (1336)](https://github.com/RuleLuluDamara/Images/assets/105763198/5751f1da-f112-4dff-b4ec-06e63c5c5d04)

#### c. DROP
### c.1 DROP DATABASE
code untuk DROP DATABASE
```bash
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
```
### Penjelasan
Kode ini memeriksa apakah permintaan dimulai dengan string "DROP DATABASE" menggunakan fungsi strncmp. Jika permintaan memenuhi syarat, kode akan memeriksa apakah pengguna memiliki izin untuk menjalankan operasi ini.

Jika pengguna adalah root, kode akan mengambil nama basis data yang akan dihapus dari permintaan tersebut. Selanjutnya, kode akan memeriksa apakah basis data tersebut ada. Jika tidak ada, kode akan mengirimkan pesan "Database does not exist!" melalui soket. Jika basis data ada, kode akan memeriksa apakah basis data tersebut sedang digunakan. Jika sedang digunakan, pesan "You are currently using this database!" akan dikirimkan melalui soket. Jika tidak sedang digunakan, kode akan menjalankan perintah sistem untuk menghapus folder basis data tersebut menggunakan rm -rf dan mengirimkan pesan "Database successfully dropped!" melalui soket.

Jika pengguna bukan root, kode akan melakukan langkah-langkah yang serupa seperti pada pengguna root, namun dengan pengecualian bahwa kode juga akan memeriksa apakah pengguna memiliki akses ke basis data yang akan dihapus. Jika pengguna tidak memiliki akses, pesan "Access denied!" akan dikirimkan melalui soket. Jika pengguna memiliki akses, basis data akan dihapus seperti yang dijelaskan sebelumnya.
### Dokumentasi DROP DATABASE database1;
![Screenshot (1339)](https://github.com/RuleLuluDamara/Images/assets/105763198/6e60bfb0-b5bd-43db-ba14-7900e85885b4)

### c.2 DROP TABLE
code untuk DROP TABLE
```bash
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

```
### Penjelasan
Kode ini memeriksa apakah permintaan dimulai dengan string "DROP TABLE" menggunakan fungsi strncmp. Jika permintaan memenuhi syarat, kode akan mengambil nama tabel yang akan dihapus dari permintaan tersebut.

Selanjutnya, kode akan memeriksa apakah tabel tersebut ada di basis data yang sedang digunakan. Kode akan membangun path file yang sesuai dengan basis data dan nama tabel yang diperoleh. Jika file tidak ada, kode akan mengirimkan pesan "Table does not exist!" melalui soket.

Jika tabel ada, kode akan menjalankan perintah sistem untuk menghapus file tabel menggunakan perintah rm dan mengirimkan pesan "Table successfully dropped!" melalui soket untuk memberi tahu pengguna bahwa tabel telah berhasil dihapus.
### Dokumentasi DROP TABLE table1;
![Screenshot (1338)](https://github.com/RuleLuluDamara/Images/assets/105763198/380f6298-7ecc-4c4c-80ab-d3366690a211)

### c.3 DROP COLUMN
code untuk DROP COLUMN
```bash
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

```
### Penjelasan
Kode ini memeriksa apakah permintaan dimulai dengan string "DROP COLUMN" menggunakan fungsi strncmp. Jika permintaan memenuhi syarat, kode akan mengambil nama kolom yang akan dihapus dan nama tabel dari permintaan tersebut.

Selanjutnya, kode akan memeriksa apakah tabel tersebut ada di basis data yang sedang digunakan. Kode akan membangun path file yang sesuai dengan basis data dan nama tabel yang diperoleh. Jika file tidak ada, kode akan mengirimkan pesan "Table does not exist!" melalui soket.

Jika tabel ada, kode akan membaca isi file tabel dan mencari posisi kolom yang ingin dihapus. Setelah menemukan posisi kolom, kode akan menggunakan perintah sistem cut untuk menghapus kolom tersebut dari file tabel. File hasil pemotongan akan disimpan dalam file sementara "output.csv". Kemudian, kode akan menghapus file tabel asli menggunakan perintah rm, dan mengganti nama file sementara menjadi file tabel baru menggunakan perintah mv.

Terakhir, kode akan mengirimkan pesan "Table successfully dropped!" melalui soket untuk memberi tahu pengguna bahwa kolom telah berhasil dihapus dari tabel.
### Dokumentasi DROP COlUMN kolom1;
![Screenshot (1337)](https://github.com/RuleLuluDamara/Images/assets/105763198/2e962a3f-fa34-48fc-89ec-3341ab87a8a2)

## D. Data Manipulation Language
### a. INSERT INTO
code untuk INSERT INTO
```bash
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
```
### Penjelasan
Kode ini memeriksa apakah permintaan dimulai dengan string "INSERT INTO" menggunakan fungsi strncmp. Jika permintaan memenuhi syarat, kode akan mengambil nama tabel dan nilai-nilai data yang akan disisipkan dari permintaan tersebut.

Selanjutnya, kode akan membangun path file yang sesuai dengan basis data, nama tabel, dan ekstensi ".csv". Kemudian, kode akan membuka file tabel dalam mode "a" (append) untuk menambahkan data baru.

Selanjutnya, kode akan memproses nilai-nilai data yang akan disisipkan. Kode akan menghapus karakter "'" (single quote) dan spasi dari nilai-nilai tersebut untuk mendapatkan nilai yang valid dan sesuai dengan format tabel. Setelah memproses nilai-nilai data, kode akan menulis data baru ke file tabel dengan menggunakan perintah fprintf dengan format yang sesuai.

Setelah selesai menulis data, kode akan menutup file tabel dan mengirimkan pesan "Data successfully inserted!" melalui soket untuk memberi tahu pengguna bahwa data telah berhasil disisipkan ke dalam tabel.
### Dokumentasi INSERT INTO table1 (‘value1’, 2, ‘value3’, 4);
![Screenshot (1340)](https://github.com/RuleLuluDamara/Images/assets/105763198/04505bef-c5ca-4b1e-8903-15703e73da61)

### b. UPDATE SET 
code untuk UPDATE SET
```bash
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
                        if (len > 0 && valuePtr[0] == '\''')
                        {
                            memmove(valuePtr, valuePtr + 1, len - 1); // Menghapus tanda petik tunggal di awal
                            len--;
                        }
                        if (len > 0 && valuePtr[len - 1] == ';')
                        {
                            valuePtr[len - 1] = '\0'; // Menghapus titik koma di akhir
                            len--;
                        }
                        if (len > 0 && valuePtr[len - 1] == '\''')
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
		        if (len > 0 && conditionValuePtr[0] == '\''')
		        {
		            memmove(conditionValuePtr, conditionValuePtr + 1, len - 1); // Menghapus tanda petik tunggal di awal
		            len--;
		        }
		        if (len > 0 && conditionValuePtr[len - 1] == ';')
		        {
		            conditionValuePtr[len - 1] = '\0'; // Menghapus titik koma di akhir
		            len--;
		        }
		        if (len > 0 && conditionValuePtr[len - 1] == '\''')
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
```
### Penjelasan
Kode ini memeriksa apakah permintaan dimulai dengan string "UPDATE" menggunakan fungsi strncmp. Jika permintaan memenuhi syarat, kode akan mengambil nama tabel, nama kolom yang akan diupdate, nilai baru yang akan diassign ke kolom tersebut, nama kolom kondisi, dan nilai kondisi dari permintaan tersebut.

Selanjutnya, kode akan membangun path file yang sesuai dengan basis data, nama tabel, dan ekstensi ".csv". Kemudian, kode akan membuka file tabel dalam mode "r" (read) dan file temporary dalam mode "w" (write).

Kode akan membaca header kolom dari file CSV dan menuliskannya ke file temporary. Selanjutnya, kode akan mencari indeks kolom target dan kolom kondisi dalam header kolom.

Setelah itu, kode akan menyalin data dari file CSV ke file temporary dengan melakukan update pada kolom target yang memenuhi kondisi yang ditentukan. Data lain dalam baris tersebut akan disalin tanpa perubahan.

Setelah selesai melakukan update pada file temporary, kode akan menggantikan file CSV asli dengan file temporary yang sudah diupdate. Selanjutnya, kode akan menghilangkan baris kosong antara setiap baris dalam file CSV.

Terakhir, kode akan mengirimkan pesan "Data successfully updated!" melalui soket untuk memberi tahu pengguna bahwa data telah berhasil diupdate dalam tabel.
### Dokumentasi UPDATE table1 SET kolom1=’new_value1’;
![Screenshot (1341)](https://github.com/RuleLuluDamara/Images/assets/105763198/f051ce6f-24e8-4c03-a985-70f0bc6fd71e)

### c. DELETE
code untuk DELETE
```bash
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
                    if (len > 0 && valueWherePtr[0] == '\''')
                    {
                        memmove(valueWherePtr, valueWherePtr + 1, len - 1);
                        len--;
                    }
                    if (len > 0 && valueWherePtr[len - 1] == ';')
                    {
                        valueWherePtr[len - 1] = '\0';
                        len--;
                    }
                    if (len > 0 && valueWherePtr[len - 1] == '\''')
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
```
### Penjelasan
Kode tersebut merupakan bagian dari implementasi untuk menghapus baris-baris data dari sebuah tabel dalam database. Bagian kode yang diberikan menjelaskan bagian yang akan dieksekusi jika perintah yang diterima adalah "DELETE FROM".

Pertama, kode menginisialisasi beberapa variabel yang akan digunakan, seperti tableName, cmd, cmdptr, token, whereColumnName, whereCondition, dan deleteWhere. Kemudian, dengan menggunakan loop for dan fungsi strtok_r, kode memproses perintah yang diterima untuk mendapatkan nama tabel yang akan dihapus datanya, serta kolom dan kondisi WHERE jika ada.

Setelah itu, kode memeriksa apakah tabel yang dimaksud ada dalam database. Jika tabel tidak ditemukan, pesan "Table does not exist!" dikirimkan melalui soket, dan iterasi berikutnya dimulai.

Jika tabel ditemukan, kode membaca baris header dari file CSV yang sesuai dengan nama tabel tersebut dan menyimpannya dalam file temporary.

Selanjutnya, jika perintah DELETE memiliki kondisi WHERE, baris terakhir kode yang diberikan akan dieksekusi. Kode tersebut menutup file yang sedang dibuka, yaitu file asli dan file temporary. Pesan "Delete where successfully!" dikirimkan melalui soket untuk memberi tahu pengguna bahwa operasi penghapusan telah berhasil. Selanjutnya, file asli diganti dengan file temporary menggunakan fungsi remove() dan rename().

Jika perintah DELETE tidak memiliki kondisi WHERE, maka semua baris dalam tabel akan dihapus. Dalam hal ini, baris terakhir kode yang diberikan akan dieksekusi. Kode tersebut juga menutup file yang sedang dibuka, yaitu file asli dan file temporary. Pesan "Delete where successfully!" dikirimkan melalui soket untuk memberi tahu pengguna bahwa semua baris dalam tabel telah dihapus. Selanjutnya, file asli diganti dengan file temporary menggunakan fungsi remove() dan rename().
### Dokumentasi DELETE FROM table1;
![Screenshot (1342)](https://github.com/RuleLuluDamara/Images/assets/105763198/1564ae6d-da87-4426-b92f-9808a54d79ae)

### d. SELECT
code untuk SELECT
```bash
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
```
### Penjelasan
mengunakan prisip ```awk``` untuk kasus kolom dan mengunakan file temp untuk ```*```.
### Dokumentasi SELECT kolom1, kolom2 FROM table1;
![10 FP sisop](https://github.com/hqlco/sampah/assets/86828535/5f79e925-cef6-4776-a77d-1f75d0d46f5a)
### Dokumentasi SELECT * FROM table1;
![9 FP sisop](https://github.com/hqlco/sampah/assets/86828535/6fee2514-72d0-460c-a351-e5dad0c44f92)

#### e. WHERE
### e.1 UPDATE SET WHERE
code untuk UPDATE SET WHERE
```bash
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
                        if (len > 0 && valuePtr[0] == '\''')
                        {
                            memmove(valuePtr, valuePtr + 1, len - 1); // Menghapus tanda petik tunggal di awal
                            len--;
                        }
                        if (len > 0 && valuePtr[len - 1] == ';')
                        {
                            valuePtr[len - 1] = '\0'; // Menghapus titik koma di akhir
                            len--;
                        }
                        if (len > 0 && valuePtr[len - 1] == '\''')
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
		        if (len > 0 && conditionValuePtr[0] == '\''')
		        {
		            memmove(conditionValuePtr, conditionValuePtr + 1, len - 1); // Menghapus tanda petik tunggal di awal
		            len--;
		        }
		        if (len > 0 && conditionValuePtr[len - 1] == ';')
		        {
		            conditionValuePtr[len - 1] = '\0'; // Menghapus titik koma di akhir
		            len--;
		        }
		        if (len > 0 && conditionValuePtr[len - 1] == '\''')
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
```
### Penjelasan
Kode ini memeriksa apakah permintaan dimulai dengan string "UPDATE" menggunakan fungsi strncmp. Jika permintaan memenuhi syarat, kode akan mengambil nama tabel, nama kolom yang akan diupdate, nilai baru yang akan diassign ke kolom tersebut, nama kolom kondisi, dan nilai kondisi dari permintaan tersebut.

Selanjutnya, kode akan membangun path file yang sesuai dengan basis data, nama tabel, dan ekstensi ".csv". Kemudian, kode akan membuka file tabel dalam mode "r" (read) dan file temporary dalam mode "w" (write).

Kode akan membaca header kolom dari file CSV dan menuliskannya ke file temporary. Selanjutnya, kode akan mencari indeks kolom target dan kolom kondisi dalam header kolom.

Setelah itu, kode akan menyalin data dari file CSV ke file temporary dengan melakukan update pada kolom target yang memenuhi kondisi yang ditentukan. Data lain dalam baris tersebut akan disalin tanpa perubahan.

Setelah selesai melakukan update pada file temporary, kode akan menggantikan file CSV asli dengan file temporary yang sudah diupdate. Selanjutnya, kode akan menghilangkan baris kosong antara setiap baris dalam file CSV.

Terakhir, kode akan mengirimkan pesan "Data successfully updated!" melalui soket untuk memberi tahu pengguna bahwa data telah berhasil diupdate dalam tabel.
### Dokumentasi UPDATE table1 SET kolom1='new_value1' WHERE kolom1='buku1';
![Screenshot (1343)](https://github.com/RuleLuluDamara/Images/assets/105763198/f6fa295f-c252-45e4-85ac-4e4eb6c08375)

### e.2 SELECT WHERE
code untuk SELECT
```bash
else if (!strncmp(buffer, "SELECT", 6))
        {
            char str[1024] = {0};
            strcpy(str, buffer);

            // // Mengirim header kolom ke klien
            // char header[1024];
            // fgets(header, sizeof(header), file);
            // send(new_socket, header, 1024, 0);

            // Memeriksa apakah harus mengirim semua kolom atau hanya kolom tertentu
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
                            if (len > 0 && valueWherePtr[0] == '\''')
                            {
                                memmove(valueWherePtr, valueWherePtr + 1, len - 1);
                                len--;
                            }
                            if (len > 0 && valueWherePtr[len - 1] == ';')
                            {
                                valueWherePtr[len - 1] = '\0';
                                len--;
                            }
                            if (len > 0 && valueWherePtr[len - 1] == '\''')
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
                            if (kasus4_len > 0 && kasus4_valueWherePtr[0] == '\''')
                            {
                                memmove(kasus4_valueWherePtr, kasus4_valueWherePtr + 1, kasus4_len - 1);
                                kasus4_len--;
                            }
                            if (kasus4_len > 0 && kasus4_valueWherePtr[kasus4_len - 1] == ';')
                            {
                                kasus4_valueWherePtr[kasus4_len - 1] = '\0';
                                kasus4_len--;
                            }
                            if (kasus4_len > 0 && kasus4_valueWherePtr[kasus4_len - 1] == '\''')
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
```
### Penjelasan
Kode di atas adalah contoh implementasi dalam bahasa C untuk menangani permintaan SQL SELECT dari klien dalam protokol komunikasi server-klien. Pada kode tersebut, server menerima perintah SELECT dari klien dan memeriksa apakah permintaan tersebut membutuhkan semua kolom (tanda '*') atau hanya kolom tertentu.

Jika permintaan membutuhkan semua kolom, server akan memproses perintah tersebut dengan mengekstrak nama tabel, kolom-kolom yang diminta, dan kondisi WHERE jika ada. Selanjutnya, server akan menjalankan perintah SQL dan mengirim hasilnya ke klien.

Namun, jika permintaan membutuhkan kolom-kolom tertentu, kode tersebut juga dapat menangani kasus tersebut. Kolom-kolom yang diminta akan diekstrak dari perintah SQL, dan server akan menjalankan perintah tersebut dengan mengirim hanya kolom-kolom yang diminta ke klien.

Selain itu, terdapat juga beberapa variabel dan perintah lain yang digunakan dalam proses pemrosesan perintah SELECT, seperti mengambil nilai WHERE jika ada, menyalin token-token perintah, dan lain sebagainya. Keseluruhan kode tersebut membantu server dalam memproses permintaan SELECT dari klien dan memberikan respons yang sesuai.

### Dokumentasi SELECT kolom1, kolom2 FROM table1;
![Screenshot (1346)](https://github.com/RuleLuluDamara/Images/assets/105763198/b44c15cc-6dff-4137-987c-7428742ede44)

### e.3 DELETE WHERE
code untuk DELETE WHERE
```bash
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
                    if (len > 0 && valueWherePtr[0] == '\''')
                    {
                        memmove(valueWherePtr, valueWherePtr + 1, len - 1);
                        len--;
                    }
                    if (len > 0 && valueWherePtr[len - 1] == ';')
                    {
                        valueWherePtr[len - 1] = '\0';
                        len--;
                    }
                    if (len > 0 && valueWherePtr[len - 1] == '\''')
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
        }
```
### Penjelasan
Kode ini adalah bagian dari sebuah program yang melakukan operasi DELETE pada sebuah tabel dalam database CSV. Saat menerima perintah DELETE, program akan memproses string perintah yang diterima untuk mendapatkan nama tabel dan kondisi WHERE (jika ada) yang digunakan untuk menghapus baris-baris tertentu dari tabel tersebut.

Pertama, program memisahkan string perintah menggunakan fungsi strtok_r() dan menyimpan nama tabel dan kondisi WHERE (jika ada) dalam variabel yang sesuai. Kemudian, program membuka file CSV yang sesuai dengan nama tabel yang diberikan.

Jika tabel tidak ditemukan, program akan mengirimkan pesan kesalahan kepada pengguna. Jika tabel ditemukan, program akan membaca baris header dari file dan menyimpannya dalam file temporary.

Selanjutnya, program akan memeriksa apakah perintah DELETE memiliki kondisi WHERE. Jika ya, program akan memeriksa indeks kolom yang sesuai dengan nama kolom yang diberikan dalam header tabel. Jika kolom tidak ditemukan, program akan mengirimkan pesan kesalahan kepada pengguna.

Selanjutnya, program akan membaca setiap baris dari file CSV. Untuk setiap baris, program akan memeriksa nilai pada kolom yang sesuai dengan kondisi WHERE. Jika nilai tersebut memenuhi kondisi, baris akan diabaikan dan tidak disalin ke file temporary. Jika nilai tidak memenuhi kondisi, baris akan disalin ke file temporary.

Setelah selesai membaca seluruh baris, program akan menutup file asli dan file temporary. Kemudian, program akan mengganti file CSV asli dengan file temporary yang telah diubah. Jika ada baris yang terhapus, program akan mengirimkan pesan yang menyatakan jumlah baris yang dihapus kepada pengguna. Jika tidak ada baris yang terhapus, program akan mengirimkan pesan yang menyatakan bahwa tidak ada baris yang terhapus.
### Dokumentasi DELETE FROM table1 WHERE kolom1='new_value1';
![Screenshot (1344)](https://github.com/RuleLuluDamara/Images/assets/105763198/af0c5320-a3f1-4ab5-b99c-5e72e89b07e7)

## Dokumentasi SOCKET berjalan
![Screenshot (1345)](https://github.com/RuleLuluDamara/Images/assets/105763198/68ccd28a-6eda-4160-8a32-3ff8c57b9d11)

## E. Logging
Pada poin logging, kami diperintahkan untuk memasukkan setiap command yang dipakai harus dilakukan logging ke suatu file dengan format. Jika yang eksekusi root, maka username root. Poin ini cukup sederhana, berikut merupakan contoh isi dari ```log.txt``` yang kami buat:
![image](https://github.com/Ikikk/jgndibuka/assets/90395116/e5b8a7cc-c792-41d0-a552-13bdb640decb)

## F. Reliability
Untuk poin reliability, kami merevisi pada bagian mengirim dan menerima respon antara ```client_dump.c``` dan ```database.c```, restuktur sinyal yang akan dikirimkan pada ```database.c```, dan crontab. Jadi pada poin ini diperintahkan untuk membuat dump dari suatu database dengan menggunakan redirection menggunakan command:
```bash
./[program_dump_database] -u [username] -p [password] [nama_database] > [nama_file.backup]
```
Program untuk membuat dump dari suatu database disimpan di dalam file ```client_dump.c```. Struktur program dalam file ```client_dump.c``` ini kurang lebih sama dengan struktur file ```client.c```, perbedaannya adalah pada ````client_dump.c``` hanya akan mengirim sinyal  ```DUMP [database]``` ke file ```database.c``` untuk mengakses proses dump database serta menerima respon dari ```database.c``` untuk diproses ke dalam file ```[database].backup```. Berikut adalah penjelasan detailnya:
1. Pertama, ```client_dump.c``` akan mengirim sinyal ```DUMP [database]```
    ```bash
    if (getuid() == 0) {  // root
        sprintf(query, "DUMP %s;", argv[2]);
    } else {
        sprintf(query, "DUMP %s;", argv[5]);
    }

    send(sock, query, strlen(query), 0);
    bzero(buffer, sizeof(buffer));
    ```

2. Setelah dikirim menggunakan socket dari ```client_dump.c```, selanjutnya sinyal akan diolah pada ```database.c```

3. Dalam ```database.c```:
    ```bash
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

    ```
    Penjelasan:
    - Program akan membaca nama database yang akan di-dump dari sinyal yang diterima dari ```client_dump.c```
        ```bash
        trncpy(DB, buffer + 5, sizeof(DB) - 1);
        char* namaDB = strtok(DB, ";");

        printf("Nama database: %s\n", namaDB);
        ```
    - Selanjutnya, program membuka direktori database tersebut dan menghitung jumlah tabel yang ada dengan cara memeriksa jumlah file ```.csv``` yang ada di dalam direktori tersebut
        ```bash
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
        ```

    - Program mengirimkan jumlah tabel yang ada ke ```client_dump.c``` menggunakan command ```send()```
        ```bash
        send(new_socket, tablecount, strlen(tablecount), 0);
        ```

    - Program kemudian melakukan loop sebanyak jumlah tabel yang akan di-dump. Berikut adalah langkah-langkah program di dalam loop:
        - Mengirimkan sinyal "DROP TABLE"
            ```bash
            // drop table
            printf("DROP TABLE %s;\n", table[i]);
            strcpy(droptable, "DROP TABLE ");
            strcat(droptable, table[i]);
            strcat(droptable, ";");

            bzero(buffer, sizeof(buffer));
            send(new_socket, droptable, strlen(droptable), 0);
            ```

        - Mengirimkan sinyal "CREATE TABLE" dengan membaca baris pertama dari file ```.csv``` untuk mendapatkan nama kolom-kolom
            ```bash
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
            ```

        - Mengirimkan sinyal "INSERT INTO"
            ```bash
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
            ```

        - Langkah-langkah diatas akan berulang untuk setiap tabel hingga tidak ada tabel lagi yang akan di-dump

4. Di dalam ```client_dump.c```, program akan menerima respon jumlah tabel yang akan di-dump dari ```database.c``` dan mengkonversi nilai tersebut ke dalam integer
    ```bash
    int receivenum;
    char tablecount[sizeof(int)];

    valread = read(sock, tablecount, 1024);
    if (valread <= 0) {
        printf("gagal");
    }

    memcpy(&receivenum, tablecount, sizeof(int));
    ```
5. Program akan melakukan loop untuk menerima respon data dump dari ```database.c``` sebanyak jumlah tabel yang akan di-dump. Dalam loop tersebut, program melakukan langkah-langkah sebagai berikut:
    - Menerima respon untuk ```DROP TABLE [table];``` dari server menggunakan dan mencetaknya ke file ```[database].backup```
        ```bash
        // drop table
        bzero(receivedata, sizeof(receivedata));
        valread = read(sock, receivedata, sizeof(receivedata));
        if (valread <= 0) {
            printf("gagal dapet droptable\n");
        }
        printf("%s\n", receivedata);
        send(sock, "droptable", 9, 0);
        ```
    - Menerima respon untuk ```CREATE TABLE [table] (%s);``` di mana ```%s``` adalah nama kolom-kolom yang ada pada tabel dari server menggunakan dan mencetaknya ke file ```[database].backup```
        ```bash
        // create table
        bzero(receivedata, sizeof(receivedata));
        valread = read(sock, receivedata, sizeof(receivedata));
        if (valread <= 0) {
            printf("gagal dapet createtable\n");
        }
        printf("%s\n", receivedata);
        send(sock, "createtable", 11, 0);
        ```
    - Menerima respon untuk ```INSERT INTO [table] (%s);``` di mana ```%s``` adalah nilai-nilai dari setiap kolom dari server menggunakan dan mencetaknya ke file ```[database].backup```
        ```bash
        // insert into
        bzero(receivedata, sizeof(receivedata));
        valread = read(sock, receivedata, sizeof(receivedata));
        if (valread <= 0) {
            printf("gagal dapet insertinto\n");
        }
        printf("%s\n", receivedata);
        send(sock, "insertinto", 10, 0);
        ```
6. Setelah proses memasukkan dump database ke dalam file ```.backup``` selesai, selanjutnya mengatur agar program dump database tersebut dapat melakukan dump setiap jam untuk semua database dan log. Kemudian di zip dengan format nama timestamp pada saat proses zip dilakukan, lalu file log dikosongkan kembali. Bagian ini akan dijalankan menggunakan crontab. Berikut isi file ```cron.sh``` beserta command ```crontab -e``` yang kami gunakan:
    ```bash
    #!/bin/bash

    cd /home/ikikk/sisop-praktikum-fp-2023-hs-e01/database

    /usr/bin/zip -rm /home/ikikk/sisop-praktikum-fp-2023-hs-e01/dump/$(date "+%Y-%m-%d")\ $(date +"%H:%M:%S") log.txt /home/ikikk/sisop-praktikum-fp-2023-hs-e01/dump/*.backup

    #crontab -e
    # 0 * * * * /bin/bash /home/ikikk/sisop-praktikum-fp-2023-hs-e01/database/cron.sh
    ```

### Dokumentasi Realiability
#### File Backup
![Screenshot 2023-06-25 164432](https://github.com/Ikikk/jgndibuka/assets/90395116/33d644f5-0bb2-45bd-a9d0-48f0fcbb2717)

#### File ZIP yang dihasilkan
![Screenshot 2023-06-25 164454](https://github.com/Ikikk/jgndibuka/assets/90395116/002afe41-e690-4148-aae2-76983cff2481)

#### Isi dari file ZIP
![Screenshot 2023-06-25 164517](https://github.com/Ikikk/jgndibuka/assets/90395116/c38d240e-daf1-4025-a8ee-53bb84695be8)
![Screenshot 2023-06-25 164533](https://github.com/Ikikk/jgndibuka/assets/90395116/67d733da-5a02-4096-b6ca-81715f147424)

## G. Tambahan
Code untuk tambahan:
```bash
// ./client
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
```
penjelasan singkat: melakukan hal yang sama dengan input user. perbedaanya membaca file perintah untuk diterjemahkan menjadi database dan isinya.

#### Valid dan tidak valid command
![WhatsApp Image 2023-06-25 at 17 20 40](https://github.com/hqlco/sampah/assets/86828535/7dccff4c-659b-463e-90c8-71da58b11907)
#### Bukti command berhasil
![8 FP sisop](https://github.com/hqlco/sampah/assets/86828535/428f4b71-f8de-444c-b57e-4455cccb9238)

## H. Error Handling
Error Handling sudah diterapkan pada masing-masing fungsi, sehingga bagian ini dapat ditemukan di setiap program baik itu di dalam ```client.c```, ```database.c```, maupun ```client_dump.c```.
## I. Containerization
1. Membuat Dockerfile, Docker Image, dan Docker Container <br>
    Berikut adalah isi dari Dockerfile yang sudah dibuat:
    ``` bash
    FROM ubuntu:latest
    RUN apt-get update && apt-get install -y \
    build-essential \
    wget unzip \
    wget zip \
    sudo
    #Set the working directory

    RUN useradd -m myuser && echo "myuser:password" | chpasswd && usermod -aG sudo myuser

    USER myuser

    WORKDIR /myuser

    COPY . /myuser

    CMD ["/bin/bash"]
    ```

    Penjelasan:
    - ```FROM ubuntu:latest``` : Menggunakan base image Ubuntu veri terbaru sebagai dasar untuk membangun image baru. Image Ubuntu ini menjadi dasar yang akan digunakan dalam container yang dihasilkan
    - ```RUN apt-get update && apt-get install -y \``` : Menjalankan perintah ```apt-get update``` untuk mengupdate daftar package yang tersedia dan ```apt-get install``` untuk menginstall package yang diperlukan dalam container. Package yang diinstall pada baris ini adalah sebagai berikut:
        - ```build-essential``` : Yaitu package yang berisi tools yang diperlukan untuk melakukan kompilasi software di Ubuntu
        - ```wget``` : Yaitu package yang digunakan untuk mengunduh file dari web melalui terminal
        - ```unzip``` : Yaitu package yang digunakan untuk mengekstrak file ZIP
        - ```zip``` : Yaitu package yang dibunakan untuk mengompresi file menjadi format ZIP
        - ```sudo``` : Yaitu package yang memberikan akses ke command-command superuser (root) kepada pengguna yang diberikan hak akses
    - ```RUN useradd -m myuser && echo "myuser:password" | chpasswd && usermod -aG sudo myuser``` : 
        - Membuat user baru dengan nama ```myuser``` menggunakan command ```useradd```
        - Kemudian, menggunakan perintah ```echo``` untuk menetapkan password ```password``` untuk user ```myuser``` dengan perintah ```chpasswd```
        - Terakhir, menggunakan perintah ```usermod``` untuk menambahkan user ```myuser``` ke grup ```sudo```, sehingga user tersebut memiliki akses superuser
    - ```USER myuser``` : Menetapkan user yang sedang digunakan dalam container menjadi ```myuser```. Dari titik ini, command-command yang dijalankan dalam Dockerfile akan dieksekusi dengan hak akses ```myuser``` bukan hak akses root
    - ```WORKDIR /myuser``` : Menetapkan working directory menjadi ```/myuser```. Direktori ini akan menjadi tempat eksekusi command-command selanjutnya dalam Dockerfile
    - ```COPY . /myuser``` : Menyalin semua file dan folder dari direktori saat ini (tempat Dockerfile berada) ke direktori ```myuser``` dalam container Docker
    - ```CMD ["/bin/bash"]``` : Menetapkan default command yang akan dijalankan ketika container dijalankan. Command ini menjalankan shell bash dalam container<br><br>

2. Publish Docker Image ke Docker Hub <br>
Dockerhub kami dapat diakses pada link yang ada di dalam file ```dockerhub.txt```. Berikut adalah linknya:
    ```bash
    https://hub.docker.com/r/leolco/storage-app
    ```

4. Membentuk *instance* sebanyak 5 menggunakan Docker Compose
Berikut merupakan ```docker-composer.yaml``` yang sudah dibuat:
    ```bash
    version: '3'
    services:
        Sukolilo:
            image: leolco/storage-app:latest
            networks:
            - mynetwork
            command: tail -f /dev/null
        Keputih:
            image: leolco/storage-app:latest
            networks:
            - mynetwork
            command: tail -f /dev/null
        Mulyos:
            image: leolco/storage-app:latest
            networks:
            - mynetwork
            command: tail -f /dev/null
        Semolowaru:
            image: leolco/storage-app:latest
            networks:
            - mynetwork
            command: tail -f /dev/null
        Gebang:
            image: cobacoba3:latest
            networks:
            - mynetwork
            command: tail -f /dev/null

    networks:
        mynetwork:
    ```

    Penjelasan:
    - ```version: '3'``` : Menentukan versi dari Docker Compose yang digunakan. Disini kami menggunakan Docker Compose versi 3.
    - ```services``` : Menyediakan konfigurasi layanan-layanan yang akan dijalankan dalam Docker Compose. Berikut adalah layanan-layanan yang diperintahkan:
        - ```Sukolilo```
        - ```Keputih```
        - ```Mulyos```
        - ```Semolowaru```
        - ```Gebang```
    - ```image: leolco/storage-app:latest``` : Mendefinisikan image yang akan digunakan untuk setiap layanan. Disini kami menggunakan image ```leolco/storage-app:latest``` sebagai dasar layanan app, yang menunjukkan bahwa layanan ini menggunakan image yang telah dibuat sebelumnya
    - ```networks:``` : Mendefinisikan jaringan yang akan digunakan. Disini kami menggunakan ```mynetwork``` sebagai jaringan yang akan digunakan
    - ```command: tail -f /dev/null``` : Menentukan perintah yang akan dijalankan ketika container dari layanan tersebut dijalanlan. Disini kami menggunakan command ```tail -f /dev/null``` untuk menjaga container tetap berjalan tanpa melakukan tindakan apapun.

### Dokumentasi Containerization
![image](https://github.com/Ikikk/jgndibuka/assets/90395116/9e3c5900-e7e5-41fd-8534-744fc664b688)

## J. Extra
Bagian Extra sudah diterapkan pada masing-masing fungsi baik itu di dalam ```client.c``` maupun ```client_dump.c```.

## Kendala
1. Reliabilitynya ngga selesai waktu praktikum, tapi disaranin sama mas nur dan akhirnya kelar yey
2. Timelinenya ngga pas, jadi agak semburat ngga karuan


-------------------
<p align="center">
  <img src="https://github.com/hqlco/sampah/assets/86828535/fcbd53cd-18dc-46af-9b03-a2dc8053971d" alt="Size Limit CLI" width="738">
</p>

**TanQ untuk semua kawan kawanku**

