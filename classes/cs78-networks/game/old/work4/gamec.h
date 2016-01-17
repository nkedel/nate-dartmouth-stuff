#include <String.h>
#include <iostream.h>

#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

// kluge since curses.h for some reason doesn't define this!
extern "C" int napms(int ms);

// until we get gamec.h
extern bool put(int db, int rec, int field, char *data);
extern char *get(int db, int rec, int field);
extern void die(char *);

#include "String.h"
#include "sockcom.h"
#include "player.h"
#include "monster.h"
#include "globals.h"

void randomize(void);
void move_monsters(void);
void init_monsters(void);
void show_monsters(void);
void update_monsters(void);
void show_players(void);
void update_players(void);
void init_curses(void);
void redraw_screen(void);
void restore_screen(void);
void play_game(void);
void init_map(char *map_path);
void init_player(int num, const char *name, int x, int y);
void draw_map(void);
void print_status(char *msg);
void win(void);

extern bool put(int db, int rec, int field, char *data);
extern char *get(int db, int rec, int field);

int is_player(int x, int y);
int is_monster(int x, int y);
bool is_validmove(int x, int y);
bool get_command(void);

