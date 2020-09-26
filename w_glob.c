#include "w_glob.h"

char** 
tokenGlob(char** tokens) {
  glob_t glist;
  char* item;
  char** exTokens = NULL;
  int exTokLen = 0;
  char* progName = tokens[0];

  // copy the name of the program to the start of the list
  exTokens = realloc(exTokens, sizeof(char*) * ++exTokLen);
  exTokens[0] = calloc(strlen(tokens[0]) + 1, sizeof(char));
  strcpy(exTokens[0], tokens[0]);
  ++tokens;

  // set all memory to 0 for the glob struct
  memset(&glist, 0, sizeof(glob_t));

  for (int i = 0; tokens[i] != NULL; i++) {
    glob(tokens[i], 0, NULL, &glist);

    // append all the globs to the master glob list
    for (int j = 0; j < glist.gl_pathc; j++) {
      // printf("%s\n", glist.gl_pathv[j]);
      exTokens = realloc(exTokens, sizeof(char*) * ++exTokLen);
      exTokens[exTokLen - 1] = calloc(strlen(glist.gl_pathv[j]) + 1, sizeof(char));
      strcpy(exTokens[exTokLen - 1], glist.gl_pathv[j]);
    }

    // preserve arguments
    if (glist.gl_pathc == 0 && tokens[i][0] == '-') {
      exTokens = realloc(exTokens, sizeof(char*) * ++exTokLen);
      exTokens[exTokLen - 1] = calloc(strlen(tokens[i]) + 1, sizeof(char));
      strcpy(exTokens[exTokLen - 1], tokens[i]);
    }    

    globfree(&glist);
  }

  // add NULL to the end of the list
  exTokens = realloc(exTokens, sizeof(char*) * ++exTokLen);
  exTokens[exTokLen - 1] = NULL;

  return exTokens;
}

// int
// main() {
//   char* tokens[5];
//   tokens[0] = "*.d";
//   tokens[1] = "makefile";
//   tokens[2] = "*.out";
//   tokens[3] = "*.pdf";
//   tokens[4] = NULL;
//   char** exTok = tokenGlob(tokens);

//   for (int i = 0; exTok[i] != NULL; i++) {
//     printf("   ~\t%s\n", exTok[i]);
//     free(exTok[i]);
//   }
//   free(exTok);

//   return 0;
// }

