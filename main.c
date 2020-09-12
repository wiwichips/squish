#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
  int fd[2];
  int status = 0;
  pid_t cpid;
  printf("argc = %d\n", argc);

  // create the pipe
  pipe(fd);

  cpid = fork();

  // if its the child
  if (cpid == 0) {
    close(fd[0]); // close the read end
    dup2(fd[1], STDOUT_FILENO); // make output to the pipe
    execlp(argv[1], argv[1], (char *) NULL);
  }

  cpid = fork();

  // if its another child
  if (cpid == 0) {
    close(fd[1]); // close the write end
    dup2(fd[0], STDIN_FILENO); // get the info form the pipe
    execlp(argv[2], argv[2], (char *) NULL);
  }

  close(fd[0]);
  close(fd[1]);

  // reap children processes on children exit
  waitpid(-1, &status, 0);
  waitpid(-1, &status, 0);

  printf("processe %d finished\n", cpid);

  return 0;
}

// get number of programs (numPro = argc - 1)
// get number of pipes (numPipe = numPro - 1)
// 