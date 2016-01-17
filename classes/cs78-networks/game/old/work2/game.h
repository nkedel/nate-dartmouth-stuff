#include <stdio.h>
#include <curses.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

// kluge since curses.h for some reason doesn't define this!
extern "C" int napms(int ms);

class Player {
private:
  int x, y;
  int hp, xp, ac, dmg, kills;
  bool live;
  char * name;
  char * weapon;
  char * armor;
public:
  Player()  {name   = new char[32]; name[0] = 0;
             weapon = new char[32]; weapon[0] = 0;
             armor  = new char[32]; armor[0] = 0;
             live = false;
            }
            
  ~Player() {delete [] name; delete [] weapon; delete [] armor; }

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
  void setlive(bool alive) { live = alive; }
  bool getlive(void)       { return live; }
  int  getkills(void)    { return kills; }
  char* getarmor(void)   {return armor;  }
  char* getweapon(void)  {return weapon; }
  char* getname(void)    {return name;   }
  void setname(const char *newname)     { if (newname != NULL)   strcpy(name, newname);     }
  void setarmor(const char *newarmor)   { if (newarmor != NULL)  strcpy(armor, newarmor);   }
  void setweapon(const char *newweapon) { if (newweapon != NULL) strcpy(weapon, newweapon); }
  void do_move(int x, int y);
  void resolve_move(int dx, int dy);
  void attack_player(int i);
  void attack_monster(int i);
  };

class Monster {
 public:
  Monster()  {name = new char[32]; name[0] = 0; live = false;}
  ~Monster() {delete [] name;}


  void setXY(int newx, int newy) { x = newx; y = newy; }
  int setX(int newx) { x = newx; return x;}
  int setY(int newy) { y = newy; return y;}
  void getXY(int& newx, int& newy) { newx = x; newy = y; }
  int  getX(void) { return x; }
  int  getY(void) { return y; }
  void setHP(int newHP)   { hp = newHP; }
  int  getHP(void)        { return hp;  }
  void setAC(int newAC)   { ac = newAC; }
  int  getAC(void)        { return ac;  }
  void setlive(bool alive) { live = alive; }
  bool getlive(void)       { return live; }
  void setDMG(int newDMG) { dmg = newDMG; }
  int  getDMG(void)       { return dmg;   }

  void settile(int ntile) { tile = ntile; }
  int  gettile(void)       { return tile;   }

  char* getname(void)    {return name;}
  void setname(const char *newname)     { if (newname != NULL)   strcpy(name, newname);     }
  void do_move();
  void moveto(int dx, int dy);
  void attack(int i);

 private:
  int x, y;
  int hp, ac, dmg;
  char * name;
  int tile;
  bool live;
  };

void randomize(void);
void move_monsters(void);
void init_monsters(void);
void show_monsters(void);
void init_screen(void);
void restore_screen(void);
void play_game(void);
void init_map(char *map_path);
void init_player(void);
void draw_map(void);
void print_status(char *msg);
void win(void);

int is_player(int x, int y);
int is_monster(int x, int y);
bool is_validmove(int x, int y);
bool get_command(void);

#ifdef MAIN
const int mapx=64, mapy=22;
static char map[64][22];
Monster monsters[16];
Player *player;
Player players[8];
int me = 0;
int n_monsters = 4;
int n_players=1;
char randomstate[64];
const int n_mon_names = 4;
const char *mon_names[] = { "Evil Homer", "Marge", "Bart", "Lisa" }; 
const char mon_tiles[]  = { 'h', 'm', 'b', 'l' };
#else
extern const int mapx, mapy;
extern char map[64][22];
extern Monster monsters[16];
extern Player players[8];
extern int n_monsters;
extern int n_players;
extern char randomstate[64];
extern const int n_mon_names;
extern const char *mon_names;
extern const char mon_tiles;
#endif
