#ifndef EVENT_H
#define EVENT_H

#include "global.h"

class monstclass;

// These classes are used to enforce anything that is supposed to happen 
// after a certain amount of time. Effects that wear off should be given
// an event to remove the effect. Items need only worry about the event 
// list when their effects are the same as other 'random' effects like
// traps and potions.


// This is a list of all possible event items.
// They are needed to intelegently set and unset attributes without 
// conflicting between the items. ex. Hero gets confused to be unconfused
// 100 turns later. 50 turns later  the hero gets confused again to be
// unconfused 150 turns later. Making the hero confused while confused does 
// nothing, but should not be unconfused a mere 50 turns later. 

// Care must be taken to always remove an unactivated event. Generally it
// won't cause a problem _IF_ the monster the event happens to is still 
// alive.  If the object has been freed, it will likely crash.
// Monster destructors try to remove objects that are associated with them.
// If an item caused the event, the item should remove the event when the 
// item is removed.

// Any 'premanent' event should have a time of MAXTIME so therefore always
// be last and theoritically never activated.

const uint MAXTIME=4294967295; // 2^32

const uchar NOTHING=0;
const uchar UNCONFUSED=1;
const uchar UNHALU=2;
const uchar UNSTUN=3;
const uchar UNBLIND=4;      // There are two ways to become unblinded.
                            // Either some effect wearing off, or a blinding
                            // item is removed. These two events must check 
                            // for each other. If the other exists, nothing
                            // is done, but this is the only way to avoid the
                            // problem of getting blinded by an effect, and 
                            // then wearing and removing the blindfold and
                            // becoming unblinded prematurely.
const uchar UNBLINDITEM=5;
const uchar UNLEVITATE=6;   // Same for levitating.
const uchar UNLEVITATEITEM=7;
const uchar UNWOUND=8;
const uchar TELEPORT=9;     // These events replace themselves when they 
                            // are triggered. The event should be removed 
                            // when the intrinsic/item is lost/removed.
const uchar POLYMORPH=10;   // Replacing event..
const uchar AWAKE=11;     
const uchar SLEEP=12;       // This event is for the Amulet of restful sleep
                            // to cause the hero to sleep again sometime
                            // after they are scheduled to wake up. 
const uchar UNPARALYZED=13;

class eventitem{
public:
    monstclass *mon;
    uint time;
    eventitem *next;
    eventitem *prev;
                                        // time is CHANGE in time from current
                                        // time
    eventitem(monstclass *m,uint t):mon(m),time(t),next(NULL),prev(NULL){}
    virtual void doevent(){}
    virtual uchar item(){ return NOTHING;}
    // is called  when the item is removed in case it needs to do something.
    // needed for removing items from event list.
    virtual void remove(){}
};

class eventlist{
    eventitem *head;
    uint currtime;
    eventitem *find(uchar type,monstclass *mon);
    void removeitem(eventitem *item);
public:
    eventlist():head(NULL),currtime(0){}
    uint operator++(int);
    void addevent(eventitem *d);
    void doevent();                         // Also increments timer
    void removeevent(monstclass *m,uchar type);  // removes event of type for monster m. 
                                            // should onle be one of each type
                                            // for each monster.
    void removeeventformon(monstclass *m);  // removes ALL events for monster
                                            // should do this before a monster
                                            // is deleted.
    uchar eventexists(uchar type,monstclass *mon){return !!find(type,mon);}
                                            // !! makes strickly 1 or 0
};

class unconfused:public eventitem{
public:
    unconfused(monstclass *m,uint time):eventitem(m,time){}
    void doevent();
    uchar item(){return UNCONFUSED;}
};

class unhalu:public eventitem{
public:
    unhalu(monstclass *m,uint time):eventitem(m,time){}
    void doevent();
    uchar item(){return UNHALU;}
};

class unblind:public eventitem{
public:
    unblind(monstclass *m,uint time):eventitem(m,time){}
    void doevent();
    uchar item(){return UNBLIND;}
};

class unblinditem:public eventitem{
public:
    unblinditem(monstclass *m):eventitem(m,MAXTIME){}
    void doevent();
    uchar item(){return UNBLINDITEM;}
};

class unlevitate:public eventitem{
public:
    unlevitate(monstclass *m,uint time):eventitem(m,time){}
    void doevent();
    uchar item(){return UNLEVITATE;}
};

class unlevitateitem:public eventitem{
public:
    unlevitateitem(monstclass *m):eventitem(m,MAXTIME){}
    void doevent(){}
    uchar item(){return UNLEVITATEITEM;}
    void remove();
};

class unwound:public eventitem{
public:
    unwound(monstclass *m,uint time):eventitem(m,time){}
    void doevent();
    uchar item(){return UNWOUND;}
};

class teleport:public eventitem{
public:
    teleport(monstclass *m,uint time):eventitem(m,time){}
    void doevent();
    uchar item(){return TELEPORT;}
};

class polymorph:public eventitem{
public:
    polymorph(monstclass *m,uint time):eventitem(m,time){}
    void doevent();
    uchar item(){return POLYMORPH;}
};

class awake:public eventitem{
public:
    awake(monstclass *m,uint time):eventitem(m,time){}
    void doevent();
    uchar item(){return AWAKE;}
};

class sleep:public eventitem{
public:
    sleep(monstclass *m,uint time):eventitem(m,time){}
    void doevent();
    uchar item(){return SLEEP;}
};

class unstun:public eventitem{
public:
    unstun(monstclass *m,uint time):eventitem(m,time){}
    void doevent();
    uchar item(){return UNSTUN;}
};

class unparalyzed:public eventitem{
public:
    unparalyzed(monstclass *m,uint time):eventitem(m,time){}
    void doevent();
    uchar item(){return UNPARALYZED;}
};
#endif
