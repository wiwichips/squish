#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int fileRedirect(FILE* ifp, FILE* ofp);
int redirection(FILE* ofp, char* filename, char* arrow);