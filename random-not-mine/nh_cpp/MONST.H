#ifndef MONST_H
#define MONST_H

#include "global.h"
#include "objlist.h"
#include "event.h"

class level;

virtual class monstclass {
protected:
    
    uchar x,y;                  // current position
    level* currentlev;          // current level pointer
    
    sint hp,hpmax;              // mons current and max HP.
    sshort alignment;           // alignment value
    uint gold;                  // amount of gold
    ushort pluck;               // positive luck
    ushort nluck;               // negative luck
    uchar hd;                   // base level
    uchar baseac;               // base AC
    ushort energy;              // current energy level
    ushort maxenergy;           // max energy level
    uchar speed;                // monsters have more variable speed than heros
    uchar mr;                   // magic resistance
    uchar color;                // from color.h
    uchar sightdistance;        // How far a monster can see without light.
                                // light producing objects should modify
                                // this.
    // object related things.

    objectclass *wielded;        // wielded object
    objectclass *helm;           // helm worn
    objectclass *armor;          // armor worn
    objectclass *gloves;         // gloves worn
    objectclass *boots;          // boots worn
    objectclass *cloak;          // cloak worn
    objectclass *rring;          // right ring worn
    objectclass *lring;          // left ring worn
    objectclass *amulet;         // amulet worn
    objectclass *blindfold;      // blindfold worn

    Bitfields(were,2);          // 0 not, 1 wererat, 2 werewolf

    // states of being

    Bitfields(paralyzed,1);
    Bitfields(female,1);
    Bitfields(stunned,1);
    Bitfields(confused,1);
    Bitfields(blinded,1);
    Bitfields(sleeping,1);
    Bitfields(halu,1);
    Bitfields(sick,1);    
    Bitfields(telepathic,1);
    Bitfields(heatres,1);
    Bitfields(coldres,1);
    Bitfields(poisonres,1);
    Bitfields(disintres,1);
    Bitfields(sleepres,1);
    Bitfields(stealthy,1);
    Bitfields(shockres,1);
    Bitfields(magicres,1);
    Bitfields(halures,1);
    Bitfields(clairvoyant,1);
    Bitfields(protect,1);
    Bitfields(adorned,1);
    Bitfields(regenerate,1);
    Bitfields(searching,1);
    Bitfields(seeinvis,1);
    Bitfields(invis,1);
    Bitfields(tele,1);
    Bitfields(telecont,1);
    Bitfields(poly,1);
    Bitfields(polycont,1);
    Bitfields(levitate,1);
    Bitfields(agrivate,1);
    Bitfields(conflict,1);
    Bitfields(protshape,1);
    Bitfields(warning,1);
    Bitfields(woundedlegs,1);
    Bitfields(lifesaving,1);
    Bitfields(fumbling,1);
    Bitfields(jumping,1);
    Bitfields(wwalking,1);
    Bitfields(hunger,1);
    Bitfields(reflecting,1);
    Bitfields(displaced,1);
    Bitfields(vomiting,1);
    Bitfields(energyregen,1);
    Bitfields(magicbreath,1);
    Bitfields(halfsdam,1);
    Bitfields(halfpdam,1);
    // monsters don't worry about food.


public:
    monstclass();
    ~monstclass(){timer.removeeventformon(this);}
    virtual void draw(){};
    virtual uchar takedamage(uint damage){};

    // These methods are for manipulating the many attributed. 
    // names are    make<attribute>()
    // and          make<attribute>(ushort time)
    // if a timeout applys. attributes which time out, automatically place
    // an event to untime out after TIME turns.

    void makeunparalyzed(){paralyzed=0;}
    void makeparalyzed(){paralyzed=1;}
    void makeparalyzed(ushort time);
    uchar isparalyzed(){return paralyzed;}
    void makeunconfused(){confused=0;}
    void makeconfused(ushort time);
    void makeconfused(){confused=1;}        // unconditional. No event placed
    uchar isconfused(){return confused;}
    void makeunhalu(){halu=0;}
    void makehalu(ushort time);
    void makehalu(){halu=1;}        // unconditional. No event placed
    uchar ishallucinating(){return halu;}
    void makeunstunned(){stunned=0;}
    void makestunned(ushort time);
    void makestunned(){stunned=1;}          // unconditional.   " "
    uchar usstunned(){return stunned;}
    void unwoundlegs(){woundedlegs=0;}
    void makelegswounded(ushort time);
    void makelegswounded(){woundedlegs=1;}      // unconditional.   " "
    uchar iswounded(){return woundedlegs;}
    void makeunblind(){blinded=0;}
    void makeblinded(ushort time);
    void makeblinded(){blinded=1;}          // unconditional.   " "
    uchar isblinded(){return blinded;}
    void wake(){sleeping=0;}
    void makesleep(ushort time);
    void makesleep(){sleeping=1;}           // unconditional.   " "
    uchar issleeping(){return sleeping;}
    void makesick(){sick=1;}    
    void maketelepathic(){telepathic=1;}
    void makeheatresistant(){heatres=1;}
    void makecoldresistant(){coldres=1;}
    void makepoisonresistant(){poisonres=1;}
    void makedisintresistant(){disintres=1;}
    void makesleepresistant(){sleepres=1;}
    void makesleathy(){stealthy=1;}
    void makeshockresistant(){shockres=1;}
    void makemagicresistant(){magicres=1;}
    void makehaluresistant(){halures=1;}
    void makeclairvoyant(){clairvoyant=1;}
    void makeprotected(){protect=1;}
    void makeadorned(){adorned=1;}
    void makeregenerate(){regenerate=1;}
    void makesearching(){searching=1;}
    void makeseeinvisible(){seeinvis=1;}
    void makeinvisible(){invis=1;}
    void maketeleport(){tele=1;}
    void maketeleportcontrol(){telecont=1;}
    void makepolymorph(){poly=1;}
    void makepolymorphcontrol(){polycont=1;}
    void makelevitate(){levitate=1;}
    void makelevitate(ushort time);
    void makeunlevitate(){levitate=0;}
    void makeagrivate(){agrivate=1;}
    void makeconflict(){conflict=1;}
    void makeprotectedshapechange(){protshape=1;}
    void makewarning(){warning=1;}
    void makelifesaving(){lifesaving=1;}
    void makefumbling(){fumbling=1;}
    void makejumping(){jumping=1;}
    void makewaterwalking(){wwalking=1;}
    void makehunger(){hunger=1;}
    void makereflecting(){reflecting=1;}
    void makedisplaced(){displaced=1;}
    void makevomiting(){vomiting=1;}
    void makeenergyregnerate(){energyregen=1;}
    void makemagicbreathing(){magicbreath=1;}
    void makehalfspelldam(){halfsdam=1;}
    void makehalfphysdam(){halfpdam=1;}
    virtual uchar ishero(){return FALSE;}
    virtual void quaff(){}
    void setpos(uchar ux,uchar uy){x=ux;y=uy;}
    void setlev(level* l){currentlev=l;}
};

