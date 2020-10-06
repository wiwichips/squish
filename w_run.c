#include "w_run.h"

int
run(FILE *ofp, char ** const tokens, int nTokens, int verbosity)
{
  char*** listTokens = malloc(sizeof(char**));
	listTokens[0] = tokens;
	int numLists = 1;
	int numRedir = 0;
	int statLoc = 0;
  int ret;

	// print the tokens
	printCmd(ofp, tokens);

	if (!strcmp(tokens[0], "cd")) {
		free(listTokens);
		ret = cd(tokens[1]);

	} else if (!strcmp(tokens[0], "exit")) {
		free(listTokens);
		exitProgram(tokens, nTokens);

	} else {
    // populate the array of strings seprated by "|" characters
		for (int i = 1; i < nTokens; i++) {
			if (tokens[i] && !strcmp(tokens[i], "|")) {
				listTokens = realloc(listTokens, sizeof(char**) * ++numLists);
				listTokens[numLists-1] = &tokens[i+1];
				tokens[i] = NULL;
			}

			if (tokens[i] && (!strcmp(tokens[i], "<") || !strcmp(tokens[i], ">"))) {
				numRedir++;
			}
		}

		if (numRedir) {
			free(listTokens);
			listTokens = NULL;
			ret = simpleRedirect(ofp, tokens);

		} else if (numLists > 1) {
			ret = ipc(ofp, listTokens, numLists);
		} else {
			ret = runCmd(ofp, tokens, &statLoc);

			/**
			 * FOLLOWING CODE SEGMENT IS COPIED FORM THE ORIGINAL vssh.c
			 * 
			 * TODO: cite this code properly
			 */
			if(WIFEXITED(statLoc)) {
				fprintf(ofp, "Child (%d) exited -- ", ret);
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

		if (listTokens) {
			free(listTokens);
		}		
	}

	return ret;
}

int
printCmd(FILE *ofp, char** tokens) {
	char** globTokens = tokenGlob(tokens);
	fprintf(ofp, "+");
	for (int i = 0; globTokens[i] != NULL; i++) {
		fprintf(ofp, " \"%s\"", globTokens[i]);
		free(globTokens[i]);
	}
	free(globTokens);
	fprintf(ofp, "\n");
	return 0;
}
