#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <openssl/md5.h>

#define MAX_CREDENTIALS_LENGTH 200
#define MAX_USERNAME_LENGTH 100
#define MAX_PASSWORD_LENGTH 100
char username[MAX_USERNAME_LENGTH];
char password[MAX_PASSWORD_LENGTH];
char hashedPassword[MD5_DIGEST_LENGTH * 2 + 1];

static  const  char *dirpath = "/home/rulelulu/modul4/rahasia";
char pathFile[] = "/home/rulelulu/modul4/rahasia"; 
char groupName[] = "E01";

#define MAX_PATH_LENGTH 5000

static char resultFilePath[MAX_PATH_LENGTH]; // Path file result.txt

struct ExtensionCount { // These lines define maximum lengths for filename, path, and command length.
    char extension[256];
    int count;
};

struct ExtensionCount extcnt[1000]; // These lines declare and initialize variables pathFile and groupName as strings.
int numExtensions = 0;

void count_files(const char* directory) { // This line declares a function count_files that takes a const char* parameter directory.
    FILE* file = fopen("extension.txt", "w"); // This line opens a file named "extension.txt" in write mode and assigns the file pointer to the variable file.
    DIR* dp; // These lines declare a directory pointer variable dp and open the directory specified by the directory parameter.
    dp = opendir(directory);
    if (dp == NULL) {
        printf("Failed to open directory: %s\n", directory);
        return;
    }

    struct dirent* de; // These lines declare a dirent pointer variable de and a character array extension.
    char extension[256];

    while ((de = readdir(dp)) != NULL) { // This line starts a loop that iterates through each directory entry in the opened directory.
        struct stat st; // These lines declare a stat structure st and initialize it with zeros.
        memset(&st, 0, sizeof(st));

        st.st_ino = de->d_ino; // These lines assign the inode number and file type information from the directory entry to the stat structure.
        st.st_mode = de->d_type << 12;
    
        if ((strcmp(de->d_name, ".") != 0) && (strcmp(de->d_name, "..") != 0)) { // This condition checks if the current directory entry is not "." or "..".
            if (S_ISREG(st.st_mode)) { // This condition checks if the current entry is a regular file.
                char* ext = strrchr(de->d_name, '.');
                if (ext != NULL) { // This line finds the last occurrence of the '.' character in the file name and assigns the pointer to ext.
                    strcpy(extension, ext + 1); // This condition checks if a file extension is found.

                    int found = 0; // This line copies the file extension (excluding the '.') to the extension array.
                    for (int i = 0; i < numExtensions; i++) {
                        if (strcmp(extcnt[i].extension, extension) == 0) {
                            extcnt[i].count++;
                            found = 1;
                            break;
                        }
                    }
                    if (!found) { // These lines search for the extension in the extcnt array and increment its count if found. If not found, it adds a new entry to the array.
                        strcpy(extcnt[numExtensions].extension, extension);
                        extcnt[numExtensions].count = 1;
                        (numExtensions)++;
                    }
                }
            } else if (S_ISDIR(st.st_mode)) { // These lines add a new extension entry to the extcnt array if it was not found previously.
                char subdir[256];
                sprintf(subdir, "%s/%s", directory, de->d_name);
                count_files(subdir);
            }
        }
    }

    for (int i = 0; i < numExtensions; i++) { // This condition checks if the current entry is a directory. If so, it recursively calls the count_files function on the subdirectory.
        fprintf(file, "%s: %d\n", extcnt[i].extension, extcnt[i].count);
    }
    fclose(file); // This loop writes the extension count information to the opened file.
    closedir(dp);
}


void traverseDirectory(const char *path, FILE *file, int depth) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("Failed to open directory");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Print indentation based on depth
        for (int i = 0; i < depth; i++) {
            fprintf(file, "|  ");
        }

        // Print entry name
        fprintf(file, "|- %s\n", entry->d_name);

        char entryPath[MAX_PATH_LENGTH];
        snprintf(entryPath, sizeof(entryPath), "%s/%s", path, entry->d_name);

        struct stat st;
        if (lstat(entryPath, &st) == -1) {
            perror("Failed to get file status");
            continue;
        }

        if (S_ISDIR(st.st_mode)) {
            // Recursive call for directories
            traverseDirectory(entryPath, file, depth + 1);
        }
    }

    closedir(dir);
}

