#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "squish_run.h"

static int
printHelp(char *progname)
{
	printf("%s <options> [ <files> ]\n", progname);
	printf("\n");
	printf("Run scripts from files, or stdin if no file specified\n");
	printf("\n");
	printf("Options:\n");
	printf("-o <file> : place output in <file>\n");
	printf("-v        : be more verbose\n");
	printf("\n");
	exit (1);
}

int
main(int argc, char **argv)
{
	FILE *ofp = stdout;
	FILE *promptfp = stdout;
	int verbosity = 0;
	int status = 0;
	int i, ch;

	while ((ch = getopt(argc, argv, "hvo:")) != -1) {
		switch (ch) {
		case 'v':
			verbosity++;
			break;

		case 'o':
			if ((ofp = fopen(optarg, "w")) == NULL) {
				(void) fprintf(stderr,
						"failed opening output file '%s' : %s\n",
						optarg, strerror(errno));
				exit(-1);
			}
			break;

		case '?':
		case 'h':
		default:
			printHelp(argv[0]);
			break;
		}
	}

	/*
	 * skip arguments processed by getopt -- note that the first
	 * remaining item in argv is now in position 0
	 */
	argc -= optind;
	argv += optind;

	if (argc > 0) {
		for (i = 0; i < argc; i++) {
			status = runScriptFile(ofp, promptfp, argv[i], verbosity);
			if (status != 0)	exit (status);
		}
		exit (0);
	} 

	if (runScript(ofp, promptfp, stdin, "stdin", verbosity) < 0)
		return (-1);

	return 0;
}
