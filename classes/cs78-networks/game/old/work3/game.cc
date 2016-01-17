#define MAIN
#include "game.h"

int main(int argc, char **argv) {
  randomize();
  init_map(argv[1]);
  init_player(3, "Principal Skinner", mapx - 2, mapy-2);
  init_player(2, "Chief Wiggum", 1, mapy-2);
  init_player(1, "Sideshow Bob", mapx - 2, 1);
  init_player(0, "Ned Flanders", 1, 1);
  init_monsters();
  init_curses();
  redraw_screen();
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

void Player::attack_monster(int i) {
  char msg[128];
  if (attack_succeeds(monsters[i].getAC())) {
    monsters[i].setHP(monsters[i].getHP() - damage(player->getDMG()));
    sprintf(msg,"You hit %s %d",monsters[i].getname(),i);
    print_status(msg);
    if (monsters[i].getHP() < 0) {
//      mvaddch(monsters[i].getY(),monsters[i].getX(),map[monsters[i].getX()][monsters[i].getY()]);
      monsters[i].setlive(false);
      monsters[i].setX(255);
      monsters[i].setY(255);
      player->setXP(player->getXP() + 10);
      player->setkills(player->getkills() + 1);
      mvprintw(7,66,"XP: %d   ",player->getXP());
      mvprintw(8,66,"Kills: %d   ",player->getkills());
      print_status("And it dies...");
      // broadcast "monster is dead" message
      } 
    return;
    }
  sprintf(msg,"You miss %s",monsters[i].getname());
  print_status(msg);
  }

void Player::attack_player(int i) {
  char msg[128];
  if (attack_succeeds(players[i].getAC())) {
    players[i].setHP(players[i].getHP() - damage(player->getDMG()));
    sprintf(msg,"You hit %s %d",players[i].getname(),i);
    print_status(msg);
    if (players[i].getHP() < 0) {
//      mvaddch(players[i].getY(),players[i].getX(),map[players[i].getX()][players[i].getY()]);
      players[i].setlive(false);
      players[i].setX(255);
      players[i].setY(255);
      player->setXP(player->getXP() + 10);
      player->setkills(player->getkills() + 1);
      mvprintw(7,66,"XP: %d   ",player->getXP());
      mvprintw(8,66,"Kills: %d   ",player->getkills());
      print_status("And he dies...");
      // broadcast "You're dead" message
      } 
    return;
    }
  sprintf(msg,"You miss %s",players[i].getname());
  print_status(msg);
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

void Player::resolve_move(int dx, int dy) {
  int newx = getX() + dx,
      newy = getY() + dy,
      i;

 if ((i=is_player(newx,newy)) != -1) {
    attack_player(i); 
    return;
    }

 if ((i=is_monster(newx,newy)) != -1) {
    attack_monster(i); 
    return;
    }

  if (is_validmove(newx, newy)) {
    do_move(newx, newy);
    return;
    }
  print_status("Duh -- where you trying to move to?");
  }

void Player::do_move(int x, int y) {
  mvaddch(player->getY(), player->getX(), map[player->getX()][player->getY()]);
  mvaddch(y,x,'@');
  player->setY(y);
  player->setX(x); 
  move(player->getY(),player->getX());
  refresh();
  }

  
void restore_screen(void) {
  clear();
  nocbreak();
  refresh();
  }  
  
void show_monsters(void) {
  int i, x, y;
  for (i=0;i<n_monsters;i++)
    if (monsters[i].getlive()) {
      mvaddch(y=monsters[i].getY(),x=monsters[i].getX(),monsters[i].gettile());
      mon_x[i]=x; mon_y[i]=y;
      }
  }

void show_players(void) {
  int i, x, y;
  for (i=0;i<n_players;i++)
    if (players[i].getlive()) {
      mvaddch(y=players[i].getY(),x=players[i].getX(),'@');
      ply_x[i]=x; ply_y[i]=y;
      }
  }



void update_monsters(void) {
  int i, x, y;
  for (i=0;i<n_monsters;i++) {
    y = monsters[i].getY(); x = monsters[i].getX();
    if (y != mon_y[i] || x != mon_x[i]) {
      mvaddch(mon_y[i], mon_x[i], map[mon_y[i]][mon_x[i]]);
      if (monsters[i].getlive()) mvaddch(y,x,monsters[i].gettile()); 
      mon_y[i] = y; mon_x[i] = x;
      }
    }
  }

void update_players(void) {
  int i, x, y;
  for (i=0;i<n_players;i++) {
    y = players[i].getY(); x = players[i].getX();
    if (y != ply_y[i] || x != ply_x[i]) {
      mvaddch(ply_y[i], ply_x[i], map[ply_y[i]][ply_x[i]]);
      if (players[i].getlive()) mvaddch(y,x,'@'); 
      ply_y[i] = y; ply_x[i] = x;
      }
    }
  }




void 
Monster::moveto(int dx, int dy) {
//  int ox = x, oy = y;
//  mvaddch(oy,ox,map[ox][oy]);
//  mvaddch(y=dy, x=dx, tile);
//  move(player->getY(), player->getX());
  setY(dy); setX(dx);
  }

  
void move_monsters(void) {
  int i;
  if (n_monsters == player->getkills()) return;
  for (i=0; i<n_monsters; i++) 
    if (monsters[i].getlive()) monsters[i].do_move();
  }

// Kluge for now... we need to implement this
int get_nearestplayer(int x, int y) {
  return 0;
  }

void 
Monster::do_move(void)  
{
  int i;
  Player *p;
  int x = getX();
  int y = getY();
  int confused = random() & 1;
  if (confused) {
    x += ((random() % 3) - 1);
    y += ((random() % 3) - 1);
    if ((i = is_player(x,y)) != -1) {
      attack(i);
      return;
      }
    else if (is_monster(x,y) == -1 && is_validmove(x,y)) {
      moveto(x,y);  
      refresh();
      }      
    }
  else {
    i = get_nearestplayer(x,y);
    p = &players[i];
    if (p->getX() < x) x--;
    if (p->getX() > x) x++;
    if (p->getY() < y) y--;   
    if (p->getY() > y) y++;
    if ((i = is_player(x,y)) != -1) {
      attack(i);
      return;
      }
    if (is_validmove(x,y) && is_monster(x,y) == -1) {
      moveto(x,y);
      refresh();
    }
    else {
      if ((i=is_player(x,y)) != -1) attack(i);
      else {
        x = getX();
        y = getY();        
        if (p->getX() < x) x--;
        if (p->getX() > x) x++;
        if ((i = is_player(x,y)) != -1) {
          attack(i);
          return;
          }        
        if (is_validmove(x,y) && is_monster(x,y) == -1) {
          moveto(x,y);
          refresh();
          }
        else {
          x = getX();
          if (p->getY() < y) y--;   
          if (p->getY() > y) y++;      
          if ((i = is_player(x,y)) != -1) {
            attack(i);
            return;
            }
          if (is_validmove(x,y) && is_monster(x,y) == -1) {
            moveto(x,y);
            refresh();
            }
          }
        }
      }
    }
  }


void 
Monster::attack(int i) {
  char msg[128];
  Player *p = &players[i];
  if (attack_succeeds(p->getAC())) {
    p->setHP(p->getHP() - damage(getDMG()));
    sprintf(msg,"%s hits!", name);
    print_status(msg);
    if (p->getHP() < 0) {
      mvaddch(p->getY(),p->getX(),map[p->getX()][p->getY()]);
      p->setlive(false);
      p->setX(255);
      p->setY(255);
      if (i == me) {
        print_status("You died...");
        napms(1333);
        exit(1);
        }
      } 
    return;
    }
  sprintf(msg,"%s misses %s",name, p->getname());
  print_status(msg);
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
