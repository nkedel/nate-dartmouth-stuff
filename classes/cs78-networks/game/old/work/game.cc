#include "game.h"

const int mapx=64, mapy=22;
char map[64][22];
Monster gremlin[16];
Player player;
int  n_gremlins = 4;
char randomstate[64];

int main(int argc, char **argv) {
  init_map(argv[1]);
	init_player();
  init_monsters();
	init_screen();
	play_game();
	restore_screen();
	endwin();
  exit(0);
	}
	
void randomize(void) {
  time_t t;
  time(&t);
  initstate((unsigned) t,randomstate,64);
  }

void init_monsters(void) {
  int i;
  bool isgoodloc;
  for (i=0;i<n_gremlins;i++) {
    isgoodloc = FALSE;
    while (!isgoodloc) {
      isgoodloc = TRUE;
      gremlin[i].x = random() % mapx;
      gremlin[i].y = random() % mapy;
      if (map[gremlin[i].x][gremlin[i].y] != '.') isgoodloc = FALSE;
      if (gremlin[i].x == player.getX() && gremlin[i].y == player.getY()) 
        isgoodloc = FALSE; 
      }
    gremlin[i].hp   = 15;
    gremlin[i].ac   = 10;
    gremlin[i].dmg  = 20;
    gremlin[i].live = TRUE;
    gremlin[i].tile = 'g';
    strcpy(gremlin[i].name,"Gremlin");
    }
  }
  
void print_status(char *msg) {
  int i;
  if (msg==NULL) return;
  mvaddch(22,0,' ');
  clrtoeol();
  i = (64 - strlen(msg)) / 2;
  mvprintw(22,i,"%s",msg);
  move(player.getY(),player.getX());
  refresh();
  napms(333);
  }

void init_player(void) {
	player.setX(1);
	player.setY(1);
  player.setkills(0);
	player.setHP(100);
  player.setDMG(20);
  player.setAC(10);
	player.setXP(0);
  player.setname("Ned Flanders");
  player.setweapon("Fist");
  player.setarmor("Loin Cloth");
  }

void init_map(char *mappath) { 
  FILE *mapinput;
  char mapline[80];
  int i, j; 
  
  mapinput=fopen(mappath,"r");
  for (i=0; i<mapy; i++) { 
    fgets(mapline, 79, mapinput);
    for (j=0; j<mapx; j++) map[j][i]=mapline[j];
    }
  fclose(mapinput); 
  }
                          
void init_map(void) {
  int i, j;
  for (i=0; i<mapy; i++) 
    for (j=0; j<mapx; j++) {
      if (i == 0 || j == 0 || i == mapy - 1 || j == mapx -1 ) map[j][i]='#';
        else map[j][i]='.';
      }
  }

void draw_map(void) {
  int i, j;
  char line[80] = "";
  for (i=0; i<mapy; i++) {
    for (j=0; j<mapx; j++) {
      line[j] = map[j][i];
      }
    line[mapx]=0;
    mvprintw(i,0,"%s",line);
    }
  }

void play_game(void) {
	while (get_command()) {
    move_monsters();
	  }
	}

bool attack_succeeds(int ac) {
  if ((random() % 20) < ac) return TRUE;
  return FALSE;
  }

int damage(int dmg) {
  return (random() % dmg) + 1;
  }

void resolve_attack(int i, int x, int y) {
  char msg[64];
  if (attack_succeeds(gremlin[i].ac)) {
    gremlin[i].hp -= damage(player.getDMG());
    sprintf(msg,"You hit Gremlin %d",i);
    print_status(msg);
    if (gremlin[i].hp < 0) {
      mvaddch(gremlin[i].y,gremlin[i].x,map[gremlin[i].x][gremlin[i].y]);
      gremlin[i].live=false;
      gremlin[i].x = 255;
      gremlin[i].y = 255;
      player.setXP(player.getXP() + 10);
      player.setkills(player.getkills() + 1);
      mvprintw(7,66,"XP: %d   ",player.getXP());
      mvprintw(8,66,"Kills: %d   ",player.getkills());
      print_status("And it dies...");
      } 
    return;
    }
  sprintf(msg,"You miss Gremlin %d",i);
  print_status(msg);
  }

int is_attack(int x, int y) {
  int i;
  for (i=0;i< n_gremlins;i++) {
    if (gremlin[i].live && x == gremlin[i].x && y == gremlin[i].y) return i;
    }
  return -1;
  }
  
bool is_validmove(int x, int y) {
  if (map[x][y] == '.') return TRUE;
  if (map[x][y] == '#') {
    print_status("OUCH!");
    return FALSE;
    }
  if (map[x][y] == '~') {
    print_status("Sorry, no swimming!");
    return FALSE;
    }
  return FALSE;
  }

inline bool is_validmonstermove(int x, int y) {
  int i;
  if (is_monsterattack(x,y)) return FALSE;
  for (i=0; i< n_gremlins; i++) 
    if (gremlin[i].live && x == gremlin[i].x && y == gremlin[i].y) 
      return FALSE;
  if (map[x][y] == '.') return TRUE;
  return FALSE;
  }

inline bool is_monsterattack(int x, int y) {
  if (x == player.getX() && y == player.getY()) return TRUE;
  return FALSE;
  }

