#include "w_run_command.h"

int
runCmd(FILE* ofp, char* name) {
  /**
   * FOLLOWING CODE SEGMENT WAS MODIFIED FROM THE ORIGINAL
   * pipeToMore.c
   * 
   * TODO: cite this code
   */
  int pipefds[2];
  int pid;
  int i;
  char nl = '\n';

  // create the pipe
  if (pipe(pipefds) < 0) {
    perror("pipe 1");
    exit(1);
  }

  // create child process
  if ((pid = fork()) < 0) {
    perror("fork 1");
    exit(1);
  }

  // child process
  if (pid == 0) {
    // close stdout, make fd 1 end of pipe, close old file descriptor?
    close (1);
    dup(pipefds[1]);
    close(pipefds[1]);

    // close the read end of the pipe
    close(pipefds[0]);

    execl("/bin/date", name, NULL);

    perror("exec 1");
    exit(1);
  }

  close(0);
  dup(pipefds[0]);
  close(pipefds[0]);

  char curr = 'a';
  while (curr != nl) {
    read(0, &curr, 1);
    printf("%c", curr);
  }

  close(pipefds[1]);

  wait(NULL);

  return 0;
}