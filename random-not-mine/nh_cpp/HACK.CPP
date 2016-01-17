#include "hack.h"

void main(){
    uint gametime=0;
    objectclass *otmp;
    srandom((long)time(NULL));
    randomizepotions();
    randomizescrolls();
    level lev1(1);
    char in;
    dungeon dun;
    hero u;
    monstclass *mon;    

    dun.setcurrentlev(&lev1);
    dun.sethero(&u);
    lev1.putmonat(&u,5,10);
    otmp=randompotion();
    lev1.putobj(otmp,5,12);
    otmp=randomscroll();
    lev1.putobj(otmp,5,13);
    //mon=new newt;
    //lev1.putmonat(mon,5,11);
   
    lev1.draw(u);
    in=cin.get();
    while(in!='Q'){
        switch (in){
            case 'h':
            case 'j':
            case 'k':
            case 'l':
            case 'y':
            case 'u':
            case 'b':
            case 'n':
            case '1':   
            case '2':
            case '3':
            case '4':
            case '6':
            case '7':
            case '8':
            case '9': dun.heromove(in);break;
            case 'i': u.inventory();break;
            case ',': u.pickup();break;
            case 'q': u.quaff();break;
            // case 'W':u.wear();break;
            // case 't':u.takeoff();break;
            // case 'r':y.remove();break;
            // case 'p':u.puton();break;
            case 'I':u.identify();break;
        };
        //lev1.draw();
        movehome();
        gametime=timer++;
        in=cin.get();
    }
    cout << "\x1b[37;0m" <<endl;
}
