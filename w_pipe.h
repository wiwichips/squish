#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int ipc(FILE* ofp, char*** listTokens, int nListTokens);