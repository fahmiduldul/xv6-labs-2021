#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/fs.h"


int deep_find(char* path, char* filename) {
  int fd;
  struct stat st;
  struct dirent de;

  if ((fd = open(path, 0)) < 0) {
   fprintf(2, "find: cannot open fd\n");
   return 1;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat\n");
    return 1;
  }

  while (read(fd, &de, sizeof(de)) == sizeof(de)) {
    if (strlen(de.name) == 0
        || strcmp(de.name, ".") == 0
        || strcmp(de.name, "..") == 0)
      continue;

    int curFd;
    struct stat curStat;
    char curPath[100];

    memset(curPath, 0, 100);
    strcpy(curPath, path);
    curPath[strlen(path)] = '/';
    strcpy(&curPath[strlen(path) + 1], de.name);

    curFd = open(curPath, 0);
    fstat(curFd, &curStat);
    close(curFd);

    switch (curStat.type) {
      case T_FILE:
        if (strcmp(de.name, filename) == 0)
          printf("%s\n", curPath);
        break;

      case T_DIR:
        deep_find(curPath, filename);
        break;
    }
  }
  return 0;
}


int main(int argc, char* argv[]) {
  if (argc < 3) {
    fprintf(2, "find needs 2 args path and file name \n");
    exit(1);
  }

  if (deep_find(argv[1], argv[2]) != 0) {
    exit(1);
  }

  exit(0);
}