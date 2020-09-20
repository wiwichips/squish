#include <stdio.h>

#include "w_run_command.h"
#include "w_change_dir.h"
#include "w_exit.h"
#include "w_pipe.h"


int run(FILE *ofp, char ** const tokens, int nTokens, int verbosity);