void resolve_move(int dx, int dy) {
  int newx = player.getX() + dx,
      newy = player.getY() + dy,
      i;

 if ((i=is_attack(newx,newy)) != -1) {
    resolve_attack(i, newx, newy); 
    return;
    }
  if (is_validmove(newx, newy)) {
    moveto(newx, newy);
    }
  }

void moveto(int x, int y) {
  mvaddch(player.getY(), player.getX(), map[player.getX()][player.getY()]);
  mvaddch(y,x,'@');
  player.setY(y);
  player.setX(x); 
  move(player.getY(),player.getX());
  refresh();
  }

	
void restore_screen(void) {
  clear();
  nocbreak();
  refresh();
  }	
	
void show_monsters(void) {
  int i;
  for (i=0;i<n_gremlins;i++)
    mvaddch(gremlin[i].y,gremlin[i].x,gremlin[i].tile);
  }

void move_gremlin(int i, int x, int y) {
  int ox = gremlin[i].x, oy = gremlin[i].y;
  mvaddch(oy,ox,map[ox][oy]);
  mvaddch(gremlin[i].y = y, gremlin[i].x = x, gremlin[i].tile);
  move(player.getY(), player.getX());
  }
  
void move_monsters(void) {
  int i, x, y, confused;
  if (n_gremlins == player.getkills()) return;
  for (i=0; i<n_gremlins; i++) {
    if (gremlin[i].live != TRUE) continue;
    x = gremlin[i].x;
    y = gremlin[i].y;
    confused = random() & 1;
    if (confused) {
      x = x + ((random() % 3) - 1);
      y = y + ((random() % 3) - 1);
      if (is_validmonstermove(x,y)) {
        move_gremlin(i,x,y);  
        refresh();
        }      
      }
    else {
      if (player.getX() < x) x--;
      if (player.getX() > x) x++;
      if (player.getY() < y) y--;   
      if (player.getY() > y) y++;
      if (is_validmonstermove(x,y)) {
        move_gremlin(i,x,y);
        refresh();
        }
      else {
        if (is_monsterattack(x,y)) {
          gremlin_attack(i);
          }
        else {
          x = gremlin[i].x;
          y = gremlin[i].y;        
          if (player.getX() < x) x--;
          if (player.getX() > x) x++;
          if (is_validmonstermove(x,y)) {
            move_gremlin(i,x,y);
            refresh();
            }
          else {
            x = gremlin[i].x;
            if (player.getY() < y) y--;   
            if (player.getY() > y) y++;      
            if (is_validmonstermove(x,y)) {
              move_gremlin(i,x,y);
              refresh();
              }
            }
          }
        }
      }
    }
  }
  
void gremlin_attack(int i) {
  char output[64];
  sprintf(output,"Gremlin %d attacks!",i);
  print_status(output);
  }
	
void init_screen(void) {
	char *temp;
	initscr(); 
	cbreak(); 
	noecho();
	nonl();
	intrflush(stdscr, FALSE);
	keypad(stdscr, TRUE);
	clear();
	draw_map();
  mvaddch(player.getY(), player.getX(), '@'); 
  show_monsters();
  mvprintw(1,66,"ThunderDome");
  mvprintw(2,66,"(Prototype)");
  mvprintw(4,66,"Player:");
  temp = player.getname();
  mvprintw(5,68,"%s",temp);
  delete(temp);
  mvprintw(6,66,"HP: %d   ",player.getHP());
  mvprintw(7,66,"XP: %d   ",player.getXP());
  mvprintw(8,66,"Kills: %d   ",player.getkills());
  mvprintw(10,66,"Weapon: ");
  temp=player.getweapon();
  mvprintw(11,68,"%s",temp);
  delete(temp);
  mvprintw(12,66,"Armor: ");
  temp=player.getarmor();
  mvprintw(13,68,"%s",temp);
  delete(temp);
  temp=player.getname();
  mvprintw(22,(36-strlen(temp))/2,"Welcome to the Thunderdome, %s",temp);  
  delete(temp);
	mvprintw(23,15,"...Two Men Enter, One Man Leaves...");
	refresh();
	}

  
bool get_command(void) {	
    if (player.getkills() == n_gremlins) {
      win();
      return FALSE;
      }
    int x;
		x = getch();
		switch (x) {
			case 'q':
			case 'Q': 
					return FALSE;
					break;
			case KEY_UP:
			case '8':
			    resolve_move(0,-1); 
          break;
			case KEY_DOWN:
			case '2': 
          resolve_move(0,1);
					break;
			case KEY_LEFT:
			case '4': 
          resolve_move(-1,0);
					break;
			case KEY_RIGHT:
			case '6': 
          resolve_move(1,0);
          break;
			case KEY_A1:
			case '7': 
			    resolve_move(-1,-1);
					break;
			case KEY_A3:
			case '9': 
          resolve_move(1,-1);
					break;
			case KEY_C1:
			case '1': 
          resolve_move(-1,1);
					break;
			case KEY_C3: 
			case '3':
          resolve_move(1,1);
					break;
			default: break;
			}
  return TRUE;
	}

void win(void) {
  print_status("You win!");
  beep();
  beep();
  beep();
  napms(1333);
  }
