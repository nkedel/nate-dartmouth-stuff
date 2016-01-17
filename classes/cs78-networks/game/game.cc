// game.cc: main program
// I wrote this, although David Marmaros may have made trivial modifications
// during the test process.

#define MAIN
#include "game.h"

int main(int argc, char **argv) {
  int who;
  int i;
  randomize();

// blank out players structures
  for (i=0;i<n_players;i++) players[i].whoami(i);

// verify command line arguments, then connect to net
  if (argc < 4) exit(128);
  who=atoi(argv[3]);
  if (who < 0 || who > 3) exit(128);
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

// initialize, then play game
  init_curses();
  init_map();
  redraw_screen();
  play_game();
  restore_screen();
  endwin();
  }
  
// set the random number generator seed based on the system timer 
void randomize(void) {
  time_t t;
  time(&t);
  initstate((unsigned) t,randomstate,64);
  }

// print a status message to the screen
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

// print a status message to the local screen only - never used
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

// initialize the map:
//   if we are not master player (player 1-3) 
//      check server for map... if not present, sleep 1/2 sec, then recheck
//   download server from map (this duplicated the uploaded copy if we are 
//      master player)
//
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
  
  // sleep 1 sec to let network stabilize
  napms(1000);
  printf("Loaded.               \n");
  }
                
// draw map from map array to local screen
void draw_map(void) {
  int i, j;
  char line[80] = "";
  for (i=0; i<mapy; i++) {     // draw each line of map 
    for (j=0; j<mapx; j++) {   // copy line to local string
      line[j] = map[j][i];
      }
    line[mapx]=0;         // tack a 0 on to end the string
    mvprintw(i,0,"%s",line);
    }
  }


// main playloop

void play_game(void) {
  while (TRUE) {
    net.tokendone();           // release token
    while (!net.IYTwait()) {   // check for turn/token
      napms(75);               // if no token, wait 75ms
      }    
    update_players();          // get player locations from server
                               // move cursor over our player's character 
    move(players[me].getY(), players[me].getX());
    refresh();                 // commit curses changes
    turns++;              
    if (get_command() == FALSE) break; // get_command returns false on quit
    }
  }

// checks for attack success: 
// if attack roll is less than target's armor class, attack hits
bool attack_succeeds(int ac) {
  if ((random() % 20) < ac) return TRUE;
  return FALSE;
  }

// checks for damage done: currently just a random number; included for
// future functions like weapon damage mods
int damage(int dmg) {
  return (random() % dmg) + 1;
  }

// checks if location (x,y) contains a player 
int is_player(int x, int y) {
  int i;
  for (i=0;i< n_players;i++) {
    if (players[i].getlive() && x == players[i].getX() && y == players[i].getY()) return i;
    }
  return -1;
  }
  
// checks if location (x,y) is an empty space on map.
bool is_validmove(int x, int y) {
  if (map[x][y] == '.') return TRUE;
  return FALSE;
  }

// restore screen to non-game state.  
void restore_screen(void) {
  clear();
  nocbreak();
  refresh();
  }  
  
// initializes curses/screen to play-time state
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

// redraws screen (map, players, etc.) for start of game or refresh on
// corruption
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
  
// process player input; called by the main loop.
bool get_command(void) {  
    players[me].settoken();
    int x;
    x = getch();
    flushinp();
    switch (x) {
      case 'q':
      case 'Q': 
          players[me].cleartoken();
          net.tokendone(); // returns token; won't be returned by main loop
                           // since player is quitting
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
      
      case '~':        // redraw screen if corrupted.
          if (++me > 3) me = 0;
              player = &players[me];
              redraw_screen();
              break;                                          

      default: break;
      }
  players[me].cleartoken();
//  net.tokendone(); // this has been moved to the main loop b/c of timing
                     // issues.
  return TRUE;
  }

// inform player of win, pause to allow them to read the win message
void win(void) {
  print_status("You win!");
  beep();
  beep();
  beep();
  napms(1333);
  }

