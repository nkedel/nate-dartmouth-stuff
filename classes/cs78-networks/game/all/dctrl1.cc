#include "dctrl1.h"



Dctrl::~Dctrl()
{
  // haha!
}

/*
int 
Dctrl::Addrec(int db, void ** fields, int numfields)
{
  assert((db >= 0) && (db < maxdb)); 
  if (getfieldnum(db) != numfields)
    return -1;

  int empty = 0;
  int last = getmaxrec(db);
  for(; (!data[db].getvalid(empty)) && (empty < last); empty++);

  data[db].setvalid(empty,true);
  
  for(int i = 0; i < numfields; i++)
    data[db].set(empty,i,fields[i]);
  data[db].setmod(empty, time);

  return 0;
}

int
Dctrl::Setrec(int db, int rec, void ** fields, int numfields)
{
  assert((db >= 0) && (db < maxdb)); 
  if (getfieldnum(db) != numfields)
    return -1;

  data[db].Del(rec);
  data[db].setvalid(rec,true);
  
  for(int i = 0; i < numfields; i++)
    data[db].set(rec,i,fields[i]);
  data[db].setmod(rec, time);

  return 0;
}
*/
