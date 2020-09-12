#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <assert.h>

/*
 * Include header for our tokenization routines
 */
#include "squish_tokenize.h"


#if defined(__linux__)
/* no definition of this in Debian yet */

static size_t
strlcat(char *dest, const char *src, size_t bufferSize)
{
	size_t currentSize = strnlen(dest, bufferSize);
	size_t maxNtoCopy = bufferSize - currentSize;
	size_t nCopied;

	for (nCopied = 0 ; nCopied < maxNtoCopy && src[nCopied] != '\0' ; nCopied++)
		dest[currentSize + nCopied] = src[nCopied];

	dest[currentSize + nCopied] = '\0';

	return currentSize + nCopied;
}

static size_t
strlcpy(char *dest, const char *src, size_t bufferSize)
{
	size_t nCopied;

	for (nCopied = 0 ; nCopied < bufferSize && src[nCopied] != '\0' ; nCopied++)
		dest[nCopied] = src[nCopied];

	dest[nCopied] = '\0';

	return nCopied;
}

#endif



/**
 * Trim blanks at beginning/end of string
 */
static char *
trim(char *data)
{
	char *endp;

	/** skip leading spaces */
	while (isblank(*data))
		*data++ = '\0';

	/**
	 * find the end, and trim off trailing spaces while we still
	 * have a string
	 */
	endp = &data[strlen(data)-1];
	while (endp >= data && isblank(*endp))
		*endp-- = '\0';

	return data;
}

static char *
operatorChar(int c)
{
	if (c == '|')	return "|";
	if (c == '&')	return "&";
	if (c == '=')	return "=";
	return "{UNKNOWN OPERATOR}";
}

/**
 * Get the next token, using blanks or operators, and updating the
 * buf pointer to the next location
 */
static char *
getTokenFromBuffer(char **buffer, int *pushedChar)
{
	const char *OPERATORS = "|&=";
	char *tokenString = (*buffer);
	int i, endch;


	/** did we arrive at the end of the string? */
	if (*pushedChar == '\n' || *pushedChar == '#')	return NULL;

	/** do we have an operator left over from last time? */
	if (*pushedChar != 0 && strchr(OPERATORS, *pushedChar) != NULL) {
		int tmpPush = *pushedChar;
		*pushedChar = 0;
		return operatorChar(tmpPush);
	}

	/*
	 * If we didn't find an operator, then pushedChar should be blank
	 * or null, and may be ignored -- but make sure
	 */
	assert(*pushedChar == 0 || *pushedChar == '\n' || isblank(*pushedChar));


	/** eat any remaining leading blanks */
	while (isblank(*tokenString)) tokenString++;

	/** have we now found the end of the string? */
	if (*pushedChar == '\n'
			|| tokenString[0] == '\n'
			|| tokenString[0] == '#') {
		*pushedChar = 0;
		return NULL;
	}

	/*
	 * while we still have a string, and not at a blank character
	 * or an operator, consider it part of the token
	 */
	for (i = 0;
			tokenString[i] != '\0'
				&& ( ! isblank(tokenString[i]) )
				&& ( tokenString[i] != '\n' )
				&& ( strchr(OPERATORS, tokenString[i]) == NULL);
						i++)
		; /* do nothing - all work is done in the test */

	/**
	 * We came to the end of the token -- figure out why.  If we
	 * have not loaded anything yet, then return any operator
	 * found, otherwise push it and get it next time
	 */
	if ((i == 0) && (strchr(OPERATORS, tokenString[0]))) {

		/* clear pushed character */
		*pushedChar = 0;

		/* update the buffer to the next char */
		(*buffer) = &tokenString[1];

		return operatorChar(tokenString[0]);
	}

	/**
	 * save our next character to return next time and clean up token 
	 */
	/* remember our terminal character */
	*pushedChar = tokenString[i];
	/* update the buffer to our new start */
	(*buffer) = &tokenString[i+1];
	/* snip off the leading token */
	tokenString[i] = '\0';

	return tokenString;
}


