#include "objlist.h"

objectlist::~objectlist(){
    
    objectclass *otmp;
    int i;
    this->reset();
    for(i=0;i<this->getnumitems();i++){
        otmp=this->remove();
    }
    
}

void objectlist::free(){
    int i;
    objectclass *otmp;
    
    reset();
    for(i=0;i<numobjs;i++){
        otmp=remove();
        delete otmp;
    }
}

void objectlist::add(objectclass * object){

    objectnode *tmp;
    tmp=new objectnode(object);
    if(tail){
        tail->setnext(tmp);
        tmp->setprev(tail);
    }
    tail=tmp;
    if(!head) head=current=tmp; 
    numobjs++;
}

int objectlist::findbyclass(uchar classtype){

    int done=0;
    objectclass *tmp;

    while( !done && current!= NULL ){
        tmp=current->getobj();
        if(tmp->getclass()==classtype) 
            done=1;
        else
            next();
    }
    return(done);
}

objectclass *objectlist::remove(){

    objectclass *otmp;
    objectnode *onext,*oprev;
    otmp=get();
    if(otmp){
        onext=current->getnext();
        oprev=current->getprev();
        if(onext)
            onext->setprev(oprev);
        else
            tail=oprev;
        if(oprev)
            oprev->setnext(onext);
        else
            head=onext;
        numobjs--;
        delete current;
        current=onext;
        return(otmp);
    }
    else
        return(NULL);
}

objectclass *objectlist::get(){
    if(current)
        return(current->getobj());
    else
        return(NULL);
}               

objpack::objpack(){
    nextitemtouse='a';
    int i;
    for(i=0;i<maxpack;i++)
        list[i]=NULL;
}

objpack::~objpack(){
    int i;
    for(i=0;i<maxpack;i++)
        delete list[i];
}

void objpack::add(objectclass *obj){
    uchar i=0;
    
    // see if last letter is used.
    if(list[lettonum(obj->getletter())]){
        //make sure nextitemtouse it valid
        if(nextitemtouse){
            list[lettonum(nextitemtouse)]=obj;
            obj->setletter(nextitemtouse);
            nextlettertouse();
        } else
            message("You cannot hold any more!");
    } else{
        // put it in old letter.
        list[lettonum(obj->getletter())]=obj;
        nextlettertouse();
    }
}

objectclass *objpack::operator[](char let){
    return list[lettonum(let)];
}

objectclass *objpack::remove(char let){
    objectclass *otmp;
    uchar index;
    index=lettonum(let);
    otmp=list[index];
    list[index]=NULL;
    return otmp;
}

void objpack::nextlettertouse(){
    // find next valid point in array to put next object.
    int i,j;
    j=lettonum(nextitemtouse);
    i=j;
    while((i<=maxpack)&&(list[i++]));
    // i either points to next one after empty one, or maxpack+1
    if(i<=maxpack)
        nextitemtouse=numtolet(--i);
    else{
        i=0;
        while((list[i++])&&(i<j));
        if(i<j)
            nextitemtouse=numtolet(--i);
        else
            nextitemtouse=0;
    }
}

// Returns an allocated objectlist and put all objects of a type in it.
// objectlist may be empty.

objectlist *objpack::getlistofclass(uchar cls){
    objectlist *tmp;
    int i=0;

    tmp=new objectlist;
    for(i=0;i<maxpack;i++)
        if((list[i])&&(list[i]->getclass()==cls))
            tmp->add(list[i]);
    return tmp;
}

objectclass *monpack::operator[](char let){
    return list[lettonum(let)];
}

objectclass *monpack::remove(char let){
    objectclass *otmp;
    uchar index;
    index=lettonum(let);
    otmp=list[index];
    list[index]=NULL;
    return otmp;
}

void monpack::add(objectclass *obj){
    int i=0;
    // find the next empty place    

    while((i<=maxpack)&&(list[i++]));
    if(i<=maxpack)
        list[--i]=obj;
}

uchar monpack::canget(){
    int i=0;

    while((i<=maxpack)&&(list[i++]));
    if(i<=maxpack)
        return(1);
    else
        return(0);
}

monpack::monpack(){
    int i;
    for(i=0;i<maxpack;i++)
        list[i]=NULL;
}

