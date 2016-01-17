#ifndef LEVEL_H
#define LEVEL_H

#include "global.h"
#include "objlist.h"
#include "monst.h"
#include "lelement.h"

#define LEVCOL 79
#define LEVROW 19
class level {
    levelelement *lev[LEVCOL][LEVROW];
    

    Bitfields(numshops,3);          // 7 shops max
    Bitfields(has_zoo,1);
    Bitfields(has_vault,1);
    Bitfields(has_court,1);
    Bitfields(has_morgue,1);
    Bitfields(has_beehive,1);
    // One byte worth
    Bitfields(has_barracks,1);
    Bitfields(has_temple,1);
    Bitfields(hardfloor,1);
    Bitfields(noteleport,1);
    Bitfields(numfountains,4);      //15 fountains max
public:
    
    level(int levelnum);
    ~level();
    void draw(hero& u);
    
    monstclass *removemonat(uchar x,uchar y){return lev[x][y]->removemon();}
    void putmonat(monstclass * mon,uchar x,uchar y){lev[x][y]->putmon(mon);}        
    monstclass *monat(uchar x,uchar y){return lev[x][y]->getmon();}
    void movemonfromto(uchar fromx,uchar fromy,uchar tox, uchar toy);
    void putobj(objectclass *otmp,uchar x,uchar y){lev[x][y]->putobj(otmp);}
    objectlist& objlistat(uchar x,uchar y){return lev[x][y]->getolist();}
    levelelement *getelement(uchar x,uchar y){return (lev[x][y]);}

    uchar canenter(uchar x,uchar y,monstclass *m){return lev[x][y]->canenter(m);}
    uchar canleave(uchar x,uchar y){return lev[x][y]->canleave();}


    void changeelement(uchar x,uchar y, levelelement *el);
    void drawelement(uchar x,uchar y);
    void updateelement(uchar x,uchar y);
    void mkroom(uchar startx,uchar starty,uchar sizex,uchar sizey);
};


#endif
