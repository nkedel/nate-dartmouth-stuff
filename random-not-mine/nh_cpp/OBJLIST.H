#ifndef OBJLIST_H
#define OBJLIST_H
#include "objects.h"

// Object list node

class objectnode{

    objectnode *next;
    objectnode *prev;
    objectclass *obj;

public:
    objectnode(objectclass *newobj){obj=newobj;next=prev=NULL;}
    objectclass *getobj(){return obj;}
    void setnext(objectnode *tmp){next=tmp;}
    void setprev(objectnode *tmp){prev=tmp;}
    objectnode *getnext(){return next;}
    objectnode *getprev(){return prev;}
    void draw(){ obj->draw();}
};

// manages the object list

class objectlist{

    objectnode *head;
    objectnode *tail;
    objectnode *current;
    uint numobjs;

public:
    objectlist(){head=tail=current=NULL;}
    ~objectlist();      // deletes only the list. does not delete the items.
    void free();             // removes all the items from the list and deletes them.
    void add(objectclass * object);
    objectclass *get();
    int findbyclass(uchar classtype);
    void next(){current=current->getnext();}
    void prev(){current=current->getprev();}
    void reset(){current=head;}
    int getnumitems(){return numobjs;}
    void draw(){head->draw();}
    objectclass *remove();
};

// This class is for the heros pack. It enforces a size limit and direct
// access via array referencing.  It takes into account the packletter member
// in objectclass and tries to preserve its place.

class objpack{
    const int maxpack=52;
    char nextitemtouse;
    objectclass *list[maxpack];
    inline uchar lettonum(char let){return let<='Z'?let-'A'+26:let-'a';}
    inline char numtolet(uchar num){return num<=25?'a'+num:'A'+num-26;}
    void nextlettertouse();
public:
    char nextletter(){return nextitemtouse;}
    objpack();
    ~objpack();
    void add(objectclass *obj);                 // try to preserve pack lettes
    objectclass *operator[](char let);          // modify to allow a-z A-Z
    objectclass *remove(char let);
    objectlist *getlistofclass(uchar cls);
};

// This class is so monsters keep the same pack restirictions as the hero,
// but ignore the packletter member in objects.

class monpack{
    const int maxpack=52;
    objectclass *list[maxpack];
    inline uchar lettonum(char let){return let<='Z'?let-'A'+26:let-'a';}
    inline char numtolet(uchar num){return num<=25?'a'+num:'A'+num-26;}
public:
    monpack();
    void add(objectclass *obj);                 // try to preserve pack lettes
    objectclass *operator[](char let);          // modify to allow a-z A-Z
    objectclass *remove(char let);             
    uchar canget();                             // monsters won't pick up 
                                                // something if they have no
                                                // room for it.
};
#endif
