#ifndef __SQUISH_RUN_SCRIPT_HEADER__
#define __SQUISH_RUN_SCRIPT_HEADER__

int runScript(FILE *ofp, FILE *pfp, FILE *ifp, const char *filename, int verbosity);
int runScriptFile(FILE *ofp, FILE *pfp, const char *filename, int verbosity);

#define	LINEBUFFERSIZE	(1024 * 8)
#define	MAXTOKENS		(1024 * 2)

#define	MAXCOMMANDSINPIPE	128

#define	PROMPT_STRING	"(squish) % "

#endif /* __SQUISH_RUN_SCRIPT_HEADER__ */
