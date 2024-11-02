#include "kernel/types.h"
#include "user/user.h"
#define MAXARGS 100
#define BUFFSIZE 600

char** split_string(char* str, char** target) {
  int i = 0;
  int wordpos = 0;
  int wordi = 0;
  while(str[i] != 0) {
    if (str[i] == ' ') {
      str[i] = 0;
      target[wordi] = &str[wordpos];
      wordpos = i + 1;
      wordi += 1;
    }

    if (str[i + 1] == 0) {
      target[wordi] = &str[wordpos];
    }

    i += 1;
  }

  return target;
}

void util(char** str_arr) {
  for(int i = 0; str_arr[i]; i++)
    printf("%s\n", str_arr[i]);
}

int execute(char** args, char** xargs) {
  uint n = strlen((char*)args);
  memcpy(&xargs[n + 1], xargs, (n + 1) * sizeof(char*));
  memcpy(xargs, args, (n + 1) * sizeof(char*));

  if (fork() == 0) {
    exec(xargs[1], &xargs[1]);
  } else {
    wait(0);
  }

  return 0;
}

int main(int argc, char* argv[]) {
  char buff[BUFFSIZE];
  memset(buff, 0, BUFFSIZE);
  char* xargs[MAXARGS];
  memset(xargs, 0, MAXARGS);

  int bi = 0;
  int n = 0;
  while((n = read(0, &buff[bi], BUFFSIZE))) {
    bi += n;
  }

  int wordstart = 0;
  int i = 0;
  char* str_arr[MAXARGS];
  while (buff[i] != 0) {
    if (buff[i] == '\n') {
      memset(str_arr, 0, MAXARGS);
      buff[i] = 0;
      split_string(&buff[wordstart], str_arr);
      wordstart = i + 1;

      execute(argv, str_arr);
      memset(str_arr, 0, MAXARGS);
    }

    i += 1;
  }

  exit(0);
}
