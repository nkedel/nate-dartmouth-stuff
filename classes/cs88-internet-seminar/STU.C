#include <ctype.h>

char *strtoupper(char *str) {
  char *work = str;
  while (work!=NULL && work < str+128 && *work != 0) {
    *work = toupper(*work);
    work++;
    }
  return str;
  }
