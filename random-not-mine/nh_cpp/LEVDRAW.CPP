#include "lelement.h"
#include "level.h"
#include "strings.h"
#include "ansi.h"
#include "hero.h"

void levelelement::draw(){
    cout << 'œ';
}

void stone::draw(){
    cout << ANSI_GRAY << ' ';
}

void vwall::draw(){
    cout << ANSI_GRAY << '³';

}

void hwall::draw(){
    cout << ANSI_GRAY << 'Ä';
}

void tlcorner::draw(){
    cout << ANSI_GRAY <<'Ú';
}

void trcorner::draw(){
    cout << ANSI_GRAY <<'¿';
}

void brcorner::draw(){
    cout << ANSI_GRAY <<'Ù';
}

void blcorner::draw(){
    cout << ANSI_GRAY <<'À';
}

void crosswall::draw(){
    cout << ANSI_GRAY <<'Å';
}

void tuwall::draw(){
    cout << ANSI_GRAY <<'Á';
}

void tdwall::draw(){
    cout << ANSI_GRAY <<'Â';
}

void tlwall::draw(){
    cout << ANSI_GRAY <<'´';
}

void trwall::draw(){
    cout << ANSI_GRAY <<'Ã';
}

void pool::draw(){
    cout << ANSI_BRIGHT_BLUE <<'÷';
}

void moat::draw(){
    cout << ANSI_BLUE <<'÷';
}

void drawbridge_up::draw(){
    cout << ANSI_BROWN <<'º';
}

void lavapool::draw(){
    cout << ANSI_ORANGE <<'÷';
}

void door::draw(){
    cout << ANSI_BROWN <<'þ';
}

void coor::draw(){
    if(lit)
        cout << ANSI_WHITE;
    else
        cout << ANSI_GRAY; 
    cout << '±';
}           

void room::draw(){
    cout << ANSI_GRAY <<'.';
}

void icepool::draw(){
    cout << ANSI_WHITE <<'.';
}

void icemoat::draw(){
    cout << ANSI_WHITE <<'.';
}

void stairsdn::draw(){
    cout << ANSI_GRAY <<'<';
}

void stairsup::draw(){
    cout << ANSI_GRAY <<'>';
}

void ladder::draw(){
    cout << ANSI_BROWN <<'ö';
}

void sink::draw(){
    cout << ANSI_GRAY <<'#';
}

void throne::draw(){
    cout << ANSI_YELLOW <<'/';
}

void altar::draw(){
    cout << ANSI_GRAY <<'_';
}

void air::draw(){
    cout << ANSI_BLACK <<' ';
}

void cloud::draw(){
    cout << ANSI_WHITE <<'#';
}

void fountain::draw(){
    cout << ANSI_BRIGHT_BLUE <<'ô';
}

void drawbridge_down::draw(){
    cout << ANSI_BROWN <<'#';
}

void level::draw(hero& u){
    int i,j=0;
    uchar ux,uy;
    cout << ANSI_CLEAR <<endl;
//    u.getpos(ux,uy);    


    for(i=0;i<LEVROW;i++){
        for(j=0;j<LEVCOL;j++)
            //
            //  Since we think x then y     -> |
            //                                 V
            //  
            //  but we print  y then x     |
            //                             V->
            //
            //  we reverse i and j and the indexes
            //
            drawelement(j,i);
        cout <<endl;
    };

}

void level::updateelement(uchar x, uchar y){
    char movestring[12];
    char digit[3];

    strcpy(movestring,"\x1b[");
    itoa(y+2,digit,10);
    strcat(movestring,digit);
    strcat(movestring,";");
    itoa(x+1,digit,10);
    strcat(movestring,digit);
    strcat(movestring,"H");
    cout << movestring;
    drawelement(x,y);
    movehome();
}

void level::drawelement(uchar x,uchar y){
    if(monat(x,y))
        monat(x,y)->draw();
    else if (objlistat(x,y).getnumitems())
        objlistat(x,y).draw();
    else
        lev[x][y]->draw();
}


