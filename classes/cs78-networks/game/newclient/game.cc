#define MAIN
#include "game.h"

int main(int argc, char **argv) {
  int who;
  int i;
  for (i=0;i<n_players;i++) players[i].whoami(i);
  if (argc < 4) exit(128);
  who=atoi(argv[3]);
  if (who < 0 || who > 3) exit(128);
  randomize();
  me = who;
  printf("Connecting to server '%s' on port %s as user %d\n", argv[1], argv[2], who);
  player = &players[me]; 
  net.Connect(argv[1], atoi(argv[2]));
  if (who==0) {
    net.requesttoken();
    while (!net.IYTwait()) ;
    init_db();
    sleep(1);
    }

  init_curses();
  init_map();
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
  int i,x,y;
  getyx(stdscr,y,x);
  if (msg==NULL) return;
  mvaddch(22,0,' ');
  clrtoeol();
  i = (64 - strlen(msg)) / 2;
  mvprintw(22,i,"%s",msg);
  move(y,x);
  refresh();
  napms(150);
  }

void print_local(char *msg) {
/*  int i, x, y;
  if (msg==NULL) return;
  getyx(stdscr, y, x);
  move(23,0);
  clrtoeol();
  i = (64 - strlen(msg)) / 2;
  mvprintw(23,i,"%s",msg);
  move(y,x);
  refresh();
*/  }

void init_map(void) { 
  printf("Waiting to load map:\n");  
  char *mapline;
  if (me != 0) while(1) {
    printf(".");
    mapline = net.get(1,21,0);
    if (mapline!=NULL && mapline[0] == '#') break;
    napms(500);
    }
  int i, j; 
  for (i=0; i<mapy; i++) { 
    printf("%d: ",i);
    mapline = net.get(1, i, 0);
    printf("%s\n",mapline);
    for (j=0; j<mapx; j++) map[j][i]=mapline[j];
    delete [] mapline;
    }
  napms(1000);
  printf("Loaded.               \n");
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
  while (TRUE) {
    net.tokendone(); 
    while (!net.IYTwait()) {
      napms(75);
      }    
    update_players();
    move(players[me].getY(), players[me].getX());
    refresh();
    turns++;
    if (get_command() == FALSE) break;
    }
  }

bool attack_succeeds(int ac) {
  if ((random() % 20) < ac) return TRUE;
  return FALSE;
  }

int damage(int dmg) {
  return (random() % dmg) + 1;
  }


int is_player(int x, int y) {
  int i;
  for (i=0;i< n_players;i++) {
    if (players[i].getlive() && x == players[i].getX() && y == players[i].getY()) return i;
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
  halfdelay(5); 
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
  mvaddch(players[me].getY(), players[me].getX(), '@'); 
  show_players();
  mvprintw(1,66,"ThunderDome");
  mvprintw(2,66,"(Prototype)");
  mvprintw(4,66,"Player:");
  temp = players[me].getname();
  mvprintw(5,68,"%s",temp);
  mvprintw(6,66,"HP: %d   ",players[me].getHP());
  mvprintw(7,66,"XP: %d   ",players[me].getXP());
  mvprintw(8,66,"Kills: %d   ",players[me].getkills());
  mvprintw(10,66,"Weapon: ");
  temp=players[me].getweapon();
  mvprintw(11,68,"%s",temp);
  mvprintw(13,66,"Armor: ");
  temp=players[me].getarmor();
  mvprintw(14,68,"%s",temp);
  temp=players[me].getname();
  mvprintw(18,66, "Me=%d", me);
  mvprintw(22,(36-strlen(temp))/2,"Welcome to the Thunderdome, %s",temp);  
  mvprintw(23,15,"...Two Men Enter, One Man Leaves...");
  refresh();
  }
  
bool get_command(void) {  
    players[me].settoken();
    int x;
    x = getch();
    flushinp();
    switch (x) {
      case 'q':
      case 'Q': 
          players[me].cleartoken();
          net.tokendone();
          return FALSE;
          break;
      case KEY_UP:
      case '8':
          players[me].resolve_move(0,-1); 
          break;
      case KEY_DOWN:
      case '2': 
          players[me].resolve_move(0,1);
          break;
      case KEY_LEFT:
      case '4': 
          players[me].resolve_move(-1,0);
          break;
      case KEY_RIGHT:
      case '6': 
          players[me].resolve_move(1,0);
          break;
      case KEY_A1:
      case '7': 
          players[me].resolve_move(-1,-1);
          break;
      case KEY_A3:
      case '9': 
          players[me].resolve_move(1,-1);
          break;
      case KEY_C1:
      case '1': 
          players[me].resolve_move(-1,1);
          break;
      case KEY_C3: 
      case '3':
          players[me].resolve_move(1,1);
          break;
      
      case '~':
          if (++me > 3) me = 0;
              player = &players[me];
              redraw_screen();
              break;                                          
      default: break;
      }
  players[me].cleartoken();
//  net.tokendone();
  return TRUE;
  }

void win(void) {
  print_status("You win!");
  beep();
  beep();
  beep();
  napms(1333);
  }

