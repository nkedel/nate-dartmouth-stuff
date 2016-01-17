#define DATABASE
#include "game.h"

static char db[3][16][12][64];
bool put(int dbn, int rec, int field, char *data) {
//  printf("%d:%d:%d:%s\n",dbn,rec,field,data);
  if (data == NULL) return FALSE;
  strcpy(db[dbn][rec][field],data);
  return TRUE;
  }

char *get(int dbn, int rec, int field) {
  char *data;
  data = new char[80];
  strcpy(data,db[dbn][rec][field]);
//  printf("%d:%d:%d:%s\n",dbn,rec,field,data);
  return data;
  }

void init_db(void) {
  init_player(3, "Principal Skinner", mapx - 2, mapy-2);
  init_player(2, "Chief Wiggum", 1, mapy-2);
  init_player(1, "Sideshow Bob", mapx - 2, 1);
  init_player(0, "Ned Flanders", 1, 1);
  load_map("map.3");
  printf("Done initializing players.\n");
  }
  
void init_player(int num, const char *name, int x, int y) {
  printf("Initializing player #%d\n",num);
  player = &players[num];
  player->settoken();
  player->whoami(num);
  player->setX(x);
  player->setY(y);
  player->set_oldX(255);
  player->set_oldY(255);
  player->setkills(0);
  player->setHP(100);
  player->setDMG(20);
  player->setAC(10);
  player->setXP(0);
  player->setname(name);
  player->setweapon("Fist");
  player->setarmor("Loin Cloth");
  player->setlive(TRUE);
  player->cleartoken();
  }

void load_map(char *mappath) { 
  printf("Loading map\n");
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
                          
