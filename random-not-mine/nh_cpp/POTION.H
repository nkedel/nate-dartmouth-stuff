#ifndef POTION_H
#define POTION_H
#include "global.h"
#include "objects.h"
#include "monst.h"
#include "hero.h"

objectclass *randompotion();

struct colordesc{ 
    uchar color;
    char *desc;
};

static struct colordesc pcolor[]={
    {RED,"ruby"},                   // gain ability
    {BRIGHT_MAGENTA,"pink"},        // restore ability
    {ORANGE,"orange"},              // confusion
    {YELLOW,"yellow"},              // blindness
    {BRIGHT_GREEN,"emerald"},       // paralysis
    {GREEN,"dark green"},           // speed
    {CYAN,"cyan"},                  // levitation
    {CYAN,"sky blue"},              // hallucination
    {BRIGHT_BLUE,"brilliant blue"}, // invisibiliy
    {MAGENTA,"magenta"},            // see invisible
    {MAGENTA,"purple-red"},         // healing
    {RED,"puce"},                   // extra healing
    {WHITE,"milky"},                // gain level
    {BROWN,"swirly"},               // enlightenment
    {WHITE,"bubbly"},               // monster detection
    {GRAY,"smoky"},                 // object detection
    {WHITE,"cloudy"}                // gain energy
};

void randomizepotions();

virtual class potionclass : public objectclass {
protected:        
        Bitfields(diluted,1);
        Bitfields(nutvalue,4);
        Bitfields(unused,2);        
public:
        potionclass::potionclass();
        void dipinto(objectclass *obj){cout <<"It is dipped into it."<<endl;}
        void draw();
        uchar ispotion(){return(TRUE);}
        uchar getclass(){return(POTION_CLASS);}
        void printname();
        static objectclass *create() {return NULL;}
//        friend ostream& operator<<(ostream& s,potionclass& o);
};
class potion_of_gain_ability:public potionclass{
public:
    potion_of_gain_ability();
    void quaff(monstclass *mon);
    void breathe(monstclass* mon);
    static objectclass *create(){return new potion_of_gain_ability;}
};
class potion_of_restore_ability:public potionclass{
public:
    potion_of_restore_ability();
    void quaff(monstclass *mon);
    void breathe(monstclass* mon);
    static objectclass *create(){return new potion_of_restore_ability;}
};
class potion_of_confusion:public potionclass{
public:
    potion_of_confusion();
    void quaff(monstclass *mon);
    void breathe(monstclass* mon);
    static objectclass *create(){return new potion_of_confusion;}
};
class potion_of_blindness:public potionclass{
public:
    potion_of_blindness();
    void quaff(monstclass *mon);
    void breathe(monstclass* mon);
    static objectclass *create(){return new potion_of_blindness;}
};
class potion_of_paralysis:public potionclass{
public:
    potion_of_paralysis();
    void quaff(monstclass *mon);
    void breathe(monstclass* mon);
    static objectclass *create(){return new potion_of_paralysis;}
};
class potion_of_speed:public potionclass{
public:
    potion_of_speed();
    void quaff(monstclass *mon);
    void breathe(monstclass* mon);
    static objectclass *create(){return new potion_of_speed;}
};
class potion_of_levitation:public potionclass{
public:
    potion_of_levitation();
    void quaff(monstclass *mon);
    void breathe(monstclass* mon);
    static objectclass *create(){return new potion_of_levitation;}
};
class potion_of_hallucination:public potionclass{
public:
    potion_of_hallucination();
    void quaff(monstclass *mon);
    void breathe(monstclass* mon);
    static objectclass *create(){return new potion_of_hallucination;}
};
class potion_of_invisibility:public potionclass{
public:
    potion_of_invisibility();
    void quaff(monstclass *mon);
    void breathe(monstclass* mon);
    static objectclass *create(){return new potion_of_invisibility;}
};
class potion_of_see_invisible:public potionclass{
public:
    potion_of_see_invisible();
    void quaff(monstclass *mon);
    void breathe(monstclass* mon);
    static objectclass *create(){return new potion_of_see_invisible;}
};
class potion_of_healing:public potionclass{
public:
    potion_of_healing();
    void quaff(monstclass *mon);
    void breathe(monstclass* mon);
    static objectclass *create(){return new potion_of_healing;}
};
class potion_of_extra_healing:public potionclass{
public:
    potion_of_extra_healing();
    void quaff(monstclass *mon);
    void breathe(monstclass* mon);
    static objectclass *create(){return new potion_of_extra_healing;}
};
class potion_of_gain_level:public potionclass{
public:
    potion_of_gain_level();
    void quaff(monstclass *mon);
    void breathe(monstclass* mon);
    static objectclass *create(){return new potion_of_gain_level;}
};
class potion_of_enlightenment:public potionclass{
public:
    potion_of_enlightenment();
    void quaff(monstclass *mon);
    void breathe(monstclass* mon);
    static objectclass *create(){return new potion_of_enlightenment;}
};
class potion_of_monster_detection:public potionclass{
public:
    potion_of_monster_detection();
    void quaff(monstclass *mon);
    void breathe(monstclass* mon);
    static objectclass *create(){return new potion_of_monster_detection;}
};
class potion_of_object_detection:public potionclass{
public:
    potion_of_object_detection();
    void quaff(monstclass *mon);
    void breathe(monstclass* mon);
    static objectclass *create(){return new potion_of_object_detection;}
};
class potion_of_gain_energy:public potionclass{
public:
    potion_of_gain_energy();
    void quaff(monstclass *mon);
    void breathe(monstclass* mon);
    static objectclass *create(){return new potion_of_gain_energy;}
};
class potion_of_booze:public potionclass{
public:
    potion_of_booze();
    void quaff(monstclass *mon);
    void breathe(monstclass* mon);
    static objectclass *create(){return new potion_of_booze;}
};
class potion_of_sickness:public potionclass{
public:
    potion_of_sickness();
    void quaff(monstclass *mon);
    void breathe(monstclass* mon);
    static objectclass *create(){return new potion_of_sickness;}
};
class potion_of_fruit_juice:public potionclass{
public:
    potion_of_fruit_juice();
    void quaff(monstclass *mon);
    void breathe(monstclass* mon);
    static objectclass *create(){return new potion_of_fruit_juice;}
};
class potion_of_water:public potionclass{
public:
    potion_of_water();
    void quaff(monstclass *mon);
    void breathe(monstclass* mon);
    static objectclass *create(){return new potion_of_water;}
};
#endif
