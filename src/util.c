#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "util.h"

char* strip_prefix(const char *str, const char prefix[])
{
  int offset = strlen(prefix);
  int len = (strlen(str) - offset + 1) * sizeof(char);
  char *ret = malloc(len);
  sprintf(ret, "%s", str + offset * sizeof(char));
  return ret;
}
