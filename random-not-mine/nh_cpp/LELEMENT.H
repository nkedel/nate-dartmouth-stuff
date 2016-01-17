#ifndef LELEMENT_H
#define LELEMENT_H

#include "global.h"
#include "objlist.h"
#include "monst.h"

// Engrave types

const uchar DUST=   0;
const uchar ENGRAVE=1;
const uchar BURN=   2;
const uchar MARK=   3;


//  when moving around in a level, you should always call canenter() on
// prospective element to enter, and canleave() on the element currently
// occupied. When the move can take place, call enter() on the new location,
// and leave() on the old.

class level;

class levelelement{
protected:
    monstclass *mon;        // monster occupying space, if any.
    objectlist olist;
    uchar x,y;              // level elements should know where they are
                            // in a level.

    level& lev;             // level containing element.

    levelelement *nextup;   // level elements know their neighbors.
    levelelement *nextdown; 
    levelelement *nextleft;
    levelelement *nextright;

    Bitfields(engravingtype,2);    // type of engraving made.
    Bitfields(secret,1);           // does this element contain a secret part?
    Bitfields(seen,1);             // has element been seen?
    Bitfields(lit,1);
    Bitfields(inshop,1);           // is element in shop?
    Bitfields(monmoved,1);         // set when monster enters so it is not moved
                                     // again in the same turn. turn off all after
                                     // each turn.
    Bitfields(digable,1);          // can this element be dug in?
    // One byte
    char *engraving;

public:
    levelelement(level &l,uchar lx,uchar ly): lev(l),x(lx),y(ly),
        mon(NULL),nextup(NULL),nextdown(NULL),nextleft(NULL),
        nextright(NULL),engraving(NULL),engravingtype(0),secret(0),seen(0),
        lit(0),inshop(0),monmoved(0),digable(1){}
    ~levelelement(){olist.free();delete mon;delete engraving;}
    virtual void draw();
    virtual uchar canenter(monstclass *m);  // see if hero or monster can walk here.
    virtual void enter(monstclass *m);      // enter. (activate traps)
    virtual uchar canleave();               // see if monster can exit 
    virtual void leave();                   // leave. cleans up mostly.
    virtual void offer(){message("Nothing happens.");}
    virtual void kick(monstclass& m){message("ouch!");}
    virtual void dig(monstclass& m){message("not yet");}
    virtual void quaff(monstclass& m){message("Nothing to drink");}
    virtual void open(monstclass& m){message("This doesn't seem to work");}
    void loot(monstclass& m){message("nothing here to loot!");}
    void engrave(objectclass& o){message("You make pretty pictures");}
    virtual uchar isdrinkable(){return FALSE;}
    virtual uchar ismapable(){return TRUE;}
    virtual uchar isstone(){return FALSE;}
    uchar isdigable(){return digable;}
    void setdigable(uchar c=1){digable=c;}
    uchar issecret(){return secret;}
    void setsecret(){secret=0;}
    uchar isseen(){seen=1;}
    void setseen(uchar c=1){seen=c;}
    uchar isshop(){return inshop;}
    void setshop(uchar c=1){inshop=1;}
    uchar islit(){return lit;}
    void setlit(uchar c=1){lit=c;}
    uchar moved(){return monmoved;}
    void setmoved(uchar c=1){monmoved=c;}
    void setnextup(levelelement *l){nextup=l;}
    void setnextdown(levelelement *l){nextdown=l;}
    void setnextleft(levelelement *l){nextleft=l;}
    void setnextright(levelelement *l){nextright=l;}
    monstclass *removemon(){monstclass *tmp;tmp=mon;mon=NULL;return tmp;}
    monstclass *getmon(){return mon;}
    void putmon(monstclass *m){mon=m;mon->setpos(x,y);mon->setlev(&lev);}
    objectlist& getolist(){return olist;}
    void putobj(objectclass *otmp){olist.add(otmp);}
};


class stone: public levelelement{
public:
    stone(level& lev,uchar lx,uchar ly): levelelement(lev,lx,ly){}
    void draw();
    uchar isstone(){return TRUE;}
};

class tlcorner:public levelelement{
//
//   +-
//   |
//
public:
    tlcorner(level& lev,uchar lx,uchar ly): levelelement(lev,lx,ly){}
    void draw();
    uchar isstone(){return TRUE;}
};

class tdwall:public levelelement{
//
//  -+-
//   |
//
public:
    tdwall(level& lev,uchar lx,uchar ly): levelelement(lev,lx,ly){}
    void draw();
    uchar isstone(){return TRUE;}
};

class trcorner:public levelelement{
//
//  -+
//   |
//
public:
    trcorner(level& lev,uchar lx,uchar ly): levelelement(lev,lx,ly){}
    void draw();
    uchar isstone(){return TRUE;}
};

class trwall:public levelelement{
//
//   |
//   +-
//   |
//
public:
    trwall(level& lev,uchar lx,uchar ly): levelelement(lev,lx,ly){}
    void draw();
    uchar isstone(){return TRUE;}
};

class crosswall:public levelelement{
//
//   |
//  -+-
//   |
//
public:
    crosswall(level& lev,uchar lx,uchar ly): levelelement(lev,lx,ly){}
    void draw();
    uchar isstone(){return TRUE;}
};

class tlwall:public levelelement{
//
//   |
//  -+
//   |
//
public:
    tlwall(level& lev,uchar lx,uchar ly): levelelement(lev,lx,ly){}
    void draw();
    uchar isstone(){return TRUE;}
};

class hwall:public levelelement{
//
//
//  ---
//
//
public:
    hwall(level& lev,uchar lx,uchar ly): levelelement(lev,lx,ly){}
    void draw();
    uchar isstone(){return TRUE;}
};

