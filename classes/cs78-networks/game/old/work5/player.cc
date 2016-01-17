#define PLAYER
#include "game.h"

void Player::attack_monster(int i) {
  char msg[128];
  if (attack_succeeds(monsters[i].getAC())) {
    monsters[i].setHP(monsters[i].getHP() - damage(player->getDMG()));
    sprintf(msg,"You hit %s %d",monsters[i].getname(),i);
    print_status(msg);
    if (monsters[i].getHP() < 0) {
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
  for (i=0;i<n_players;i++) {
    y = players[i].getY(); x = players[i].getX();
    if (y != players[i].oldY() || x != players[i].oldX()) {
      mvaddch(players[i].oldY(), players[i].oldX(), 
              map[players[i].oldY()][players[i].oldX()]);
      if (players[i].getlive()) mvaddch(y,x,'@'); 
      players[i].set_oldY(y); players[i].set_oldX(x);
      }
    }
  }
