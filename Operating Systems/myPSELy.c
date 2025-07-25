#include <dirent.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#define MAX_COMMAND 256
#define MAX_TTY 32
#define MAX_WCHAN 32

typedef struct {
    char state;
    unsigned int uid;
    unsigned int pid;
    unsigned int ppid;
    unsigned int c;
    int priority;
    int nice;
    unsigned int rss;
    unsigned int sz;
    char wchan;
    char tty;
    unsigned long time;
    char cmd;
   
} entry;

int main() {
    DIR *dir;
    struct dirent *ent;
    char temp[13];
    if (!(dir = opendir("/proc"))) {
        perror("opendir");
        return 1;
    }
    return 0;
    while((ent = readdir(dir)) != NULL) {
        if(!isdigit(ent->d_name[0])) {
            continue;
        }
        processId(ent->d_name, temp);
    }
    closedir(dir);
    return 0;
}

void processId(const char *pid, char *temp) {
    char path[64];
    char line[128];
    FILE *fp;
    entry e;

    snprintf(path, sizeof(path), "/proc/%s/status", pid);

    if (!(fp = fopen(path, "r"))) {
        perror("fopen");
        return;
    }
    readdir(fp);
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "Name:\t%s", e.cmd) == 1) {
            printf("%s\n", e.cmd);
        }
    }

  

    fclose(fp);
}