virtual class monster :public monstclass{
protected:
    
    uchar herox,heroy;          // Where mon thinks you are.
    ushort weight;              // weight of body
    uchar nutvalue;             // nutritional value
    monpack pack;               // monsters pack.    

    // helps decide what monsters can and cannot do.
    // should be set in monster's constructor
    
    Bitfields(brainless,1);
    Bitfields(legs,1);
    Bitfields(arms,1);
    Bitfields(head,1);
    Bitfields(eyes,1);
    Bitfields(mouth,1);
    Bitfields(canwield,1);
    Bitfields(canwear,1);


public:

    monster();
    virtual void draw();
    // each monster has up to 6 attacks.  complete one for each. They are 
    // called 1 to 6 if order is important.
    virtual void attack1(monstclass *attackee){}
    virtual void attack2(monstclass *attackee){}
    virtual void attack3(monstclass *attackee){}
    virtual void attack4(monstclass *attackee){}
    virtual void attack5(monstclass *attackee){}
    virtual void attack6(monstclass *attackee){}
    // This just calls all 6 attack functions
    void attack(monstclass *attackee);
    // chance to do something to the attacker when attacked
    // ie. gelatenous cubes and rustmonsters
    virtual void attacked(monstclass *attacker){}  
    void computehp();
    uchar takedamage(uint damage);
    void updatehero(uchar x,uchar y){herox=x;heroy=y;}
};

class human : public monster{

public:
    human(){weight=1450;}
    void draw();

};

class lizardgroup:public monster{
public:
    lizardgroup();
    void draw();
};

class newt:public lizardgroup{
public:
    newt();
    void attack1(monstclass *attackee);
};
#endif

