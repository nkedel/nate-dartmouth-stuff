#define PLAYER
#include "game.h"

// resolve attacks between players.
void Player::attack_player(int i) {
  char msg[128];
  sprintf(msg,"Attacking %s with AC %d", players[i].getname(), players[i].getAC());
  print_status(msg);
  if (attack_succeeds(players[i].getAC())) {
    players[i].setHP(players[i].getHP() - damage(players[me].getDMG()));
    sprintf(msg,"You hit %s",players[i].getname());
    print_status(msg);
    if (players[i].getHP() < 0) {
      players[i].setlive(false);
      players[i].setX(255);
      players[i].setY(255);
      players[me].setXP(players[me].getXP() + 10);
      players[me].setkills(players[me].getkills() + 1);
      mvprintw(7,66,"XP: %d   ",players[me].getXP());
      mvprintw(8,66,"Kills: %d   ",players[me].getkills());
      print_status("And he dies...");
      // broadcast "You're dead" message -- not yet implemented
      } 
    return;
    }
  sprintf(msg,"You miss %s",players[i].getname());
  print_status(msg);
  }

void Player::resolve_move(int dx, int dy) {
  int newx = getX() + dx,
      newy = getY() + dy,
      i;

 if ((i=is_player(newx,newy)) != -1) {
    attack_player(i); 
    return;
    }

  if (is_validmove(newx, newy)) {
    do_move(newx, newy);
    return;
    }
  print_status("Duh -- where you trying to move to?");
  }

void Player::do_move(int x, int y) {
  mvaddch(players[me].getY(), players[me].getX(), map[players[me].getX()][players[me].getY()]);
  mvaddch(y,x,'@');
  players[me].setY(y);
  players[me].setX(x); 
  move(players[me].getY(),players[me].getX());
  refresh();
  }

void show_players(void) {
  int i, x, y;
  for (i=0;i<n_players;i++)
    if (players[i].getlive()) {
      mvaddch(y=players[i].getY(),x=players[i].getX(),'@');
      players[i].set_oldY(y); players[i].set_oldX(x);
      }
  }

void update_players(void) {
  int i, x, y;
  char *temp;
    
  for (i=0;i<n_players;i++) {
    y = players[i].getY(); x = players[i].getX();
    if (y != players[i].oldY() || x != players[i].oldX()) {
      mvaddch(players[i].oldY(), players[i].oldX(), 
              map[players[i].oldX()][players[i].oldY()]);
      if (players[i].getlive()) mvaddch(y,x,'@'); 
      players[i].set_oldY(y); players[i].set_oldX(x);
      }
    }
  player = &players[me];
  mvprintw(4,66,"Player:");
  temp = players[me].getname();
  mvprintw(5,68,"%s",temp);
  mvprintw(6,66,"HP: %d   ",players[me].getHP());
  mvprintw(7,66,"XP: %d   ",players[me].getXP());
  mvprintw(8,66,"Kills: %d   ",players[me].getkills());
  mvprintw(10,66,"Weapon: ");
  temp=players[me].getweapon();
  mvprintw(11,68,"%s",temp);
  mvprintw(12,68,"(DMG: %d)",players[me].getDMG());
  mvprintw(13,66,"Armor: ");
  temp=players[me].getarmor();
  mvprintw(14,68,"%s",temp);
  mvprintw(15,68,"(AC: %d)",players[me].getAC());
  mvprintw(16,66,"Time %d", turns);
  mvprintw(18,66,"Me=%d",me);
  }
