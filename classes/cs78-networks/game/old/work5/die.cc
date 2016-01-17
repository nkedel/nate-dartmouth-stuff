#define DIE
// die "Crash and Burn"
#include "game.h"

void die(char *message) {
  clear();
  refresh();
  printf("%s", message);
  exit(0);
  }
