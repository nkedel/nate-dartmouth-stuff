#include "hero.h"

uchar hero::attack(monstclass *mon){
    uint roll;

    roll=rnd1(20);
    cout << roll;
    if(roll>15)
        return(mon->takedamage(rnd1(8)));
    return(FALSE);
}

hero::hero(){

    x=y=0;
    str=0;
    intel=0;
    wis=0;
    dex=0;
    con=0;
    cha=0;
    xp=0;
    food=0;
    foodcons=0;
    water=0;
    watercons=0;
    fainting=0;
    haveqobj=0;
    havebell=0;
    havecandel=0;
    havebook=0;
    haveamulet=0;
}


void hero::move(char dir){

    schar dx=0,dy=0;
    schar tox=0,toy=0;

    switch (dir){
        case 'b':
        case '1':dx=-1;dy=1;break;
        case 'j':
        case '2':dy=1;break;
        case 'n':
        case '3':dy=1;dx=1;break;
        case 'h':
        case '4':dx=-1;break;
        case 'l':
        case '6':dx=1;break;
        case 'y':
        case '7':dx=-1;dy=-1;break;
        case 'k':
        case '8':dy=-1;break;
        case 'u':
        case '9':dy=-1;dx=1;break;
    };
    toy=y+dy;
    tox=x+dx;
    if (tox<0||tox>LEVCOL)
        return;
    if (toy<0||toy>LEVROW)
        return;
    if (currentlev->monat(tox,toy)){
        if(attack(currentlev->monat(tox,toy))){
            // mon died. lelement can handle it now, it needs to be moved.
            currentlev->removemonat(tox,toy);
            currentlev->updateelement(tox,toy);
        }
    } 
    else if(currentlev->canenter(tox,toy,this)){
        currentlev->movemonfromto(x,y,tox,toy);
    }
    else
        return;
}


void hero::pickup(){

    objectlist& olist=currentlev->objlistat(x,y);

    if(olist.getnumitems()==1)
        pack.add(olist.remove());
}

void hero::inventory(){
    objectclass *otmp;
    char in;

    char i;

    cout << ANSI_CLEAR<<endl;
    for(i='a';i<'z';i++)
        if(pack[i]){
            cout << i<<" - ";
            pack[i]->printname();
        }
    for(i='A';i<'Z';i++)
        if(pack[i]){
            cout << i << " - ";
            pack[i]->printname();
        }
    in=cin.get();
    in=cin.get();
    currentlev->draw(*this);
}

void hero::quaff(){
    char in;
    objectclass *otmp;
    objectlist *olist;

    if(currentlev->getelement(x,y)->isdrinkable()){
        // need to put name of element. Fountain, sink etc.
        message ("Do you want to drink from this?");
        in=cin.get();
        in=cin.get();
        if(in=='y')
            currentlev->getelement(x,y)->quaff(*this);
    }
    // make potion list
    olist=pack.getlistofclass(POTION_CLASS);
    if(olist->getnumitems()){
        displaylist(olist);
        // prompt for input
        in=cin.get();   //get return
        in=cin.get();
        // redraw map
        currentlev->draw(*this);
        // quaf appropriate one.
        if(((in>='a')&&(in<='z'))||((in>='A')&&(in<='Z'))){
            otmp=pack.remove(in);
            if(otmp){
                // make sure there is something there.
                otmp->quaff(this);
                otmp=otmp->useup();
                if(otmp)
                    pack.add(otmp);
            } else {
                message("You don't have that item!");
            }
        }
    }
    delete olist;
}

void hero::identify(){
    objectclass *otmp;
    char c;
    for(c='a';c<'z';c++)
        if(pack[c]) pack[c]->identify();
    for(c='A';c<'Z';c++)
        if(pack[c]) pack[c]->identify();
}

void hero::changestr(schar diff){
    if((str==126)&&(diff>0))    
        message("You are already as strong as you can get!");
    if((str==3)&&(diff<0))
        message("You are already as weak as you can get!");

    str+=diff;
    
    if(str>126)
        str=126;
    if(str<3)
        str=3;
}
void hero::changeint(schar diff){
    if((intel==25)&&(diff>0))
        message("You are already as intelegent as you can get!");
    if((intel==3)&&(diff<0))
        message("You are already as dumb as you can get!");
    
    intel+=diff;

    if(intel>25)
        intel=25;
    if(intel<3)
        intel=3;
}
void hero::changewis(schar diff){
    if((wis==25)&&(diff>0))
        message("You are already as wise as you can get!");
    if((wis==3)&&(diff<0))
        message("You are already as ignorant as you can get!");
    
    wis+=diff;

    if(wis>25)
        wis=25;
    if(wis<3)
        wis=3;
}
void hero::changedex(schar diff){
    if((dex==25)&&(diff>0))    
        message("You are already as agile as you can get!");
    if((dex==3)&&(diff<0))
        message("You are already as clumsy as you can get!");
    
    dex+=diff;

    if(dex>25)
        dex=25;
    if(dex<3)
        dex=3;
}
void hero::changecon(schar diff){
    if((con==25)&&(diff>0))    
        message("You are already as healthy as you can get!");
    if((con==3)&&(diff<0))
        message("You are already as weak as you can get!");
    
    con+=diff;

    if(con>25)
        con=25;
    if(con<3)
        con=3;
}
void hero::changecha(schar diff){
    if((cha==25)&&(diff>0))    
        message("You are already as charasmatic as you can get!");
    if((cha==3)&&(diff<0))
        message("You are already as ugly as you can get!");
    
    cha+=diff;

    if(cha>25)
        cha=25;
    if(cha<3)
        cha=3;
}

