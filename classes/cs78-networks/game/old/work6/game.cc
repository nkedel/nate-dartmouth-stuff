
#define MAIN
#include "game.h"

int main(int argc, char **argv) {
  randomize();
  init_db();
  game();
  }
  
void game(void) {
  init_map();
  init_curses();
  redraw_screen();
  play_game();
  restore_screen();
  endwin();
  }
  
void randomize(void) {
  time_t t;
  time(&t);
  initstate((unsigned) t,randomstate,64);
  }

void print_status(char *msg) {
  int i;
  if (msg==NULL) return;
  mvaddch(22,0,' ');
  clrtoeol();
  i = (64 - strlen(msg)) / 2;
  mvprintw(22,i,"%s",msg);
  move(player->getY(),player->getX());
  refresh();
  napms(333);
  }

void init_map(void) { 
  char *mapline;
  int i, j; 
  
  for (i=0; i<mapy; i++) { 
    mapline = get(1, i, 1);
    for (j=0; j<mapx; j++) map[j][i]=mapline[j];
    delete [] mapline;
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
    update_players();
    update_monsters();
    move(player->getY(), player->getX());
    refresh();
    }
  }

bool attack_succeeds(int ac) {
  if ((random() % 20) < ac) return TRUE;
  return FALSE;
  }

int damage(int dmg) {
  return (random() % dmg) + 1;
  }


int is_monster(int x, int y) {
  int i;
  for (i=0;i< n_monsters;i++) {
    if (monsters[i].getlive() && x == monsters[i].getX() && y == monsters[i].getY()) return i;
    }
  return -1;
  }
  
int is_player(int x, int y) {
  int i;
  for (i=0;i< n_players;i++) {
    if (players[i].getlive() && x == players[i].getX() && y == players[i].getY()) return true;
    }
  return -1;
  }
  
bool is_validmove(int x, int y) {
  if (map[x][y] == '.') return TRUE;
  return FALSE;
  }

  
void restore_screen(void) {
  clear();
  nocbreak();
  refresh();
  }  
  
void init_curses(void) {
  initscr(); 
  cbreak(); 
  noecho();
  nonl();
  intrflush(stdscr, FALSE);
  keypad(stdscr, TRUE);
  clear();
  refresh();
  }

void redraw_screen(void) {
  char *temp;
  clear();
  draw_map();
  mvaddch(player->getY(), player->getX(), '@'); 
  show_monsters();
  show_players();
  mvprintw(1,66,"ThunderDome");
  mvprintw(2,66,"(Prototype)");
  mvprintw(4,66,"Player:");
  temp = player->getname();
  mvprintw(5,68,"%s",temp);
  mvprintw(6,66,"HP: %d   ",player->getHP());
  mvprintw(7,66,"XP: %d   ",player->getXP());
  mvprintw(8,66,"Kills: %d   ",player->getkills());
  mvprintw(10,66,"Weapon: ");
  temp=player->getweapon();
  mvprintw(11,68,"%s",temp);
  mvprintw(12,66,"Armor: ");
  temp=player->getarmor();
  mvprintw(13,68,"%s",temp);
  temp=player->getname();
  mvprintw(22,(36-strlen(temp))/2,"Welcome to the Thunderdome, %s",temp);  
  mvprintw(23,15,"...Two Men Enter, One Man Leaves...");
  refresh();
  }
  
bool get_command(void) {  
    if (player->getkills() == n_monsters) {
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
      case '~':
          if (me == 0) me = 1;
          else me = 0;
          player = &players[me];
          redraw_screen();
          break;
      case KEY_UP:
      case '8':
          player->resolve_move(0,-1); 
          break;
      case KEY_DOWN:
      case '2': 
          player->resolve_move(0,1);
          break;
      case KEY_LEFT:
      case '4': 
          player->resolve_move(-1,0);
          break;
      case KEY_RIGHT:
      case '6': 
          player->resolve_move(1,0);
          break;
      case KEY_A1:
      case '7': 
          player->resolve_move(-1,-1);
          break;
      case KEY_A3:
      case '9': 
          player->resolve_move(1,-1);
          break;
      case KEY_C1:
      case '1': 
          player->resolve_move(-1,1);
          break;
      case KEY_C3: 
      case '3':
          player->resolve_move(1,1);
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

