#include "potion.h"

potionclass::potionclass(){
    otype=OTYPE_GLASS;
    weight=20;
    nutvalue=10;
    merge=1;
}

potion_of_gain_ability::potion_of_gain_ability(){
    desc=pcolor[0].desc;
    name="gain ability";
    color=pcolor[0].color;
    price=300;
}

void potion_of_gain_ability::quaff(monstclass *mon){
}

void potion_of_gain_ability::breathe(monstclass *mon){

}

potion_of_restore_ability::potion_of_restore_ability(){
    desc=pcolor[1].desc;
    name="restore ability";
    color=pcolor[1].color;
    price=100;
}

void potion_of_restore_ability::quaff(monstclass *mon){

}

void potion_of_restore_ability::breathe(monstclass *mon){

}

potion_of_confusion::potion_of_confusion(){
    desc=pcolor[2].desc;
    name="confusion";
    color=pcolor[2].color;
    price=100;
}

void potion_of_confusion::quaff(monstclass *mon){
    mon->makeconfused(rnd1(30)+30);
}

void potion_of_confusion::breathe(monstclass *mon){
    mon->makeconfused(rnd1(20)+20);
}

potion_of_blindness::potion_of_blindness(){
    desc=pcolor[3].desc;
    name="blindness";
    color=pcolor[3].color;
    price=150;
}

void potion_of_blindness::quaff(monstclass *mon){
    mon->makeblinded(rnd1(30)+30);
}

void potion_of_blindness::breathe(monstclass *mon){
    mon->makeblinded(rnd1(20)+20);
}

potion_of_paralysis::potion_of_paralysis(){
    desc=pcolor[4].desc;
    name="paralysis";
    color=pcolor[4].color;
    price=300;
}


void potion_of_paralysis::quaff(monstclass *mon){
    mon->makeparalyzed(rnd1(30)+30);
}

void potion_of_paralysis::breathe(monstclass *mon){
    mon->makeparalyzed(rnd1(20)+20);
}

potion_of_speed::potion_of_speed(){
    desc=pcolor[5].desc;
    name="speed";
    color=pcolor[5].color;
    price=200;
}

void potion_of_speed::quaff(monstclass *mon){

}

void potion_of_speed::breathe(monstclass *mon){

}

potion_of_levitation::potion_of_levitation(){
    desc=pcolor[6].desc;
    name="levitation";
    color=pcolor[6].color;
    price=200;
}

void potion_of_levitation::quaff(monstclass *mon){
    mon->makelevitate(rnd1(30)+30);
}

void potion_of_levitation::breathe(monstclass *mon){
    mon->makelevitate(rnd1(20)+20);
}

potion_of_hallucination::potion_of_hallucination(){
    desc=pcolor[7].desc;
    name="halucination";
    color=pcolor[7].color;
    price=100;
}

void potion_of_hallucination::quaff(monstclass *mon){

}

void potion_of_hallucination::breathe(monstclass *mon){

}

potion_of_invisibility::potion_of_invisibility(){
    desc=pcolor[8].desc;
    name="invisibility";
    color=pcolor[8].color;
    price=150;
}

void potion_of_invisibility::quaff(monstclass *mon){

}

void potion_of_invisibility::breathe(monstclass *mon){

}

potion_of_see_invisible::potion_of_see_invisible(){
    desc=pcolor[9].desc;
    name="see invisibile";
    color=pcolor[9].color;
    price=50;
}

void potion_of_see_invisible::quaff(monstclass *mon){

}

void potion_of_see_invisible::breathe(monstclass *mon){

}

potion_of_healing::potion_of_healing(){
    desc=pcolor[10].desc;
    name="healing";
    color=pcolor[10].color;
    price=100;
}

void potion_of_healing::quaff(monstclass *mon){

}

void potion_of_healing::breathe(monstclass *mon){

}

potion_of_extra_healing::potion_of_extra_healing(){
    desc=pcolor[11].desc;
    name="extra healing";
    color=pcolor[11].color;
    price=100;
}

void potion_of_extra_healing::quaff(monstclass *mon){

}

void potion_of_extra_healing::breathe(monstclass *mon){

}

potion_of_gain_level::potion_of_gain_level(){
    desc=pcolor[12].desc;
    name="gain level";
    color=pcolor[12].color;
    price=300;
}

void potion_of_gain_level::quaff(monstclass *mon){

}

void potion_of_gain_level::breathe(monstclass *mon){

}

potion_of_enlightenment::potion_of_enlightenment(){
    desc=pcolor[13].desc;
    name="enlightenment";
    color=pcolor[13].color;
    price=200;
}

void potion_of_enlightenment::quaff(monstclass *mon){

}

void potion_of_enlightenment::breathe(monstclass *mon){

}

potion_of_monster_detection::potion_of_monster_detection(){
    desc=pcolor[14].desc;
    name="monster detection";
    color=pcolor[14].color;
    price=150;
}

void potion_of_monster_detection::quaff(monstclass *mon){

}

void potion_of_monster_detection::breathe(monstclass *mon){

}

