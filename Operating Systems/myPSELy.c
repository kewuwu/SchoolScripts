#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_COMMAND 256
#define MAX_WCHAN   32
#define MAX_TTY     32
#define MAX_TOKENS  52


typedef struct {
    char         state;
    unsigned int uid;
    unsigned int pid;
    unsigned int ppid;
    unsigned int c;
    int          priority;
    int          nice;
    unsigned int rss;
    unsigned int sz;
    char         wchan[MAX_WCHAN];
    char         tty[MAX_TTY];
    unsigned long time;
    char         cmd[MAX_COMMAND];
} entry;

void processId(const char *pid);
char *format_time(unsigned long total_secs);


int main(void) {
    
    DIR *dir;
    struct dirent *ent;
    printf("pid\tstate\tuid\tppid\tc\tpri\tnice\trss\tsz\twchan\ttty\ttime\tcmd\n");

    if (!(dir = opendir("/proc"))) {
        perror("opendir");
        return 1;
    }

    while ((ent = readdir(dir)) != NULL) {
        if (!isdigit(ent->d_name[0]))
            continue;
        processId(ent->d_name);
    }

    closedir(dir);
    return 0;
}

void processId(const char *pid) {
  long ticks = sysconf(_SC_CLK_TCK);
  char path[64], line[1024];
  FILE *fp;
  entry e = {0};
  char *tokens[MAX_TOKENS];
  int i = 0;

  e.pid = atoi(pid);
   e.c = 0;
  snprintf(path, sizeof(path), "/proc/%s/stat", pid);
  if (!(fp = fopen(path, "r"))) {
      return;
  }
  if (fgets(line, sizeof(line), fp)) {
      char *tok = strtok(line, " ");
      while (tok && i < MAX_TOKENS) {
          if(i == 1) {
              size_t len = strlen(tok);
              tok[len - 1] = '\0';
              tokens[i] = tok + 1;
          }
          else {
              tokens[i] = tok;
          }
          tok = strtok(NULL, " ");
          i++;
      }
      
      
      strncpy(e.cmd, tokens[1], sizeof(e.cmd)-1);
      e.cmd[sizeof(e.cmd)-1] = '\0';
      e.state = tokens[2][0];
      e.ppid = atoi(tokens[3]);
      strncpy(e.tty, tokens[6], sizeof(e.tty)-1);
      e.tty[sizeof(e.tty)-1] = '\0';
      e.time = (atoi(tokens[13]) + atoi(tokens[14])) / ticks;
      e.priority = atoi(tokens[17]);
      e.nice = atoi(tokens[18]);
      strncpy(e.wchan, tokens[34], sizeof(e.wchan)-1);
      e.wchan[sizeof(e.wchan)-1] = '\0';

  }
  fclose(fp);

  snprintf(path, sizeof(path), "/proc/%s/status", pid);
  if (!(fp = fopen(path, "r"))) {
      return;
  }
  bool foundUID = false, foundSize = false, foundRSS = false;
  while (fgets(line, sizeof(line), fp)) {
      char *colon = strchr(line, ':');
      *colon = '\0';
      char*key = line;
      char*value = colon + 1;
      while (*value == '\t' || *value == ' ') {
          value++;
      }
      char *end = strchr(value, '\n');
      if(end) {
          *end = '\0';
      }
      if (!strcmp(key, "Uid")) {
          unsigned int r_uid;
          if (sscanf(value, "%u", &r_uid) == 1) {
              e.uid = r_uid;
              foundUID = true;
          }
      }
      else if (!strcmp(key, "VmSize")) {
          e.sz = atoi(value);
          foundSize = true;
      }
      else if (!strcmp(key, "VmRSS")) {
          e.rss = atoi(value);
          foundRSS = true;
      }
      if (foundUID && foundSize && foundRSS) {
          break;
      }
  }
  fclose(fp);
  char *time = format_time(e.time);
  printf("%u\t%c\t%u\t%u\t%u\t%d\t%d\t%u\t%u\t%s\t%s\t%s\t%s\n",
    e.pid,
    e.state,
    e.uid,
    e.ppid,
    e.c,
    e.priority,
    e.nice,
    e.rss,
    e.sz,
    e.wchan,
    e.tty,
    time,         
    e.cmd);

    free(time);
  }
  char *format_time(unsigned long total_secs) {
    char *buf = malloc(10);
    unsigned int hours   = total_secs / 3600;
    unsigned int minutes = (total_secs % 3600) / 60;
    unsigned int seconds = total_secs % 60;
    snprintf(buf,10, "%02u:%02u:%02u", hours, minutes, seconds);
    return buf;
}

