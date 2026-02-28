#include "labster/core/file.h"
#include <stdio.h>

bool fileWrite(const char *path, Slice content) {
  FILE *file = fopen(path, "w");
  if (!file) return false;
  bool status = fwrite(content.pointer, 1, content.length, file) == content.length;
  fclose(file);
  return status;
}

