#include "monst.h"
#include "hero.h"
#include "ansi.h"

void monster::draw(){
    cout << 'û';
}

void human::draw(){
    cout << "\x1b[37;1m" << '@';
}

void hero::draw(){
    cout << "\x1b[37;1m" << '@';
}

void lizardgroup::draw(){
    cout << getcolor(color) << ':';
}
