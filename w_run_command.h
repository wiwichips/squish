#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "w_glob.h"

int runCmd(FILE* ofp, char** tokens, int* statLoc);