potion_of_object_detection::potion_of_object_detection(){
    desc=pcolor[15].desc;
    name="object detection";
    color=pcolor[15].color;
    price=150;
}

void potion_of_object_detection::quaff(monstclass *mon){

}

void potion_of_object_detection::breathe(monstclass *mon){

}

potion_of_gain_energy::potion_of_gain_energy(){
    desc=pcolor[16].desc;
    name="gain energy";
    color=pcolor[16].color;
    price=150;
}

void potion_of_gain_energy::quaff(monstclass *mon){

}

void potion_of_gain_energy::breathe(monstclass *mon){

}

potion_of_booze::potion_of_booze(){
    // This color doesn't change
    desc="brown";
    name="booze";
    color=BROWN;
    price=50;
}

void potion_of_booze::quaff(monstclass *mon){

}

void potion_of_booze::breathe(monstclass *mon){

}

potion_of_sickness::potion_of_sickness(){
    // This color doesn't change
    desc="fizzy";
    name="sickness";
    color=CYAN;
    price=50;
}

void potion_of_sickness::quaff(monstclass *mon){

}

void potion_of_sickness::breathe(monstclass *mon){

}

potion_of_fruit_juice::potion_of_fruit_juice(){
    // This color doesn't change
    desc="dark";
    name="fruit juice";
    color=BRIGHT_BLACK;
    price=50;
}

void potion_of_fruit_juice::quaff(monstclass *mon){

}

void potion_of_fruit_juice::breathe(monstclass *mon){

}

potion_of_water::potion_of_water(){
    // This color doesn't change
    desc="clear";
    name="water";
    color=CYAN;
    price=100;
}

void potion_of_water::quaff(monstclass *mon){

}

void potion_of_water::breathe(monstclass *mon){

}

// Needs work to be more complete.

void potionclass::printname(){

if (curseknown && cursed)
    cout << "cursed ";
else if(curseknown && blessed)
    cout << "blessed ";
else
    cout << "uncursed ";

if (nameknown)
    cout << "potion of "<< name;
else
    cout << desc << " potion";

//if (called)
//    cout << " called " << called;

cout << endl;

}

// This function generates a random item according the to probabilities
// in the array. It is accurate to Nethack 3.1.3 algorithms.

objectclass *randompotion(){
    
    struct pinfo{
        objectclass *(*fp)();       // Function that creates the object
        uchar prob;                 // probability of object being created.
    };
    
    struct pinfo pinfo[]={
    // Put all the class create() function is here that are
    // possible candidates for this random object generator.
    // The sum of all the probabilities in this list must be 1000.
    // Think of it as XX.X% chance of being generated.
    {potion_of_gain_ability::create,45},
    {potion_of_restore_ability::create,45},
    {potion_of_confusion::create,45},
    {potion_of_blindness::create,45},
    {potion_of_paralysis::create,45},
    {potion_of_speed::create,45},
    {potion_of_levitation::create,45},
    {potion_of_hallucination::create,45},
    {potion_of_invisibility::create,45},
    {potion_of_see_invisible::create,45},
    {potion_of_healing::create,65},
    {potion_of_extra_healing::create,50},
    {potion_of_gain_level::create,20},
    {potion_of_enlightenment::create,20},
    {potion_of_monster_detection::create,45},
    {potion_of_object_detection::create,45},
    {potion_of_gain_energy::create,45},
    {potion_of_booze::create,45},
    {potion_of_sickness::create,45},
    {potion_of_fruit_juice::create,45},
    {potion_of_water::create,125}
    // make sure the last one does not have a trailing comma
    };

    sshort prob;
    uchar count=0;

    prob=rnd0(1000);
    while((prob -= pinfo[count].prob) > 0) count++;
    return (pinfo[count].fp)();
}

// This function takes pcolor[] (declared in potion.h) and moves the 
// structs around.  This algorithm may not be strictly accurate to 
// Nethack 3.1.3.  It should only be called once at the start of the game.

void randomizepotions(){

    uchar numpotions=sizeof(pcolor)/sizeof(struct colordesc);
    struct colordesc tmp;
    uchar changeto=0;
    
    for(int i=0;i<numpotions;i++){
        changeto=rnd0(numpotions-1);
        // move destination to tmp
        tmp.desc=pcolor[changeto].desc;
        tmp.color=pcolor[changeto].color;
        // move current index to destination
        pcolor[changeto].desc=pcolor[i].desc;
        pcolor[changeto].color=pcolor[i].color;
        // move tmp to current index
        pcolor[i].desc=tmp.desc;
        pcolor[i].color=tmp.color;
    }
}

// Needs work to be more complete.
/*
ostream& potionclass::operator<<(ostream& s,potionclass& o){

    if (o.curseknown && o.cursed)
        s << "cursed ";
    else if(o.curseknown && o.blessed)
        s << "blessed ";
    else        
        s << "uncursed ";

    if (o.nameknown)
        s << "potion of "<< o.name;
    else        
        s << o.desc << " potion";

    //if (o.called)
    //    cout << " called " << o.called;

    s << endl;

    return s;
}

*/
