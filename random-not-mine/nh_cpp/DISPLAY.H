#ifndef DISPLAY_H
#define DISPLAY_H

#include "ansi.h"
#include <iostream.h>
class objectlist;

void movehome();
void message(char *msg);
void displaylist(objectlist *olist);
// later
//void displayinventory(hero *mon);

#endif
