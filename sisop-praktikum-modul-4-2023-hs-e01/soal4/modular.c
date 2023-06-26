#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/xattr.h>
#define sz 1024

static const char *log_file_path = "/home/ikikk/fs_module.log";
// static const char *modular_prefix = "module_";
static const char *dirpath = "/home/ikikk/sisop-praktikum-modul-4-2023-hs-e01/soal4.1/haloges";

char oldpath[1000];

struct log_info {
    const char *level;
    const char *cmd;
    const char *desc;
};


void log_command(const struct log_info *info) {
    FILE *file = fopen(log_file_path, "a");
    if (file) {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        struct tm *tm_info = localtime(&tv.tv_sec);

        fprintf(file, "%s::%02d%02d%02d-%02d:%02d:%02d::%s::%s\n", info->level,
                tm_info->tm_year % 100, tm_info->tm_mon + 1, tm_info->tm_mday,
                tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec,
                info->cmd, info->desc);
        fclose(file);
    }
}

void split(char *path){
    DIR *dir = opendir(path);
    struct dirent *ent;
    struct stat sb;

    while ((ent = readdir(dir)) != NULL){
        if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..")) continue;

        char curr[300];
        sprintf(curr, "%s/%s", path, ent->d_name);
        if (stat(curr, &sb) == -1) 
            continue;

        if (S_ISDIR(sb.st_mode)){
            split(curr);
        }
        else if (S_ISREG(sb.st_mode)){       
            FILE *input = fopen(curr, "rb");
            if (input == NULL) return;

            fseek(input, 0, SEEK_END);

            long fileSize = ftell(input);

            if (fileSize <= sz) continue;

            rewind(input);

            int count = (fileSize + sz - 1) / sz;

            for (int i = 0; i < count; i++){
                char num[5], tempPath[350];

                if (i < 10) {
                    sprintf(num, "00%d", i);
                }
                else if (i < 100) {
                    sprintf(num, "0%d", i);
                }
                else {
                    sprintf(num, "%d", i);
                }

                sprintf(tempPath, "%s.%s", curr, num);

                FILE *output = fopen(tempPath, "wb");
                if (output == NULL) return;
                
                char *buffer = (char*) malloc(sz);
                if (buffer == NULL) return;

                size_t bytes = fread(buffer, 1, sz, input);

                fwrite(buffer, 1, bytes, output);

                free(buffer);
                fclose(output);
            }
            fclose(input);

            char cmd[700];
            sprintf(cmd, "rm %s ", curr);
            system(cmd);
        }
    }
    closedir(dir);
}

void merge(char *path){
    DIR *dir = opendir(path);
    struct dirent *ent;
    struct stat sb;

    while ((ent = readdir(dir)) != NULL){
        if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..")) continue;

        char curr[300];
        sprintf(curr, "%s/%s", path, ent->d_name);
        if (stat(curr, &sb) == -1) continue;

        if (S_ISDIR(sb.st_mode)){
            merge(curr);
        }
        else if (S_ISREG(sb.st_mode) && strlen(curr) > 3 && !strcmp(curr + strlen(curr) - 3, "000")){         
            int count = 0;
            char dest[300], temp[350];
            memset(dest, '\0', sizeof(dest));
            memset(temp, '\0', sizeof(temp));

            strncpy(dest, curr, strlen(curr) - 4);
            while(true){
                if (count < 10) {
                    sprintf(temp, "%s.00%d", dest, count);
                }
                else if (count < 100) {
                    sprintf(temp, "%s.0%d", dest, count);
                }
                else {
                    sprintf(temp, "%s.%d", dest, count);
                }

                int res = stat(temp, &sb);
                if (res) break;

                char cmd[700];
                sprintf(cmd, "cat %s >> %s", temp, dest);
                system(cmd);

                sprintf(cmd, "rm %s", temp);
                system(cmd);
                count++;
            }
        }
    }
    closedir(dir);
}



static int xmp_getattr(const char *path, struct stat *stbuf){
    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);

    int res = lstat(fpath, stbuf);
    
    if (res == -1) {
        return -errno;
    }

    log_command(&(struct log_info){ "REPORT", "GETATTR", path});
    return 0;
}

static int xmp_access(const char *path, int mode){
    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);

    int res = access(fpath, mode);

    if (res == -1) {
        return -errno;
    }

    log_command(&(struct log_info){ "REPORT", "ACCESS", path });

    if (strstr(path, "/module_")){
        merge(fpath);
        strcpy(oldpath, fpath);
    }
    else if (strlen(oldpath)){
        split(oldpath);
        memset(oldpath, '\0', sizeof(oldpath));
    }

    return 0;
}