static  int  xmp_getattr(const char *path, struct stat *stbuf)
{
    int res;
    char fpath[1000];

    sprintf(fpath,"%s%s",dirpath,path);
    
    res = lstat(fpath, stbuf);

    if (res == -1) return -errno;

    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];
    if(strcmp(path,"/") == 0)
    {
        path=dirpath;

        sprintf(fpath,"%s",path);
    }
    else sprintf(fpath, "%s%s",dirpath,path);
    
    int res = 0;
    int fd = 0 ;

    (void) fi;

    fd = open(fpath, O_RDONLY);

    if (fd == -1) return -errno;

    res = pread(fd, buf, size, offset);

    if (res == -1) res = -errno;

    close(fd);

    return res;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) 
{
    char fpath[5000]; 
    char extension[256];
    if (strcmp(path, "/") == 0) { 
        path = pathFile;
        snprintf(fpath, sizeof(fpath), "%s", path);
    } else {
        snprintf(fpath, sizeof(fpath), "%s%s", pathFile, path);
    }

    int res = 0; 

    DIR *dp;
    struct dirent *de;
    (void) offset;
    (void) fi;

    dp = opendir(fpath); 

    if (dp == NULL) return -errno;
    
    FILE *resultFile = fopen(resultFilePath, "w");
    if (resultFile == NULL) {
        perror("Failed to create result file");
        closedir(dp);
        return -errno;
    }

    fprintf(resultFile, "%s\n", fpath);
    traverseDirectory(fpath, resultFile, 0);

    fclose(resultFile);

    while ((de = readdir(dp)) != NULL) { 
        struct stat st; 

        memset(&st, 0, sizeof(st));

        st.st_ino = de->d_ino; 
        st.st_mode = de->d_type << 12;

        if ((strcmp(de->d_name, ".") != 0) && (strcmp(de->d_name, "..") != 0)) { 
            if (S_ISDIR(st.st_mode)) { 
                if (strstr(de->d_name, "E01") == NULL) {
                    char oldFilePath[5000];
                    char newFilePath[5000];
                    sprintf(oldFilePath, "%s/%s", fpath, de->d_name);
                    sprintf(newFilePath, "%s/%s_E01", fpath, de->d_name);
                    if (access(newFilePath, F_OK) == -1) {
                        if (rename(oldFilePath, newFilePath) == -1) {
                            perror("Failed to rename directory");
                        }
                    }
                }
            } else if (S_ISREG(st.st_mode)) { 
                if (strstr(de->d_name, "E01") == NULL) {
                    char *ext = strrchr(de->d_name, '.');
                    if (ext != NULL) {
                        char oldFilePath[5000];
                        char newFilePath[5000];
                        sprintf(oldFilePath, "%s/%s", fpath, de->d_name);
                        sprintf(newFilePath, "%s/E01_%s", fpath, de->d_name);
                        if (access(newFilePath, F_OK) == -1) {
                            if (rename(oldFilePath, newFilePath) == -1) {
                                perror("Failed to rename file");
                            }
                        }
                    }
                }
            }
            else{
                printf("Error di s_is %s\n", de->d_name);
            }
        }
        else{
            // printf("Error %s di check dir\n", de->d_name);
        }

        res = filler(buf, de->d_name, &st, 0); 
        if (res != 0) break;
    }
    closedir(dp);
    return 0;
}



static int xmp_rename(const char *oldpath, const char *newpath) 
{
    char fpath[1000], fnewpath[1000]; 

    sprintf(fpath, "%s%s", pathFile, oldpath);
    sprintf(fnewpath, "%s%s", pathFile, newpath);

    int res = rename(fpath, fnewpath); 

    if (res == -1) return -errno;

    return 0;
}




static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .read = xmp_read,
    .rename = xmp_rename,
};

void DownloadFile(const char *url, const char *zipPath) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("Failed to fork\n");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        char *command = malloc(1024 * sizeof(char));
        snprintf(command, 1024, "wget --load-cookies /tmp/cookies.txt \"https://docs.google.com/uc?export=download&confirm=$(wget --quiet --save-cookies /tmp/cookies.txt --keep-session-cookies --no-check-certificate 'https://docs.google.com/uc?export=download&id=%s' -O- | sed -rn 's/.*confirm=([0-9A-Za-z_]+).*/\\1\\n/p')&id=%s\" -O %s && rm -rf /tmp/cookies.txt", url, url, zipPath);
        printf("Downloading file: %s\n", url);
        execl("/bin/sh", "sh", "-c", command, (char*)NULL);
        free(command);
    }
    else {
        int status;
        waitpid(pid, &status, 0);
    }
}

void unzipping(const char *zipPath)
{

    pid_t pidUnzip = fork();
    if(pidUnzip == 0){
        printf("Unzipping file : %s\n", zipPath);
        execl("/bin/unzip", "unzip", zipPath, NULL);
        exit(0);
    }
    else if (pidUnzip < 0)
    {
        perror("Failed to fork");
        exit(EXIT_FAILURE);
    } 
    else 
    {
        int status;
        waitpid(pidUnzip, &status, 0);

    }
} 

