#include "lelement.h"    

uchar levelelement::canenter(monstclass *m){
    // also need to take into account boulders
    // and tight squeezes with size.
    // monsters who can pass/dig through stone.
    return (!mon)&&(!isstone());
}

void levelelement::enter(monstclass *m){
    mon=m;
    mon->setpos(x,y);
    mon->setlev(&lev);
    monmoved=1;
}

uchar levelelement::canleave(){
    // only takes into account being stuck be something in the element.
    // should take into account hugging monsters.
    return TRUE;
}

void levelelement::leave(){
    mon=NULL;
    monmoved=0;
}

void sink::quaff(){
    message("You drink from the sink.");
}

void pool::quaff(){
    message("You drink from the pool");
}

void moat::quaff(){
    message("You drink from the moat");
}

void lavapool::quaff(){
    message("You drink lava.");
}
