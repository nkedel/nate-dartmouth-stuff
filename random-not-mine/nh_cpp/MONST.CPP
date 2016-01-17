#include "monst.h"

monstclass::monstclass(){    

    currentlev=NULL;
    hp=hpmax=0;
    alignment=0;
    gold=0;
    pluck=0;
    nluck=0;
    hd=0;
    baseac=0;
    energy=0;
    maxenergy=0;
    sightdistance=1;

    wielded=helm=armor=gloves=boots=cloak=rring=lring=NULL;
    amulet=blindfold=NULL;
    were=0;

    female=0;
    stunned=0;
    confused=0;
    blinded=0;
    sleeping=0;
    halu=0;
    sick=0;    
    telepathic=0;
    heatres=0;
    coldres=0;
    poisonres=0;
    disintres=0;
    sleepres=0;
    stealthy=0;
    shockres=0;
    magicres=0;
    halures=0;
    speed=0;
    clairvoyant=0;
    protect=0;
    adorned=0;
    regenerate=0;
    searching=0;
    seeinvis=0;
    invis=0;
    tele=0;
    telecont=0;
    poly=0;
    polycont=0;
    levitate=0;
    agrivate=0;
    conflict=0;
    protshape=0;
    warning=0;
    woundedlegs=0;
    lifesaving=0;
    fumbling=0;
    jumping=0;
    wwalking=0;
    hunger=0;
    reflecting=0;
    displaced=0;
    vomiting=0;
    energyregen=0;
    magicbreath=0;
    halfsdam=0;
    halfpdam=0;
}

monster::monster(){
    nutvalue=0;
}

void monster::attack(monstclass *attackee){

    attack1(attackee);
    attack2(attackee);
    attack3(attackee);
    attack4(attackee);
    attack5(attackee);
    attack6(attackee);
}

void monster::computehp(){
    uchar i;

    for(i=0;i<hd;i++)
        hp+=rnd1(8)+1;
    hpmax=hp;
}

lizardgroup::lizardgroup(){
    canwear=FALSE;
    canwield=FALSE;
}

newt::newt(){
    hd=1;
    speed=6;
    mr=8;
    baseac=0;
    color=YELLOW;
    nutvalue=20;
    weight=10;
    monster::computehp();
}

void newt::attack1(monstclass *attackee){
    //if(bite(attackee))
    //    pysical(1,2,attackee);
}


uchar monster::takedamage(uint damage){
    hp-=damage;
    if(hp<0){ 
        delete this;
        return(TRUE);
    }
    return(FALSE);
}

void monstclass::makehalu(ushort time){
    halu=1;
    timer.addevent(new unhalu(this,time));
}

void monstclass::makeconfused(ushort time){
    confused=1;
    timer.addevent(new unconfused(this,time));
}

void monstclass::makelegswounded(ushort time){
    woundedlegs=1;
    timer.addevent(new unwound(this,time));
}

void monstclass::makesleep(ushort time){
    sleeping=1;
    timer.addevent(new awake(this,time));
}
void monstclass::makeblinded(ushort time){
    blinded=1;
    timer.addevent(new unblind(this,time));
}

void monstclass::makestunned(ushort time){
    stunned=1;
    timer.addevent(new unstun(this,time));
}

void monstclass::makeparalyzed(ushort time){
    paralyzed=1;
    timer.addevent(new unparalyzed(this,time));
}

void monstclass::makelevitate(ushort time){
    levitate=1;
    timer.addevent(new unlevitate(this,time));
}

