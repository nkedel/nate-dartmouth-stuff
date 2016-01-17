#define DIE
#include "game.h"

// crash out, restore screen from curses. Not necessarily caused by
// character death, it was used in debugging, because curses left the
// console in a console-unfriendly state.

void die(char *message) {
  move(23,0);
  nocbreak();
  echo();
  nl();
  refresh();  
  printf("%s", message);
  assert(FALSE);
  }
