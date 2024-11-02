#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
  int p[2];
  if (pipe(p) != 0)
    exit(-1);
  if (fork() == 0) { // child process
    int pid = getpid();

    char* buf[1];
    read(p[0], buf, 1);

    fprintf(1, "%d: received ping\n", pid);
    write(p[1], buf, 1);
    close(p[0]);
    close(p[1]);
    exit(0);
  } else { //parent
    int pid = getpid();

    char* buf[1];
    write(p[1], buf, 1);

    close(p[1]);
    read(p[0], buf, 1);
    fprintf(1, "%d: received pong\n", pid);

    close(p[0]);
    exit(0);
  }

}