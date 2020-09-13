#include "w_run_command.h"

int
runCmd(FILE* ofp, char* name) {
  /**
   * FOLLOWING CODE SEGMENT WAS MODIFIED FROM THE ORIGINAL
   * pipeToMore.c
   * 
   * TODO: cite this code properly
   */
  int pipefds[2];
  int pid;
  int i;
  pid_t child = -1;
  int statLoc = 0;
  int exitingPID = -123;

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

    // replce the process image with the command specified
    execlp(name, name, (char *) NULL);

    // on failure, write the null character to the pipe and exit
    char test = '\0';
    write(1, &test, 1);

    // perror("exec 1");
    exit(1);
  }

  char curr = 'a';
  while (curr != '\n' && curr != '\0') {
    read(pipefds[0], &curr, 1);
    printf("%c", curr);
  }

  close(pipefds[1]);

  exitingPID = wait(&statLoc);

  /**
   * FOLLOWING CODE SEGMENT IS COPIED FORM THE ORIGINAL vssh.c
   * 
   * TODO: cite this code properly
   */
  if(WIFEXITED(statLoc)) {
    fprintf(ofp, "Child (%d) exitted -- ", exitingPID);
    if (statLoc == 0) {
      fprintf(ofp, "success");
    } else {
      fprintf(ofp, "failure");
    }
    fprintf(ofp, "(%d)\n", statLoc);
  } else {
    fprintf(ofp, "Child (%d) did not exit (crashed?)\n", exitingPID);
  }
  
  return statLoc;
}