#define DATABASE
#include "game.h"

static char db[3][16][12][64];
bool put(int dbn, int rec, int field, char *data) {
  if (data == NULL) return FALSE;
  strcpy(db[dbn][rec][field],data);
  return TRUE;
  }
char *get(int dbn, int rec, int field) {
  char *data = new char[80];
  strcpy(data,db[dbn][rec][field]);
  return data;
  }

void init_db(void) {
  load_map("map.3");
  init_player(3, "Principal Skinner", mapx - 2, mapy-2);
  init_player(2, "Chief Wiggum", 1, mapy-2);
  init_player(1, "Sideshow Bob", mapx - 2, 1);
  init_player(0, "Ned Flanders", 1, 1);
  init_monsters();
  }
  
void init_monsters(void) {
  int i, j, x, y;
  bool isgoodloc;
  for (i=0;i<n_monsters;i++) {
    isgoodloc = FALSE;
    while (!isgoodloc) {
      isgoodloc = TRUE;
      x = (random() % mapx);
      y = (random() % mapy);
      if (map[x][y] != '.') isgoodloc = FALSE;
      else {
        for (j=0; j<=n_monsters;j++) 
          if (i != j && monsters[j].getlive() && x == monsters[j].getX() && y == monsters[j].getY()) 
            isgoodloc = FALSE; 
        for (j=0; j<=n_players;j++) 
          if (i != j && players[j].getlive() && x == players[j].getX() && y == players[j].getY()) 
            isgoodloc = FALSE; 
        }
      }
    monsters[i].setX(x);
    monsters[i].setY(y);

    monsters[i].setHP(15);
    monsters[i].setAC(10);
    monsters[i].setDMG(20);
    monsters[i].setlive(TRUE);

    if (i < n_mon_names) {
      monsters[i].settile(mon_tiles[i]);
      monsters[i].setname(mon_names[i]);  
      }
    else  {
      monsters[i].settile('g');
      monsters[i].setname("Gremlin");  
      }
    }
  }
  
void init_player(int num, const char *name, int x, int y) {
  player = &players[num];
  player->setX(x);
  player->setY(y);
  player->setkills(0);
  player->setHP(100);
  player->setDMG(20);
  player->setAC(10);
  player->setXP(0);
  player->setname(name);
  player->setweapon("Fist");
  player->setarmor("Loin Cloth");
  player->setlive(TRUE);
  }

void load_map(char *mappath) { 
  FILE *mapinput;
  char mapline[80];
  int i, j; 
  
  mapinput=fopen(mappath,"r");
  for (i=0; i<mapy; i++) { 
    fgets(mapline, 79, mapinput);
    for (j=0; j<mapx; j++) strcpy(db[1][i][1],mapline);
    }
  fclose(mapinput); 
  }
                          
