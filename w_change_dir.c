#include "w_change_dir.h"

int cd(char* path) {
  return chdir(path);
}