static  int  xmp_readlink(const char *path, char *buf, size_t size){
    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);

    int res = readlink(fpath, buf, size - 1);
    
    if (res == -1) {
        return -errno;
    }

    log_command(&(struct log_info){ "REPORT", "READLINK", path });
    return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi){
    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);

    DIR *dp;
    struct dirent *de;
    (void) offset;
    (void) fi;

    dp = opendir(fpath);
    if (dp == NULL) {
        return -errno;
    }

    while ((de = readdir(dp)) != NULL) {
        struct stat st;

        memset(&st, 0, sizeof(st));

        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;

        if ((filler(buf, de->d_name, &st, 0)) != 0) break;
    }
    closedir(dp);

    log_command(&(struct log_info){ "REPORT", "READDIR", path });
    return 0;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t dev){
    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);

    int res = mknod(fpath, mode, dev);
    
    if (res == -1) {
        return -errno;
    }
    
    log_command(&(struct log_info){ "REPORT", "MKNOD", path });
    return 0;
}

static int xmp_mkdir(const char *path, mode_t mode){
    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);

    int res = mkdir(fpath, mode);

    if (res == -1) {
        return -errno;
    }
    
    log_command(&(struct log_info){ "REPORT", "MKDIR", path });
    
    return 0;
}

static int xmp_symlink(const char *to, const char *from){
    char dest_path[1000], src_path[1000];
    sprintf(dest_path, "%s%s", dirpath, to);
    sprintf(src_path, "%s%s", dirpath, from);

    int res = symlink(dest_path, src_path);
    
    if (res == -1) {
        return -errno;
    }
    
    log_command(&(struct log_info){ "REPORT", "SYMLINK", dest_path });
    
    return 0;
}

static int xmp_unlink(const char *path){
    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);

    int res = unlink(fpath);
    
    if (res == -1) {
        return -errno;
    }

    log_command(&(struct log_info){ "FLAG", "UNLINK", path });
    return 0;
}

static int xmp_rmdir(const char *path){
    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);

    int res = rmdir(fpath);

    if (res == -1) {
        return -errno;
    }
    
    log_command(&(struct log_info){ "FLAG", "RMDIR", path });
    return 0;
}

static int xmp_rename(const char *from, const char *to){
    char src_path[1000], dest_path[1000];
    sprintf(src_path, "%s%s", dirpath, from);
    sprintf(dest_path, "%s%s", dirpath, to);

    int res = rename(src_path, dest_path);

    if (res == -1) {
        return -errno;
    }
    
    log_command(&(struct log_info){ "REPORT", "RENAME", src_path });

    char curr[1000], dup[1000];
    strcpy(curr, dirpath);
    strcpy(dup, to);
    char *token, *old = strrchr(from, '/'), *new = strrchr(to, '/');
    token = strtok(dup, "/");
    
    while(token != NULL) {
        strcat(curr, "/");
        strcat(curr, token);

        struct stat sb;

        if (stat(curr, &sb) == -1) {
            continue;
        }

        if (strlen(token) >= 7 && !strncmp(token, "module_", 7) && S_ISDIR(sb.st_mode)) {
            split(curr);
            break;
        }

        token = strtok(NULL, "/");
    }

    if (strcmp(old, new)) {
        struct stat sb;
        if ((stat(dest_path, &sb) != -1) && S_ISDIR(sb.st_mode) && (strlen(new) < 8 || strncmp(new, "/module_", 8))) merge(dest_path);
    }

    return 0;
}

static int xmp_link(const char *from, const char *to){
    char src_path[1000], dest_path[1000];
    sprintf(src_path, "%s%s", dirpath, from);
    sprintf(dest_path, "%s%s", dirpath, to);

    int res = link(src_path, dest_path);

    if (res == -1) {
        return -errno;
    }

    log_command(&(struct log_info){ "REPORT", "LINK", src_path });
    return 0;
}

static int xmp_chmod(const char *path, mode_t mode){
    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);

    int res = chmod(fpath, mode);

    if (res == -1) {
        return -errno;
    }
    
    log_command(&(struct log_info){ "REPORT", "CHMOD", path });
    return 0;
}

static int xmp_chown(const char *path, uid_t uid, gid_t gid){
    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);

    int res = chown(fpath, uid, gid);

    if (res == -1) {
        return -errno;
    }
    
    log_command(&(struct log_info){ "REPORT", "CHOWN", path });
    return 0;
}

static int xmp_truncate(const char *path, off_t size){
    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);

    int res = truncate(fpath, size);

    if (res == -1) {
        return -errno;
    }
    
    log_command(&(struct log_info){ "REPORT", "TRUNCATE", path });
    return 0;
}

