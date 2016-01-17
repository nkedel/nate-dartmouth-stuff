#include <assert.h>
#include <stdio.h>
#include <curses.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

// kluge since curses.h for some reason doesn't define this!
extern "C" int napms(int ms);

// until we get gamec.h
#idef DATABASE
bool put(int dbn, int rec, int field, char *data);
char *get(int dbn, int rec, int field);
#else
extern bool put(int db, int rec, int field, char *data);
extern char *get(int db, int rec, int field);
#endif

#ifdef DIE
void die(char *);
#else
extern void die(char *);
#endif

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
void init_map(void);
void load_map(char *);
void game(void);
void init_player(int num, const char *name, int x, int y);
void draw_map(void);
void print_status(char *msg);
void win(void);
bool attack_succeeds(int ac);
int  damage(int dmg);

extern bool put(int db, int rec, int field, char *data);
extern char *get(int db, int rec, int field);

int is_player(int x, int y);
int is_monster(int x, int y);
bool is_validmove(int x, int y);
bool get_command(void);

