#include "w_run_command.h"
#include "w_redirection.h"

int
runCmd(FILE* ofp, char** tokens, int* statLoc) {
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
  int exitingPID = -123;

  // create child process
  if ((pid = fork()) < 0) {
    perror("fork 1");
    exit(1);
  }

  // child process
  if (pid == 0) {
    // check if there are ">" or "<" characters, if so do redirection
    if (redirectTree(ofp, tokens)) {
      exit(1);
      // return 0;
    }

    // replce the process image with the command specified
    execvp(tokens[0], tokens);

    // perror("exec 1");
    exit(1);
  }

  exitingPID = wait(statLoc);
    
  return exitingPID;
}