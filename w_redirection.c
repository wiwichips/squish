#include "w_redirection.h"
#include "w_run.h"

/**
 * This code is very ugly. Please look at w_pipe.c or any other files
 * instead of this one. Thank you.
 */

int
simpleRedirect(FILE* ofp, char** tokens) {
  char* cmd = tokens[0];
  char* inputFN = NULL;
  int inInd = 0;
  char* outputFN = NULL;
  int outInd = 0;
  char* tmp = NULL;
  pid_t cpid = 12345;
  int Ntokens = 0;

  // find the input and output files
  for (int i = 0; tokens[i]; i++) {
    if (!strcmp(tokens[i], ">")) {
      outputFN = tokens[i+1];
      outInd = i;
    } else if (!strcmp(tokens[i], "<")) {
      inputFN = tokens[i+1];
      inInd = i;
    }
    Ntokens++;
  }

  // if there is no file after a ">" specify here
  if (!inputFN && !outputFN) {
    perror("Error with redirection files");
    return -1;
  }

  if (inputFN && outputFN) {
    // redirectTree only takes in multiple redirection in this order < >
    // so I'm going to swap the input and output files if they're in > <

    if (outputFN < inputFN) {
      tokens[outInd+1] = inputFN;
      tokens[inInd+1] = outputFN;
      tokens[outInd][0] = '<';
      tokens[inInd][0] = '>';
    }

    redirectTree(ofp, tokens);
    
  } else if (inputFN || outputFN) {
    redirectTree(ofp, tokens);
  }
  
  return 0;
}

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
      _exit(0);
    }

    cpid = fork();

    if (cpid == 0) {
      close(fd[1]);
      dup2(fd[0], STDIN_FILENO);
      redirection(ofp, tokens[nullLoc2+1], ">");
      _exit(0);
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
    _exit(exitStatus);
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

    _exit(exitStatus);
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