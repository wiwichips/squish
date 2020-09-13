#include "w_exit.h"

int exitProgram(char** tokens, int nTokens) {
  int exitCode;
  // parse the exit code from the tokens
  if (nTokens > 1) {
    exitCode = atoi(tokens[1]);
  } else {
    exitCode = 0;
  }
  exit(exitCode);
  return 0;
}