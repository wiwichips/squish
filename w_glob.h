#include <stdio.h>
#include <glob.h>
#include <stdlib.h>
#include <string.h>

/**
 * Must free the list and each string in the list.
 */
char** tokenGlob(char** tokens);
