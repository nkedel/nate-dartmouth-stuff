#include "dungeon.h"

dungeon::dungeon(){

}


void dungeon::setcurrentlev(level *curr){
    current=curr;
}

/*
void dungeon::movemonsters(){
    int i,j;
    uchar dir;
    schar dx,dy;    // monster direction
    uchar mx,my;    // monster destination

    for(i=0;i<MAXCOLS;i++)
        for(j=0;j<MAXROWS;j++)
            if((current.monst[i][j])&&(!current.monst[i][j]->ishero())){
                // update where they think you are.
                findhero(i,j);
                // if hostile move in that direction.
                dx=ux-i;
                dy=uy-j;
                // this gives us direction, but monsters can only move
                // one square at a time.
                mx=i;
                my=j;
                if(dx<0) mx--;
                if(dx>0) mx++;
                if(dy<0) my--;
                if(dy>0) my++;
                if((mx==ux)&&(my==uy)){
                    // attack hero.

                } else 
                    if(current->canenter(mx,my)){
                        movemonfromto(i,j,mx,my);
                        setmoved(curent->monst[mx][my]);
                    }
                // else move in random direction.
                // not implemented
                }
            }
        
}
*/

/*
void dungeon::findhero(uchar x,uchar y){

    // assume monster always knows heros location
    current->monst[x][y]->sethero(ux,uy);
}
*/
