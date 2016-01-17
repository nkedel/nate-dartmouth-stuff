#include <assert.h>
#include <stdio.h>

#ifndef gamec         // if this is the main module, include the exports
#include "sockcom.h"  // for Dave's socket and client libraries, plus 
#include "gamec.h"    // curses
#include <curses.h>
#endif
 
#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

// kluge since curses.h for some reason doesn't define this!
extern "C" int napms(int ms);

// globally accessible "recover screen from curses" function
#ifdef DIE
void die(char *);
#else
extern void die(char *);
#endif

// object for Dave's network class.
extern Client net;

#include "player.h"
#include "globals.h"

// randomize(): set random seed.
void randomize(void);

// show_players(): places player icons (chars) on screen first time
void show_players(void);

// update_players(): move each player icon from old position to new
void update_players(void);

// init_curses(): duh
void init_curses(void);

// redraw_screen():
void redraw_screen(void);

// restore_screen():
void restore_screen(void);
void play_game(void);
void init_map(void);
void load_map(char *);
void game(void);

// 
void init_player(int num, const char *name, int x, int y);

// draw_map(): initially draw's the map
void draw_map(void);

// print_status(msg): prints a message to the plater's status line
void print_status(char *msg);

// print_local(msg): prints a message to the player's console
void print_local(char *msg);

// win(): beep and display "You win!"
void win(void);

// attack_succeeds(ac): simulates an attack on armor class ac returns true
//                      if the attack succeeds 
bool attack_succeeds(int ac);

// damage(dmg): returns a damage amound from 1..dmg
int  damage(int dmg);

// init_db(void):
void init_db(void);

// calls to C-type wrapper fro Dave's network classes:
// put(db#,rec#,field#,"data"): sets field, returns true if successful
// get(db#,rec#,field#): returns value in field.
extern bool put(int db, int rec, int field, char *data);
extern char *get(int db, int rec, int field);

// is_player(x,y): returns player# if the icon at x,y is a player, 0 otherwise
int is_player(int x, int y);

// is_validmove(x,y): returns true if x,y is a valid move (no icon except empty space there)
bool is_validmove(int x, int y);

// get_command():
bool get_command(void);

