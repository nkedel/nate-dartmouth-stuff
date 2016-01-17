#ifndef SCROLL_H
#define SCROLL_H

#include "objects.h"

class scrollclass:public objectclass{

public:
    scrollclass();
    void draw();
    uchar isscroll(){return(TRUE);}
    uchar getclass(){return(SCROLL_CLASS);}
    void printname();
    static objectclass *create() {return NULL;}
};

static char *sname[]={
    "ZELGO MER",
    "JUYED AWK YACC",
    "NR 9",
    "XIXAXA XOXAXA XUXAXA",
    "PRATYAVAYAH",
    "DAIYEN FOOELS",
    "LEP GEX VEN ZEA",
    "PRIRUTSENIE",
    "ELBIB YLOH",
    "VERR YED HORRE",
    "VENZAR BORGAVVE",
    "THARR",
    "YUM YUM",
    "KERNOD WEL",
    "ELAM EBOW",
    "DUAM XNAHT",
    "ANDOVA BEGARIN",
    "VE FORBRYDERNE",
    "HACKEM MUCHE",
    // From here on are extra
    "VELOX NEB",
    "FOOBIE BLETCH",
    "TEMOV",
    "GARVEN DEH",
    "READ ME",
    "KIRJE"
};

void randomizescrolls();

objectclass * randomscroll();

class scroll_of_enchant_armor:public scrollclass{
public:
    scroll_of_enchant_armor();
    void read();
    static objectclass *create(){return new scroll_of_enchant_armor;}
};

class scroll_of_destroy_armor:public scrollclass{
public:
    scroll_of_destroy_armor();
    void read();
    static objectclass *create(){return new scroll_of_destroy_armor;}
};

class scroll_of_confuse_monster:public scrollclass{
public:
    scroll_of_confuse_monster();
    void read();
    static objectclass *create(){return new scroll_of_confuse_monster;}
};

class scroll_of_scare_monster:public scrollclass{
public:
    scroll_of_scare_monster();
    void read();
    static objectclass *create(){return new scroll_of_scare_monster;}
};

class scroll_of_remove_curse:public scrollclass{
public:
    scroll_of_remove_curse();
    void read();
    static objectclass *create(){return new scroll_of_remove_curse;}
};

class scroll_of_enchant_weapon:public scrollclass{
public:
    scroll_of_enchant_weapon();
    void read();
    static objectclass *create(){return new scroll_of_enchant_weapon;}
};

class scroll_of_create_monster:public scrollclass{
public:
    scroll_of_create_monster();
    void read();
    static objectclass *create(){return new scroll_of_create_monster;}
};

class scroll_of_taming:public scrollclass{
public:
    scroll_of_taming();
    void read();
    static objectclass *create(){return new scroll_of_taming;}
};

class scroll_of_genocide:public scrollclass{
public:
    scroll_of_genocide();
    void read();
    static objectclass *create(){return new scroll_of_genocide;}
};

class scroll_of_light:public scrollclass{
public:
    scroll_of_light();
    void read();
    static objectclass *create(){return new scroll_of_light;}
};

class scroll_of_teleportation:public scrollclass{
public:
    scroll_of_teleportation();
    void read();
    static objectclass *create(){return new scroll_of_teleportation;}
};

class scroll_of_gold_detection:public scrollclass{
public:
    scroll_of_gold_detection();
    void read();
    static objectclass *create(){return new scroll_of_gold_detection;}
};

class scroll_of_food_detection:public scrollclass{
public:
    scroll_of_food_detection();
    void read();
    static objectclass *create(){return new scroll_of_food_detection;}
};

class scroll_of_identify:public scrollclass{
public:
    scroll_of_identify();
    void read();
    static objectclass *create(){return new scroll_of_identify;}
};

class scroll_of_magic_mapping:public scrollclass{
public:
    scroll_of_magic_mapping();
    void read();
    static objectclass *create(){return new scroll_of_magic_mapping;}
};

class scroll_of_amnesia:public scrollclass{
public:
    scroll_of_amnesia();
    void read();
    static objectclass *create(){return new scroll_of_amnesia;}
};

class scroll_of_fire:public scrollclass{
public:
    scroll_of_fire();
    void read();
    static objectclass *create(){return new scroll_of_fire;}
};

class scroll_of_punishment:public scrollclass{
public:
    scroll_of_punishment();
    void read();
    static objectclass *create(){return new scroll_of_punishment;}
};

class scroll_of_charging:public scrollclass{
public:
    scroll_of_charging();
    void read();
    static objectclass *create(){return new scroll_of_charging;}
};

class scroll_of_blank_paper:public scrollclass{
public:
    scroll_of_blank_paper();
    void read();
    static objectclass *create(){return new scroll_of_blank_paper;}
};

// Here only for completeness. 

class scroll_of_mail:public scrollclass{
public:
    scroll_of_mail();
    void read();
    // Mail isn't randomly generated. it shouldn't need create.
};

#endif
