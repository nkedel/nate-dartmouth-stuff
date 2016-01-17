// die "Crash and Burn"
#include <curses.h>

void die(char *message) {
  clear();
  refresh();
  printf("%s", message);
  exit(0);
  }