class blcorner: public levelelement{
//
//  |
//  +-
//
//
public:
    blcorner(level& lev,uchar lx,uchar ly): levelelement(lev,lx,ly){}
    void draw();
    uchar isstone(){return TRUE;}
};

class tuwall: public levelelement{
//
//   |
//  -+-
//
//
public:
    tuwall(level& lev,uchar lx,uchar ly): levelelement(lev,lx,ly){}
    void draw();
    uchar isstone(){return TRUE;}
};

class brcorner: public levelelement{
//
//   |
//  -+
//
//
public:
    brcorner(level& lev,uchar lx,uchar ly): levelelement(lev,lx,ly){}
    void draw();
    uchar isstone(){return TRUE;}
};

class vwall:public levelelement{
//
//   |
//   |
//   |
//
public:
    vwall(level& lev,uchar lx,uchar ly): levelelement(lev,lx,ly){}
    void draw();
    uchar isstone(){return TRUE;}
};

class pool: public levelelement{
public:
    pool(level& lev,uchar lx,uchar ly): levelelement(lev,lx,ly){digable=FALSE;}
    void draw();
    uchar isdrinkable(){return TRUE;}
    void quaff();
};

class moat: public levelelement{
public:
    moat(level& lev,uchar lx,uchar ly): levelelement(lev,lx,ly){digable=FALSE;}
    void draw();
    uchar isdrinkable(){return TRUE;}
    void quaff();
};

class drawbridge_up: public levelelement{
public:
    drawbridge_up(level& lev,uchar lx,uchar ly): levelelement(lev,lx,ly){digable=FALSE;}
    void draw();
};

class lavapool: public levelelement{
public:
    lavapool(level& lev,uchar lx,uchar ly): levelelement(lev,lx,ly){digable=FALSE;}
    void draw();
    uchar isdrinkable(){return TRUE;}
    void quaff();
};

// Door states.

const uchar OPEN=0;
const uchar CLOSED=1;
const uchar LOCKED=2;
const uchar BROKEN=3;

class door: public levelelement{
    Bitfields(doorstate,2);
public:
    door(level& lev,uchar lx,uchar ly,uchar state): levelelement(lev,lx,ly),doorstate(state){}
    void open(){message("You open the door.");}
    void draw();
    void kick(){message("Crash!");}
};

class coor: public levelelement{
public:
    coor(level& lev,uchar lx,uchar ly): levelelement(lev,lx,ly){}
    void draw();
};

class room: public levelelement{
public:
    room(level& lev,uchar lx,uchar ly): levelelement(lev,lx,ly){}
    void draw();
    uchar ismapable(){return FALSE;}
};

class stairsdn: public levelelement{
public:
    stairsdn(level& lev,uchar lx,uchar ly): levelelement(lev,lx,ly){digable=FALSE;}
    void draw();
};

class stairsup: public levelelement{
public:
    stairsup(level& lev,uchar lx,uchar ly): levelelement(lev,lx,ly){digable=FALSE;}
    void draw();
};

class ladder: public levelelement{
public:
    ladder(level& lev,uchar lx,uchar ly): levelelement(lev,lx,ly){digable=FALSE;}
    void draw();
};

class fountain: public levelelement{
    Bitfields(abuse1,1);
    Bitfields(abuse2,1);
    Bitfields(abuse3,1);
public:
    fountain(level& lev,uchar lx,uchar ly): levelelement(lev,lx,ly){digable=FALSE;abuse1=abuse2=abuse3=0;}
    void draw();
    uchar isdrinkable(){return TRUE;}
    void quaff();
};

class throne: public levelelement{
    Bitfields(abuse1,1);
    Bitfields(abuse2,1);
public:
    throne(level& lev,uchar lx,uchar ly): levelelement(lev,lx,ly){abuse1=abuse2=0;}
    void draw();
    void kick(){message("You kick the throne!");}
};

class sink: public levelelement{
    Bitfields(abuse1,1);
    Bitfields(abuse2,1);
    Bitfields(abuse3,1);
public:
    sink(level& lev,uchar lx,uchar ly): levelelement(lev,lx,ly){abuse1=abuse2=abuse3=0;}
    void draw();
    void kick(){message("You kick the sink!");}
    uchar isdrinkable(){return TRUE;}
    void quaff();
};

const uchar RANDOM=0;
const uchar LAWFUL=1;
const uchar NEUTRAL=2;
const uchar CHAOTIC=3;

class altar : public levelelement{
    Bitfields(alignment,2);
public:
    altar(level& lev,uchar lx,uchar ly,uchar al): levelelement(lev,lx,ly){if(al)alignment=al; else alignment=rnd1(3);}
    void draw();
    void kick(){message("You kick the Altar! Oops!");}
};

class icepool : public levelelement{
public:
    icepool(level& lev,uchar lx,uchar ly): levelelement(lev,lx,ly){}
    void draw();
};

class icemoat: public levelelement{
public:
    icemoat(level& lev,uchar lx,uchar ly): levelelement(lev,lx,ly){}
    void draw();
};

class drawbridge_down: public levelelement{
public:
    drawbridge_down(level& lev,uchar lx,uchar ly): levelelement(lev,lx,ly){}
    void draw();
};

class air: public levelelement{
public:
    air(level& lev,uchar lx,uchar ly): levelelement(lev,lx,ly){digable=FALSE;}
    void draw();
};

class cloud: public levelelement{
public:
    cloud(level& lev,uchar lx,uchar ly): levelelement(lev,lx,ly){digable=FALSE;}
    void draw();
};
#endif
