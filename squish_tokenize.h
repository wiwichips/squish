#ifndef __SQUISH_TOKENIZE_HEADER__
#define __SQUISH_TOKENIZE_HEADER__


/* the types of lines that we support */
#define	LINE_ASSIGN		0x01
#define	LINE_EXECUTE	0x02

/**
 * load a line from the input file, placing the data into the memory
 * indicated by buf, and breaking up into a NULL terminated set of
 * tokens indicated by tokens
 */
int parseLine(FILE *ifp,
		char **tokenPointers, int maxTokens,
		char *lineBuffer, int lineBufferSize,
		int verbosity);

void fprintfTokens(FILE *fp, char ** const tokens, int useQuotes);

char *tokensToString(char *buf, int bufsiz, char ** const tokens, int useQuotes);

#endif /* __SQUISH_TOKENIZE_HEADER__ */

