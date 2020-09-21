#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int fileRedirect(FILE* ifp, FILE* ofp);
int redirection(FILE* ofp, char* filename, char* arrow);
int redirectTree(FILE* ofp, char** tokens);