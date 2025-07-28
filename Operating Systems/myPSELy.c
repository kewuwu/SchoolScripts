#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

// ✅ Your constants and struct, slightly improved
#define MAX_COMMAND 256
#define MAX_TTY 32
#define MAX_WCHAN 32

typedef struct {
  unsigned int flags;
  char state;
  unsigned int uid;
  unsigned int pid;
  unsigned int ppid;
  int priority;
  int nice;
  unsigned int rss;
  unsigned int sz;
  char wchan[MAX_WCHAN];
  char tty[MAX_TTY]; // 🆕 We’ll use placeholder text for now
  unsigned long time;
  char cmd[MAX_COMMAND];
} entry;

// 🆕 Forward declaration
void processId(const char *pid);

// ✅ Your `main` function, fixed
int main() {
  DIR *dir;
  struct dirent *ent;

  if (!(dir = opendir("/proc"))) {
    perror("opendir");
    return 1;
  }

  // 🆕 Print headers
  printf("%-5s %-2s %-5s %-5s %-3s %-3s %-6s %-6s %-8s %-5s %-8s %s\n", "UID",
         "S", "PID", "PPID", "PR", "NI", "SZ", "RSS", "WCHAN", "TTY", "TIME",
         "CMD");

  while ((ent = readdir(dir)) != NULL) {
    if (!isdigit(ent->d_name[0]))
      continue;
    processId(ent->d_name);
  }

  closedir(dir);
  return 0;
}

// ✅ Your helper, rebuilt and extended
void processId(const char *pid) {
  char path[64], line[256];
  FILE *fp;
  entry e = {0};
  unsigned long utime = 0, stime = 0;

  // ✅ Read /proc/[pid]/stat
  snprintf(path, sizeof(path), "/proc/%s/stat", pid);
  if (!(fp = fopen(path, "r")))
    return;

  char comm[256]; // 🆕 Needed because (comm) has spaces
  fscanf(fp,
         "%u %s %c %u %*d %*d %d %*d %u %*u %*u %*u %*u %lu %lu %*d %*d %*d %d "
         "%d %*u %lu %lu",
         &e.pid, comm, &e.state, &e.ppid,
         &e.tty, // tty_nr
         &e.flags, &utime, &stime, &e.priority, &e.nice, &e.sz, &e.rss);
  fclose(fp);

  e.time = utime + stime;

  // 🆕 Read /proc/[pid]/status for UID and CMD
  snprintf(path, sizeof(path), "/proc/%s/status", pid);
  if ((fp = fopen(path, "r"))) {
    while (fgets(line, sizeof(line), fp)) {
      if (sscanf(line, "Uid:\t%u", &e.uid) == 1)
        continue;
      if (sscanf(line, "Name:\t%255s", e.cmd) == 1)
        continue;
    }
    fclose(fp);
  }

  // 🆕 Read /proc/[pid]/wchan
  snprintf(path, sizeof(path), "/proc/%s/wchan", pid);
  if ((fp = fopen(path, "r"))) {
    fscanf(fp, "%31s", e.wchan);
    fclose(fp);
  } else {
    strcpy(e.wchan, "-");
  }

  // 🆕 TTY placeholder — decoding tty_nr requires mapping major/minor to device
  // (complex)
  strcpy(e.tty, "?");

  // 🆕 Print formatted output
  printf("%-5u %-2c %-5u %-5u %-3d %-3d %-6u %-6u %-8s %-5s %-8lu %s\n", e.uid,
         e.state, e.pid, e.ppid, e.priority, e.nice, e.sz, e.rss, e.wchan,
         e.tty, e.time, e.cmd);
}
