#ifndef HERO_H
#define HERO_H

#include "monst.h"
#include "level.h"

class hero:public monstclass{
protected:
    objpack pack;               // heros pack.
    uchar str;                  // Strength
                                // 0-18 str 0-18
                                // 19-118 str 18/??
                                // 119-126 str 19-25
    Bitfields(intel,5);         // Intelligence
    Bitfields(wis,5);           // Wisdom
    Bitfields(dex,5);           // Dexterity
    Bitfields(con,5);           // Constitution
    Bitfields(cha,5);           // Charisma
    uint xp;                   // experience points.
    sshort food;                // food level
    uchar foodcons;             // food consumed per turn
    sshort water;               // water level
    uchar watercons;            // water consumed per turn
    Bitfields(fainting,1);
    Bitfields(haveqobj,1);
    Bitfields(havebell,1);
    Bitfields(havecandel,1);
    Bitfields(havebook,1);
    Bitfields(haveamulet,1);
public:
    hero();
    uchar ishero(){return TRUE;}
    uchar attack(monstclass *mon);
    void draw();
    uchar takedamage(uint damage){};
    void pickup();
    void inventory();
    void quaff();
    void identify();
    void move(char dir);
    void getpos(uchar& nx,uchar& ny){nx=x;ny=y;} 
    void changestr(schar diff);
    void changeint(schar diff);
    void changewis(schar diff);
    void changedex(schar diff);
    void changecon(schar diff);
    void changecha(schar diff);
    uchar getstr(){return(str);}
    uchar getint(){return((uchar)intel);}
    uchar getwis(){return((uchar)wis);}
    uchar getdex(){return((uchar)dex);}
    uchar getcon(){return((uchar)con);}
    uchar getcha(){return((uchar)cha);}
    uchar maxstr(){return((uchar)126);}
    uchar maxint(){return((uchar)25);}
    uchar maxwis(){return((uchar)25);}
    uchar maxdex(){return((uchar)25);}
    uchar maxcon(){return((uchar)25);}
    uchar maxcha(){return((uchar)25);}
};

#endif
