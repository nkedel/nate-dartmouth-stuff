#include <stdio.h>
#include <curses.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

const int colors[7] = { COLOR_RED, COLOR_GREEN, COLOR_YELLOW, COLOR_BLUE,
                        COLOR_MAGENTA, COLOR_CYAN, COLOR_WHITE };

class Player {
private:
  int x, y;
  int hp, xp, ac, dmg, kills;
  char name[32];
  char weapon[32];
  char armor[32];
public:
  void setXY(int newx, int newy) { x = newx; y = newy; }
  void setX(int newx) { x = newx; }
  void setY(int newy) { y = newy; }
  void getXY(int& newx, int& newy) { newx = x; newy = y; }
  int getX(void) { return x; }
  int getY(void) { return y; }
  void setHP(int newHP)   { hp = newHP; }
  int  getHP(void)        { return hp;  }
  void setXP(int newXP)   { xp = newXP; }
  int  getXP(void)        { return xp;  }
  void setAC(int newAC)   { ac = newAC; }
  int  getAC(void)        { return ac;  }
  void setDMG(int newDMG) { dmg = newDMG; }
  int  getDMG(void)       { return dmg;   }
  void setkills(int newkills) 
                         { kills = newkills; }
  int  getkills(void)    { return kills; }
  char* getarmor(void)   { char *tmp = new char[32]; strcpy(tmp, armor); return tmp;  }
  char* getweapon(void)  { char *tmp = new char[32]; strcpy(tmp, weapon); return tmp; }  
  char* getname(void)    { char *tmp = new char[32]; strcpy(tmp, name); return tmp;   }  
  void setname(char *newname)     { if (newname != NULL)   strcpy(name, newname);     }
  void setarmor(char *newarmor)   { if (newarmor != NULL)  strcpy(armor, newarmor);   }
  void setweapon(char *newweapon) { if (newweapon != NULL) strcpy(weapon, newweapon); }
  };

class Monster {
public:
  int x, y;
  int hp, ac, dmg;
  char name[32];
  int tile;
  bool live;
  };

void randomize(void);
void move_gremlin(int i, int x, int y);
void move_monsters(void);
void init_monsters(void);
void show_monsters(void);
void init_screen(void);
void restore_screen(void);
void play_game(void);
void init_map(char *map_path);
void init_player(void);
void draw_map(void);
void moveto(int x, int y);
void resolve_move(int dx, int dy);
void print_status(char *msg);
void resolve_attack(int x, int y);
void gremlin_attack(int i);
void win(void);

int is_attack(int x, int y);
bool is_validmove(int x, int y);
bool get_command(void);
bool is_validmonstermove(int x, int y);
bool is_monsterattack(int x, int y);

