#ifndef DUNGEON_H
#define DUNGEON_H
#include "global.h"
#include "level.h"
#include "monst.h"
#include "hero.h"

class dungeon{
protected:
    schar astral;
    schar water;
    schar air;
    schar fire;
    schar earth;
    uchar mines;
    uchar oracle;
    uchar fort;
    uchar quest;
    uchar medusa;
    uchar stronghold;

    level* current;
    hero* u;
public:
    dungeon();
    void setcurrentlev(level *curr);
    level *currentlev(){return current;}
    hero *gethero(){return u;}
    void sethero(hero *you){u=you;}
//    void findhero(uchar x,uchar y);
//    void movemonsters();
    void heromove(char dir){u->move(dir);}
};

#endif
