#include "w_pipe.h"
#include "w_run_command.h"

/**
 * Simple diagram of the process tree for squish calling pipes. 
 * 
 *   squish
 *     /\
 *    /  \
 *   /\   p3
 *  /  \   
 * p1  p2  
 */
int
ipc(FILE* ofp, char*** listTokens, int nListTokens) {
  int fd[2];
  int status = 0;
  pid_t cpid;
  int exitStatus = 0;
  int statLoc = 0;

  // if there is only one process, just run the command
  if (nListTokens == 1) {
    return runCmd(ofp, listTokens[0], &statLoc);
  }

  // create the pipe
  pipe(fd);

  // fork first child
  cpid = fork();

  if (cpid == 0 ) {
    // first child
    close(fd[0]);
    dup2(fd[1], STDOUT_FILENO);
    exitStatus = ipc(ofp, listTokens, nListTokens - 1);
    free(listTokens);
    _exit(exitStatus);
  }

  // fork second child
  cpid = fork();

  if (cpid == 0) {
    // second child
    close(fd[1]);
    dup2(fd[0], STDIN_FILENO);
    exitStatus = runCmd(ofp, listTokens[nListTokens - 1], &statLoc);
    free(listTokens);
    _exit(exitStatus);
  }

  close(fd[0]);
  close(fd[1]);

  // reap children processes on children exit
  waitpid(-1, &status, 0);
  waitpid(-1, &status, 0);

  return 0;
}