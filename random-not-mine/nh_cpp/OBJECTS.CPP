#include "objects.h"

objectclass::objectclass() {
        otype=OTYPE_NOTHING;
        cursed=FALSE;
        blessed=FALSE;       
        curseknown=FALSE;       
        nameknown=FALSE;
        wielded=FALSE;
        unique=FALSE;
        wishable=FALSE;
        unpaid=FALSE;
        oinvis=FALSE;
        greased=FALSE;
        color=NO_COLOR;
        packletter='a';
        weight=255;
        quantity=1;
        name=NULL;
        desc=NULL;
        price=0;
        merge=0;
}


objectclass *objectclass::useup(){
    if(--quantity>0) 
        return this;
    else{ 
        delete this; 
        return(NULL);
    }
}


