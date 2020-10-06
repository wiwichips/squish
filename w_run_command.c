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
  char** globTokens = NULL;

  // create child process
  if ((pid = fork()) < 0) {
    perror("fork 1");
    _exit(1);
  }

  // child process
  if (pid == 0) {
    // check if there are ">" or "<" characters, if so do redirection
    if (redirectTree(ofp, tokens)) {
      _exit(1);
    }

    // glob
    globTokens = tokenGlob(tokens);

    // replce the process image with the command specified
    execvp(tokens[0], globTokens);

    for (int i = 0; globTokens[i] != NULL; i++) {
      free(globTokens[i]);
    }
    free(globTokens);

    fprintf(stderr, "%s: command not found\n", tokens[0]);

    //perror("exec 1");
    _exit(1);
  }

  exitingPID = wait(statLoc);

  return exitingPID;
}