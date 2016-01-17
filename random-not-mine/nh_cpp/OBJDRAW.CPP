#include "ansi.h"
#include "objects.h"
#include "potion.h"
#include "scroll.h"
#include "weapon.h"
#include "armor.h"

void potionclass::draw(){
    cout << getcolor(color) << '!';
}

void scrollclass::draw(){
    cout << getcolor(color) <<'?';
}

void armorclass::draw(){
    cout << getcolor(color) << '[';
}

void weaponclass::draw(){
    cout << getcolor(color) << ')';
}


