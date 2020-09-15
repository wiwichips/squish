#include "w_pipe.h"
#include "w_run_command.h"

int
ipc(FILE* ofp, char*** listTokens, int nListTokens) {

  // // testing
  // for (int i = 0; i < nListTokens; i++) {
  //   printf("first word in list %d is %s\n", i, listTokens[i][0]);
  // }

  // printf("command 1 = %s\n", listTokens[0][0]);
  // printf("command 2 = %s\n", listTokens[1][0]);

  runCmd(ofp, listTokens[0]);

  runCmd(ofp, listTokens[1]);

}