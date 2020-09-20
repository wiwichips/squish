#include "w_run.h"

int
run(FILE *ofp, char ** const tokens, int nTokens, int verbosity)
{
  char*** listTokens = malloc(sizeof(char**));
	listTokens[0] = tokens;
	int numLists = 1;
	int statLoc = 0;
  int ret;


	if (!strcmp(tokens[0], "cd")) {
		ret = cd(tokens[1]);

	} else if (!strcmp(tokens[0], "exit")) {
		free(listTokens);
		exitProgram(tokens, nTokens);

	} else {
		for (int i = 1; i < nTokens; i++) {
			if (!strcmp(tokens[i], "|")) {
				listTokens = realloc(listTokens, sizeof(char**) * ++numLists);
				listTokens[numLists-1] = &tokens[i+1];
				tokens[i] = NULL;
			}
		}
		

		if (numLists > 1) {
			ret = ipc(ofp, listTokens, numLists);

		} else {
			ret = runCmd(ofp, tokens, &statLoc);

			/**
			 * FOLLOWING CODE SEGMENT IS COPIED FORM THE ORIGINAL vssh.c
			 * 
			 * TODO: cite this code properly
			 */
			if(WIFEXITED(statLoc)) {
				fprintf(ofp, "Child (%d) exitted -- ", ret);
				if (statLoc == 0) {
					fprintf(ofp, "success");
				} else {
					fprintf(ofp, "failure");
				}
				fprintf(ofp, "(%d)\n", statLoc);
			} else {
				fprintf(ofp, "Child (%d) did not exit (crashed?)\n", ret);
			}

		}
	}

	free(listTokens);
	return ret;
}