#include "w_redirection.h"

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

  while (getline(&line, &len, ifp) != -1)
    fprintf(ofp, "%s", line);
  
  free(line);

  return 0;
}