#include "w_redirection.h"
#include "w_run.h"

// returns 0 if there are no redireciton characters
int
redirectTree(FILE* ofp, char** tokens)
{
  char* arrow = NULL;
  char* arrow2 = NULL;
  int fd[2];
  int i = 0;
  int nullLoc = 0;
  int nullLoc2 = 0;
  int arrowCount = 0;
  pid_t cpid;
  int status = 0;

  // iterate through looking for strings
  for (i = 0; tokens[i] != NULL; i++) {
    if (!strcmp(tokens[i], ">") || !strcmp(tokens[i], "<")) {
      if (arrowCount == 0) {
        arrow = tokens[i];
        tokens[i] = NULL;
        nullLoc = i;
      } else if (!strcmp(tokens[i], ">")) {
        arrow2 = tokens[i];
        tokens[i] = NULL;
        nullLoc2 = i;
      }
      arrowCount++;
    }
  }

  if (arrow == NULL) {
    return 0;
  }

  // perform redirection between a process and a file
  if (arrowCount == 1) {
    redirectChilds(ofp, tokens, arrow, nullLoc);
  }

  // put output into another file
  else {
    pipe(fd);

    cpid = fork();

    if (cpid == 0) {
      close(fd[0]);
      dup2(fd[1], STDOUT_FILENO);
      redirectChilds(ofp, tokens, arrow, nullLoc);
      exit(0);
    }

    cpid = fork();

    if (cpid == 0) {
      close(fd[1]);
      dup2(fd[0], STDIN_FILENO);
      redirection(ofp, tokens[nullLoc2+1], ">");
      exit(0);
    }

    close(fd[0]);
    close(fd[1]);

    waitpid(-1, &status, 0);
    waitpid(-1, &status, 0);
  }

  return 1;
}
  
int
redirectChilds(FILE* ofp, char** tokens, char* arrow, int nullLoc) {
  int fd[2];
  int status = 0;
  pid_t cpid;
  int exitStatus = 0;
  int statLoc = 0;

  // create the pipe
  pipe(fd);
  

  cpid = fork();
  if (cpid == 0 ) {
    // first child - the child that makes output
    close(fd[0]);
    dup2(fd[1], STDOUT_FILENO);
    if (arrow[0] == '>') {
      exitStatus = runCmd(ofp, tokens, &statLoc);
    } 
    else {
      exitStatus = redirection(ofp, tokens[nullLoc+1], "<");
    }
    exit(exitStatus);
  }

  cpid = fork();
  if (cpid == 0) {
    // second child - the child that reads output as input
    close(fd[1]);
    dup2(fd[0], STDIN_FILENO);
    if (arrow[0] == '>') {
      exitStatus = redirection(ofp, tokens[nullLoc+1], ">");
    }
    else {
      exitStatus = runCmd(ofp, &(tokens[0]), &statLoc);
    }

    exit(exitStatus);
  }

  // redirection(ofp, "zoey.txt", ">");

  close(fd[0]);
  close(fd[1]);

  waitpid(-1, &status, 0);
  waitpid(-1, &status, 0);
  
  return 1;
}

int
redirection(FILE* ofp, char* filename, char* arrow)
{
  if (!strcmp(arrow, "<")) {
    // piping from a file
    FILE* ifp = fopen(filename, "r");
    fileRedirect(ifp, ofp);
    fclose(ifp);
  } 
  else {
    // piping to a file
    FILE* ofp = fopen(filename, "w");
    fileRedirect(stdin, ofp);
    fclose(ofp);
  }
  return 0;
}

int
fileRedirect(FILE* ifp, FILE* ofp)
{
  size_t len;
  char* line;

  // initialize getline variables
  len = 0;
  line = NULL;

  if (ifp == NULL) {
    return -1;
  }

  while (getline(&line, &len, ifp) != -1) {
    fprintf(ofp, "%s", line);
  }
  
  free(line);

  return 0;
}