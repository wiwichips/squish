#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


/**
 * Pipes the output of one process to another. The other process
 * outputs to stdout.
 * 
 * If only one command is sent, it just executed the one command
 * without piping naything.
 */
int ipc(FILE* ofp, char*** listTokens, int nListTokens);