/**
 * Split the input line into tokens, based on whitespace.
 *
 * This is done naively based on looking for blanks/tabs.
 * If quotes were supported, this is where the handling
 * for them would go.
 */
static int
loadTokens(char **tokens, int maxtokens, char *buf, int verbosity)
{
	char **loadp;
	int pushChar = 0;

	/** set our loader pointer to use the token list */
	loadp = tokens;

	/** iterate through all the tokens we can get */
	while ((*(loadp++) = getTokenFromBuffer(&buf, &pushChar)) != NULL) {

		/** make sure that we are not overflowing the buffer */
		if ( (loadp - tokens) >= maxtokens ) {
			fprintf(stderr, "Too many tokens (max %d)\n", maxtokens);
			return -1;
		}
		if (verbosity > 0)	fprintf(stderr, " + :token[%s]\n", loadp[-1]);
	}

	/** use pointer arithmetic to determine how many tokens we loaded */
	return (loadp - tokens - 1);
}

/**
 * Read a line from an input stream, separating into tokens
 */
int
parseLine(
		FILE *ifp,
		char **tokens, int maxtokens,
		char *buf, int bufsiz,
		int verbosity
	)
{
	int i, nTokens = -1, keepGoing = 1;
	char *loc;

	while (nTokens <= 0 && keepGoing != 0) {

		/** read line into buffer of managed size */
		if (fgets(buf, bufsiz, ifp) == NULL) {
			return 0;
		}

		/** make sure that the entire line fit into the buffer */
		if (buf[strlen(buf)-1] != '\n') {
			fprintf(stderr, "Line too long - buffer exceeded\n");
			return (-1);
		}

		/** load up the tokens, including any | and & tokens */
		nTokens = loadTokens(tokens, maxtokens, buf, verbosity);
	}

	/** return the number of tokens loaded */
	return nTokens;
}

/**
 * print the tokens to an output stream
 */
void
fprintfTokens(FILE *fp, char ** const tokens, int useQuotes)
{
	int i;

	if (tokens[0] == NULL) {
		printf("NULL token list\n");
		return;
	}

	fputs(tokens[0], fp);
	for (i = 1; tokens[i] != NULL; i++) {
		fputc(' ', fp);
		if (useQuotes) fputc('\'', fp);
		fputs(tokens[i], fp);
		if (useQuotes) fputc('\'', fp);
	}
	fputc('\n', fp);
}


/**
 * Put the tokens into the given buffer
 */
char *
tokensToString(char *buf, int bufsiz, char ** const tokens, int useQuotes)
{
	const char *overflowMarker = "//++";
	int i;

	/** ensure that the buffer size given is greater than zero */
	assert(bufsiz > 0);

	if (tokens[0] == NULL)	return ("NULL token list");

	/** start with an empty buffer */
	*buf = 0;

	/**
	 * Use strlcat to safely add portions to the buffer
	 * Note that this is about the only time that you will ever
	 * see 'goto' used -- here it is effectively being used
	 * much like a try/catch block to handle exceptional conditions
	 * and clean up nicely while aborting the regular processing
	 */
	if (strlcat(buf, tokens[0], bufsiz) >= bufsiz)	goto OVERFLOW;
	for (i = 1; tokens[i] != NULL; i++) {
		if (strlcat(buf, " ", bufsiz) >= bufsiz)	goto OVERFLOW;
		if (useQuotes) if (strlcat(buf, "'", bufsiz) >= bufsiz)
			goto OVERFLOW;
		if (strlcat(buf, tokens[i], bufsiz) >= bufsiz)	goto OVERFLOW;
		if (useQuotes) if (strlcat(buf, "'", bufsiz) >= bufsiz)
			goto OVERFLOW;
	}

	return buf;


	/**
	 * local exception to cleanup routine - put in overflow marker,
	 * overwriting the end of the string to do so
	 */
OVERFLOW:
	strlcpy(&buf[bufsiz - strlen(overflowMarker)], overflowMarker, bufsiz);
	return buf;
}

