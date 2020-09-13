#include "w_run_command.h"

int
runCmd(FILE* ofp, char** tokens) {
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

  // create child process
  if ((pid = fork()) < 0) {
    perror("fork 1");
    exit(1);
  }

  // child process
  if (pid == 0) {
    // replce the process image with the command specified
    execvp(tokens[0], tokens);

    // perror("exec 1");
    exit(1);
  }

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