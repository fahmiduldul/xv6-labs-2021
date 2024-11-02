#include "kernel/types.h"
#include "user/user.h"


int prime() {

  int first_num;
  read(0, &first_num, sizeof(int));
  printf("prime %d\n", first_num);

  int num;
  int p[2];
  int child_fd = -1;
  while(read(0, &num, sizeof(int))) {
    if ((num % first_num) == 0)
      continue;

    if (child_fd == -1) {
      pipe(p);
      if (fork() == 0) {
        close(0);
        dup(p[0]);
        close(p[0]);
        close(p[1]);
        prime();
        break;
      } else {
        close(2);
        dup(p[1]);
        close(p[1]);
      }
      child_fd = p[1];
    }
    write(2, &num, sizeof(int));
  }
  close(2);
  wait(0);
  return 0;
}


int main(int argc, char* argv[]) {
  int p[2];
  pipe(p);
  close(0);
  dup(p[0]);
  close(p[0]);

  for (int i = 2; i < 35; i++)
    write(p[1], &i, sizeof(int));

  close(p[1]);
  close(2);
  prime();
  exit(0);
}
