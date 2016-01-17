#include "display.h"
#include "objlist.h"

void movehome(){
    cout <<"\x1b[21;0H" << ANSI_CLEAR_LINE;
}

void message(char * msg){
    cout << "\x1b[0;0H" << ANSI_CLEAR_LINE << msg << endl;
}

void displaylist(objectlist *olist){
    objectclass *otmp;

    cout <<"\x1b[21;0H" << ANSI_CLEAR_LINE;
    olist->reset();
    while(otmp=olist->get()){
        cout << otmp->getletter()<< " - ";
        otmp->printname();
        olist->next();
    }
}