static int xmp_utimens(const char *path, const struct timespec ts[2]){
    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);

    struct timeval tv[2];
    tv[0].tv_sec = ts[0].tv_sec;
    tv[0].tv_usec = ts[0].tv_nsec / 1000;
	tv[1].tv_sec = ts[1].tv_sec;
	tv[1].tv_usec = ts[1].tv_nsec / 1000;

    int res = utimes(fpath, tv);

    if (res == -1) {
        return -errno;
    }
    
    log_command(&(struct log_info){ "REPORT", "UTIMENS", path });
    return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi){
    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);

	int res = open(fpath, fi->flags);

	if (res == -1) {
        return -errno;
    }
    
	close(res);

    log_command(&(struct log_info){ "REPORT", "OPEN", path });
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);

    int res = 0;
    int fd = 0;
    (void) fi;

    fd = open(fpath, O_RDONLY);
    if (fd == -1) {
        return -errno;
    }

    res = pread(fd, buf, size, offset);
    if (res == -1) {
        return -errno;
    }

    close(fd);

    log_command(&(struct log_info){ "REPORT", "READ", path });
    return res;
}

static int xmp_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
	char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);

    int fd;
	int res;
	(void) fi;

	fd = open(fpath, O_WRONLY);
	if (fd == -1) {
        return -errno;
    }

	res = pwrite(fd, buf, size, offset);
	if (res == -1) {
        return -errno;
    }

	close(fd);

    log_command(&(struct log_info){ "REPORT", "WRITE", path });
	return res;
}

static int xmp_statfs(const char *path, struct statvfs *stbuf){
    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);

	int res = statvfs(fpath, stbuf);

	if (res == -1) {
        return -errno;
    }
    
    log_command(&(struct log_info){ "REPORT", "STATFS", path });
	return 0;
}

static int xmp_create(const char *path, mode_t mode, struct fuse_file_info *fi){
    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);

    int res = open(fpath, fi->flags, mode);

    if (res == -1) {
        return -errno;
    }
    
    log_command(&(struct log_info){ "REPORT", "CREATE", path });
    return res;
}

static int xmp_release(const char *path, struct fuse_file_info *fi){
    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);

	(void) fpath;
	(void) fi;

    log_command(&(struct log_info){ "REPORT", "RELEASE", path });
	return 0;
}

static int xmp_fsync(const char *path, int isdatasync, struct fuse_file_info *fi){
    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);

	(void) fpath;
	(void) isdatasync;
	(void) fi;

    log_command(&(struct log_info){ "REPORT", "FSYNC", path });
	return 0;
}

static int xmp_setxattr(const char *path, const char *name, const char *value, size_t size, int flags){
    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);

	int res = lsetxattr(fpath, name, value, size, flags);

	if (res == -1) {
        return -errno;
    }
    
    log_command(&(struct log_info){ "REPORT", "SETXATTR", path });
	return 0;
}

static int xmp_getxattr(const char *path, const char *name, char *value,size_t size){
    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);

	int res = lgetxattr(fpath, name, value, size);

	if (res == -1) {
        return -errno;
    }
    
    log_command(&(struct log_info){ "REPORT", "GETXATTR", path });
	return res;
}

static int xmp_listxattr(const char *path, char *list, size_t size){
    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);

	int res = llistxattr(fpath, list, size);

	if (res == -1) {
        return -errno;
    }
    
    log_command(&(struct log_info){ "REPORT", "LISTXATTR", path });
	return res;
}

static int xmp_removexattr(const char *path, const char *name){
    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);

	int res = lremovexattr(fpath, name);

	if (res == -1) {
        return -errno;
    }
    
    log_command(&(struct log_info){ "REPORT", "REMOVEXATTR", path });
	return 0;
}

static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .access = xmp_access,
    .readlink = xmp_readlink,
    .readdir = xmp_readdir,
    .mknod = xmp_mknod,
    .mkdir = xmp_mkdir,
    .symlink = xmp_symlink,
    .unlink = xmp_unlink,
    .rmdir = xmp_rmdir,
    .rename = xmp_rename,
    .link = xmp_link,
    .chmod = xmp_chmod,
    .chown = xmp_chown,
    .truncate = xmp_truncate,
    .utimens = xmp_utimens,
    .open = xmp_open,
    .read = xmp_read,
    .write = xmp_write,
    .statfs = xmp_statfs,
    .create = xmp_create,
    .release = xmp_release,
    .fsync = xmp_fsync,
    .setxattr = xmp_setxattr,
    .getxattr = xmp_getxattr,
    .listxattr = xmp_listxattr,
    .removexattr = xmp_removexattr,
};

int main(int  argc, char *argv[]){
    // inisialisasi log file dan modular prefix
    const char *user = getenv("USER");
    char log_file[256];
    sprintf(log_file, "/home/%s/fs_module.log", user);
    log_file_path = log_file;

    // inisialisasi FUSE
    umask(0);
    int ret = fuse_main(argc, argv, &xmp_oper, NULL);

    // ...

    return ret;
}
