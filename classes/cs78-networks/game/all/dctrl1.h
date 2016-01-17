#include "dbase1.h"
#include <assert.h>
#include <iostream.h>

#ifndef doh
#define doh

class Dctrl {

private:
  Dbase * data;
  int time;
  int maxdb;

public:
  Dctrl() {time = 0; maxdb = 0;}
  ~Dctrl();

  void  initdb(int num)
    {data = new Dbase[num]; maxdb = num;}

  int numdb() {return maxdb;}

  char * getname(int db)
  {
    return data[db].getname();
  }
  
  char * getfname(int db, int f)
  {
    return data[db].getfname(f);
  }
  void setupdb(int db, int numr, int numf, char * named, char ** namef, int * ftype)
    {
      assert((db >= 0) && (db < maxdb));
      
      data[db].Init(numr, numf);
      
      data[db].setname(named);
      
      for(int i = 0; i < numf; i++)
	{
	  
	  data[db].setfname(i, namef[i]);
	  
	  data[db].setfieldtype(i, ftype[i]);
	}
      
    }

  void settime(int newtime) {time = newtime;}
  int  gettime() {return time;}
  void inctime() {time = time++ % 2000000000;}

  int  Addrec(int db, void ** fields, int numfields)
    {
      
      
      assert((db >= 0) && (db < maxdb)); 
      if (getfieldnum(db) != numfields)
	return -1;
      
      int empty = 0;
      int last = getmaxrec(db);
      
      for(; (data[db].getvalid(empty)) && (empty < (last-1)); empty++);
      
      
      
      data[db].setvalid(empty,true);
  
      

      for(int i = 0; i < numfields; i++)
	{
	  data[db].set(empty,i,fields[i]);
	  
	}

      
      data[db].setmod(empty, time);
      
      return 0;
    }

  int  getfieldnum(int dbasenum) {return data[dbasenum].getfieldperrec();}
  int  getmaxrec(int dbasenum)   {return data[dbasenum].getmaxrec();}
  int  Setrec(int db, int rec, void ** fields, int numfields)
    {
      //assert((db >= 0) && (db < maxdb)); 
      if (getfieldnum(db) != numfields)
	return -1;
      
      data[db].Del(rec);
      data[db].setvalid(rec,true);
      
      for(int i = 0; i < numfields; i++)
	data[db].set(rec,i,fields[i]);
      data[db].setmod(rec, time);

      return 0;
    }

  void Delrec(int db, int rec) 
    {
      assert((db >= 0) && (db < maxdb)); 
      data[db].Del(rec);
      data[db].setmod(rec, time);
    }

  int set(int db, int rec, int field, void * info)
    {
      //assert((db >= 0) && (db < maxdb)); 
      int i = data[db].set(rec, field, info);
      data[db].setmod(rec, time);
      return i;
    }  
  
  void * get(int db, int rec, int field)
    {
      
      return data[db].get(rec, field);
    }

  int getfieldtype(int db, int field)
    {
      return data[db].getfieldtype(field);
    }
 
  int getmod(int db, int rec)
    {
      return data[db].getmod(rec);
    }

  };

#endif
