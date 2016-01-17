#define DIE
// die "Crash and Burn"
#include "game.h"

void die(char *message) {
  move(23,0);
  nocbreak();
  echo();
  nl();
  refresh();  
  printf("%s", message);
  assert(FALSE);
  }
