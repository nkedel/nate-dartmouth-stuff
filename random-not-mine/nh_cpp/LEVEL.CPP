#include "level.h"
#include "potion.h"

level::level(int levelnum){
    int i,j;
    objectclass * otmp;
    levelelement *lel;
    
    numshops=0;
    has_zoo=FALSE;
    has_vault=FALSE;
    has_court=FALSE;
    has_morgue=FALSE;
    has_beehive=FALSE;
    has_barracks=FALSE;
    has_temple=FALSE;
    hardfloor=FALSE;
    noteleport=FALSE;
    numfountains=0;
    for (i=0;i<LEVCOL;i++)
        for(j=0;j<LEVROW;j++){
            lev[i][j]=new stone(*this,i,j);
        }
    // connect all the level elements
    for(i=0;i<LEVCOL;i++)
        for(j=0;j<LEVROW;j++){
            if(j>0) lev[i][j]->setnextup(lev[i][j-1]);
            if(j<LEVROW) lev[i][j]->setnextdown(lev[i][j+1]);
            if(i>0) lev[i][j]->setnextleft(lev[i-1][j]);
            if(i<LEVCOL) lev[i][j]->setnextright(lev[i+1][j]);
        }

    mkroom(4,4,10,10);
    changeelement(5,5,new lavapool(*this,5,5));
    changeelement(7,7,new altar(*this,7,7,RANDOM));
    changeelement(5,6,new pool(*this,5,6));
    changeelement(5,7,new stairsup(*this,5,7));
    changeelement(5,8,new sink(*this,5,8));
    changeelement(5,9,new fountain(*this,5,9));
    changeelement(5,10,new stairsdn(*this,5,10));
}

level::~level(){
    int i,j,k;
    objectclass *otmp;

    for(i=0;i<LEVCOL;i++){
        for(j=0;j<LEVROW;j++){
            delete lev[i][j];
        }
    }
}


void level::movemonfromto(uchar fromx,uchar fromy,uchar tox, uchar toy){
    monstclass *mon;

    mon=monat(fromx,fromy);
    if(lev[fromx][fromy]->canleave()&&lev[tox][toy]->canenter(mon)){
        lev[tox][toy]->enter(mon);
        lev[fromx][fromy]->leave();
        updateelement(fromx,fromy);
        updateelement(tox,toy);
    }
}

void level::changeelement(uchar x,uchar y,levelelement *el){
    
    levelelement *tmp;
    
    tmp=lev[x][y];
    lev[x][y]=el;
    if(y>0) lev[x][y]->setnextup(lev[x][y-1]);
    if(y<LEVROW) lev[x][y]->setnextdown(lev[x][y+1]);
    if(x>0) lev[x][y]->setnextleft(lev[x-1][y]);
    if(x<LEVCOL) lev[x][y]->setnextright(lev[x+1][y]);
    
    delete tmp;
}

void level::mkroom(uchar startx,uchar starty,uchar sx,uchar sy){
    uchar i,j;
    uchar sizex,sizey;

    sizex=sx;
    sizey=sy;

    if((startx+sizex)>LEVCOL)sizex=LEVCOL-startx-1;// Need -1 becuase array is
    if((starty+sizey)>LEVROW)sizey=LEVROW-starty-1;// really 0 based

    // Make the top  left corner
    changeelement(startx,starty,new tlcorner(*this,startx,starty));
    // Make the left vertical wall
    for(i=starty+1;i<starty+sizey;i++)
        changeelement(startx,i,new vwall(*this,startx,i));
    changeelement(startx,starty+sizey,new blcorner(*this,startx,starty+sizey));
    // Make the room and horizontal walls
    for (i=startx+1;i<startx+sizex;i++){
        changeelement(i,starty,new hwall(*this,i,starty));
        for(j=starty+1;j<starty+sizey;j++)
            changeelement(i,j,new room(*this,i,j));
        changeelement(i,starty+sizey,new hwall(*this,i,starty+sizey));
    };
    // Make the top right corner
    changeelement(startx+sizex,starty,new trcorner(*this,startx+sizex,starty));
    // Make the right vertical wall
    for(i=starty+1;i<starty+sizey;i++)
        changeelement(startx+sizex,i,new vwall(*this,startx+sizex,i));
    changeelement(startx+sizex,starty+sizey,new brcorner(*this,startx+sizex,starty+sizey));
}

