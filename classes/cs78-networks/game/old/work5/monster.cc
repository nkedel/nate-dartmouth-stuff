#define MONSTER
#include "game.h"

void show_monsters(void) {
  int i, x, y;
  for (i=0;i<n_monsters;i++)
    if (monsters[i].getlive()) {
      mvaddch(y=monsters[i].getY(),x=monsters[i].getX(),monsters[i].gettile());
      monsters[i].set_oldX(x); monsters[i].set_oldX(y);
      }
  }

void update_monsters(void) {
  int i, x, y;
  for (i=0;i<n_monsters;i++) {
    y = monsters[i].getY(); x = monsters[i].getX();
    if (y != monsters[i].oldY() || x != monsters[i].oldX()) {
      mvaddch(monsters[i].oldY(), monsters[i].oldX(), 
              map[monsters[i].oldX()][monsters[i].oldY()]);
      if (monsters[i].getlive()) mvaddch(y,x,monsters[i].gettile()); 
      monsters[i].set_oldY(y); monsters[i].set_oldX(x);
      }
    }
  }

void move_monsters(void) {
  int i;
  if (n_monsters == player->getkills()) return;
  for (i=0; i<n_monsters; i++) 
    if (monsters[i].getlive()) monsters[i].do_move();
  }

void 
Monster::moveto(int dx, int dy) {
  setY(dy); setX(dx);
  }

// Kluge for now: return a random player... we need to implement this for real.
int get_nearestplayer(int x, int y) {
  return (random() % 8);  
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