// Fungsi untuk melakukan hashing MD5 terhadap string input
void md5Hash(const char *input, char *output) {
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5((unsigned char*)input, strlen(input), digest);
    for(int i = 0; i < MD5_DIGEST_LENGTH; i++)
        sprintf(&output[i*2], "%02x", (unsigned int)digest[i]);
}

int isUsernameExists(const char *username) {
    // Buka file kredensial
    FILE *file = fopen("credentials.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 0;
    }

    // Cari kredensial sesuai username
    char line[MAX_CREDENTIALS_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        // Pisahkan username dan password dari kredensial
        char storedUsername[MAX_CREDENTIALS_LENGTH];
        sscanf(line, "%[^;];", storedUsername);

        // Cocokkan username dengan kredensial yang tersimpan
        if (strcmp(storedUsername, username) == 0) {
            fclose(file);
            return 1; // Username already exists
        }
    }

    fclose(file);
    return 0; // Username does not exist
}

// Fungsi untuk melakukan registrasi user baru dan menyimpannya dalam file
int registerUser(const char *username, const char *password) {
    // Periksa apakah username sudah ada
    if (isUsernameExists(username)) {
        printf("Username already exists.\n");
        return -1;
    }

    
    // Buat string kredensial dengan format "<username>;<password>"
    char cred[MAX_CREDENTIALS_LENGTH];
    snprintf(cred, sizeof(cred), "%s;%s\n", username, password);

    // Buka file untuk menambahkan kredensial baru
    FILE *file = fopen("credentials.txt", "a");
    if (file == NULL) {
        printf("Error opening file.\n");
        return -1;
    }

    // Tulis kredensial ke dalam file
    fputs(cred, file);

    // Tutup file
    fclose(file);

    return 0;
}

// Fungsi untuk memvalidasi kredensial user saat login
int validateCredentials(const char *username, const char *password) {
    // Buka file kredensial
    FILE *file = fopen("credentials.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 0;
    }

    // Cari kredensial sesuai username
    char line[MAX_CREDENTIALS_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        // Pisahkan username dan password dari kredensial
        char storedUsername[MAX_CREDENTIALS_LENGTH];
        char storedPassword[MAX_CREDENTIALS_LENGTH];
        sscanf(line, "%[^;];%s", storedUsername, storedPassword);

        // Cocokkan username dengan kredensial yang tersimpan
        if (strcmp(storedUsername, username) == 0) {
            // Cocokkan password yang diinput dengan password yang tersimpan
            if (strcmp(storedPassword, password) == 0) {
                fclose(file);
                return 1; // Kredensial valid
            } else {
                fclose(file);
                return 0; // Kredensial tidak valid (password salah)
            }
        }
    }

    fclose(file);
    return 0; // Kredensial tidak valid (username tidak ditemukan)
}


int  main(int  argc, char *argv[])
{

    //https://drive.google.com/uc?export=download&id=1oDgj5kSiDO0tlyS7-20uz7t20X3atwrq 
    const char *zipPath = "rahasia.zip";
    //DownloadFile("18YCFdG658SALaboVJUHQIqeamcfNY39a", "rahasia.zip");
    unzipping(zipPath);
    
    snprintf(resultFilePath, sizeof(resultFilePath), "%s/%s", dirpath, "result.txt");

    umask(0);
    
    int choice;
    
    printf("1. Register\n");
    printf("2. Login\n");
    
    printf("Enter your choice: ");
    scanf("%d", &choice);
    
    if(choice == 1) 
    {
    	    printf("Enter username: ");
            scanf("%s", username);
            printf("Enter password: ");
            scanf("%s", password);

            // Hashing password
            md5Hash(password, hashedPassword);

            // Register user
            if (registerUser(username, hashedPassword) == 0) {
                printf("Registration successful!\n");
            } else {
                printf("Registration failed.\n");
            }
    } else if (choice == 2 )
    {
	    printf("Enter username: ");
            scanf("%s", username);
            printf("Enter password: ");
            scanf("%s", password);

            // Hashing password
            md5Hash(password, hashedPassword);

            // Validate credentials
            if (validateCredentials(username, hashedPassword)) {
                printf("Login successful!\n");
                
                system("sudo docker-compose up -d");
                //printf("Docker-compose successful!\n");

                system("sudo docker start modul4_myservice_1");
                //printf("Docker Start successful!\n");
                count_files(pathFile);


                fuse_main(argc, argv, &xmp_oper, NULL);
                //printf("Fuse successful!\n");

                //system("sudo docker exec -it modul4_myservice_1 /bin/bash");
            

            } else {
                printf("Invalid credentials.\n");
            }
            
            
    } else {
    	    printf("Invalid choice.\n");
    }
    
}