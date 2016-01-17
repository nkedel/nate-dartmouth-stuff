#include "global.h"
#include "monst.h"

eventitem *eventlist::find(uchar type, monstclass *m){
    eventitem *tmp;

    tmp=head;
    while(tmp!=NULL){
        if((tmp->item()==type)&&(m==tmp->mon))
            return(tmp);
        else
            tmp=tmp->next;
    }
    return(NULL);
}

void eventlist::removeitem(eventitem *item){

    if(item->next)
        item->next->prev=item->prev;
    if(item->prev)
        item->prev->next=item->next;
    if(head==item)
        head=item->next;
    item->remove();
    delete item;
}

void eventlist::addevent(eventitem *d){
    eventitem *tmp;

//  need something..
//    assert(d!=NULL);
    
    // make it an absolute time, not an interval
    d->time+=currtime;

    tmp=find(d->item(),d->mon);
    // if it exists already, decide which is bigger.
    if(tmp)
        if(tmp->time<d->time)
            // if the existing time is smaller, remove it and add new like
            // normal.
            removeitem(tmp);
        else{
            // otherwise new one is useless. delete it and return.
            delete d;
            return;
        }
    // Add new item in chronological order.
    if(!head)
        // Nothing in the list. Add it first (trivial case).
        head=d;
    // list has 1 or more items.
    tmp=head;
    while((tmp->next!=NULL)&&(tmp->time<d->time))
        tmp=tmp->next;
    // tmp is either the last item or on the item to insert in front of
    
    if(!tmp->next){
        // last item
        // does it go before or after?
        if(tmp->time<d->time){
            // goes after
            tmp->next=d;
            d->prev=tmp;
        }
    }
    else {
        // tmp is on item to place in front of.
        d->next=tmp;
        d->prev=tmp->prev;
        tmp->prev=d;
        // prev is not gauranteed
        if(d->prev)
            d->prev->next=d;
    } 

}                                                               

void eventlist::doevent(){

    if((head)&&(currtime>=head->time)){
        head->doevent();
        removeitem(head);
        eventlist::doevent();   // may be two events at the same time or
                                // need to do 'catch-up' for some reason.
                                // hopefully won't recurse TOO much.
    }
}

// Removes an event of a specific type for a specific monster. Only one
// event should exist.

void eventlist::removeevent(monstclass *mon,uchar type){
    eventitem *tmp1,*tmp;

    tmp=head;
    while(tmp!=NULL){
        if((tmp->mon==mon)&&(tmp->item()==type)){
            removeitem(tmp);
            return;
        } else
            tmp=tmp->next;
    }
}

void eventlist::removeeventformon(monstclass *m){
    eventitem *tmp,*tmp1;

    tmp=head;
    while(tmp!=NULL){   
        if(tmp->mon==m){
            tmp1=tmp;
            tmp=tmp->next;
            removeitem(tmp1);
        } else
            tmp=tmp->next;
    }
}


uint eventlist::operator++(int){
    currtime++;
    // put all the things that should be done EACH TURN in here.
    doevent();
    // update halucination
    // movemonsters
    // generate new monsters
    // display messages
    // heal hero
    // use food
    // get rid of old corpses


    return currtime;
}

// The list handles removing of used events. Event items  should 
// do the event only.

void unconfused::doevent(){mon->makeunconfused();}

void unhalu::doevent(){mon->makeunhalu();}

void unblind::doevent(){mon->makeunblind();}

void unblinditem::doevent(){mon->makeunblind();}

void unlevitate::doevent(){if(!timer.eventexists(UNLEVITATEITEM,mon)) mon->makeunlevitate();}

void unlevitateitem::remove(){if(!timer.eventexists(UNLEVITATE,mon)) mon->makeunlevitate();}

void unwound::doevent(){mon->unwoundlegs();}

void teleport::doevent(){}

void polymorph::doevent(){}

void awake::doevent(){mon->wake();}

void sleep::doevent(){mon->makesleep();}

void unstun::doevent(){mon->makeunstunned();}

void unparalyzed::doevent(){mon->makeunparalyzed();}
