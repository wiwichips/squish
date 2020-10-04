#include <stdio.h>

#include "w_run_command.h"
#include "w_change_dir.h"
#include "w_exit.h"
#include "w_pipe.h"
#include "w_glob.h"
#include "w_redirection.h"

int run(FILE *ofp, char ** const tokens, int nTokens, int verbosity);
int printCmd(FILE *ofp, char** tokens);
