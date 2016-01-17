
#include "dctrl1.h"
#include <time.h>
#include <iostream.h>
#include "String.h"

const int EXPIRE_ARRAY = 10;
const int EXPIRE_TIME = 50000;
class Access {

private:
  Dctrl databases;
  bool tokenhere;
  int tokenturn;
  int tokenwho;
  
  clock_t * expires; // Divide by CLOCKS_PER_SEC

public:
  Access() {expires = new clock_t[EXPIRE_ARRAY]; clock(); tokenhere = true;}
  ~Access() {delete [] expires;}

  void initdb(int num)
    {
      databases.initdb(num);
    }
  
  void setupdb(int db, int numr, int numf, char * named, char ** namef, int * ftype)
    {
      databases.setupdb(db, numr, numf, named, namef, ftype);
    }
  
  char * gettoken(int user)
    {
      SString output("NYT",false);
      if (tokenhere)
	{
	  tokenhere = false;
	  tokenwho = user;
	  databases.inctime();
	  tokenturn = databases.gettime();
	  clock_t temp_clock = clock();
	  expires[tokenturn % EXPIRE_ARRAY] = temp_clock + EXPIRE_TIME;
	  return givetoken(user);
	}
      else
	if (tokenwho == user)
	  {
	    SString iytout("IYT 0 0\n",false);
	    
	    return iytout.GetC();
	  }
	return output.GetC();
    }
  
  char * namebind()
    {
      SString output("",false);
      SString map("MAP ");
      int last = databases.numdb();
      
      for(int i = 0; i < last; i++)
	if (databases.set(i, -1, 0, NULL) != -101)   // Check if the database has been initiated, if it has not, it will return error code
	  // when someone attempts to set it 
	  {
	    

	    output += map;
	    SString dbi(i);
	    output += dbi;
	    output += ' ';
	    SString m(databases.getname(i));
	    output += m;
	    output += ' ';
	    int fnum = getfieldnum(i);
	    SString dbfnum(fnum);
	    output += dbfnum;
	    
	    for(int j = 0; j < fnum; j++)
	      {
		output += ' ';
		SString dbj(j);
		output += dbj;
		output += ' ';
	        SString n(databases.getfname(i, j));
		output += n;
		output += ' ';
		SString dbgft(databases.getfieldtype(i, j));
		output += dbgft;
	      }
	    output += '\n';
	    
	  }
      return output.GetC();
    }
  
  void addrec(int db, void ** fields, int numfields)
    {
      databases.Addrec(db, fields, numfields);
    }
  
  char * put(int user, int token, int trans, int db, int rec, int field, void * info)
    {
      if ((tokenwho == user) || (user == -1))
	{  
	  if (0 <= databases.set(db,rec,field, info)) {
	    SString output("AOK ",false);
	    SString tnum(trans);
	    output += tnum;
	    
	    //if (user != -1) 
	    //  expires[tokenturn % EXPIRE_ARRAY] += EXPIRE_TIME/2;
	    return output.GetC();
	  }

	  SString output("ERR ",false);
	  output += trans;
	  SString out2(" Put Command Fails");
	  output += out2;
	  return output.GetC();
	}
	  SString output("NYT",false);
	  return output.GetC();
    }

  char * givetoken(int user)
    {
      tokenhere = false;
      tokenwho = user;
      databases.inctime();
      tokenturn = databases.gettime();
      clock_t temp_clock = clock();
      expires[tokenturn % EXPIRE_ARRAY] = temp_clock + EXPIRE_TIME;

      SString output("IYT ",false);
      SString time(EXPIRE_TIME);
      output += time;
      output += ' ';
      SString num(tokenturn);
      output += num;
      return output.GetC();

    }

  int tokendone(int user)
    {
      if (tokenwho == user)
	{
	  tokenhere = true;
	  tokenwho = -1;
	  return 0;
	}
      return -1;
    } 

  void * get(int db, int rec, int field)
    {
      //cout << "AC: get\n";
      return databases.get(db, rec, field);
    }
  
  int getfieldtype(int db, int field)
    {
      return databases.getfieldtype(db, field);
    }

  int getfieldnum(int db)
  {
    return databases.getfieldnum(db);
  }

  int getrecnum(int db)
  {
    int recs = 0;
    int last = databases.getmaxrec(db);
    for (int i = 0; i < last; i++)
      if (get(db,i,0)) recs++;
    return recs;
  }

  char * getf(int db, int rec, int field)
    {
      int num = 1;
      if (rec == -1) num *= getrecnum(db);
      if (field == -1) num *= getfieldnum(db);
      
      SString output("NRF ",false);
      SString dbtime(databases.gettime());
      output += dbtime;
      output += ' ';
      SString dbnum(num);
      output += dbnum;
      output += '\n';
     

      int lastrec = databases.getmaxrec(db);
      int lastfield = getfieldnum(db);
      SString fld("FLD");
      
      
      if (rec == -1)
	for(int i = 0; i < lastrec; i++)
	  {
	    if (get(db,i,0) == NULL) continue;
	    
	    if (field == -1)
	      for(int j = 0; j < lastfield; j++)  
		{
		  if (get(db,i,j) == NULL) continue;
		  
		  output += fld;
		  output += ' ';
		  SString dbn(db);
		  output += dbn;
		  output += ' ';
		  SString dbi(i);
		  output += dbi;
		  output += ' ';
		  SString dbj(j);
		  output += dbj;
		  output += ' ';		  

		  if (getfieldtype(db, j) == 0) 
		    {
		      SString m((char*)get(db,i,j));
		      output += m;
		    }
		  if (getfieldtype(db, j) == 1) 
		    {
		      SString m(*(int*)get(db,i,j));
		      output += m;
		    }
		  output += '\n';
		}
	    else
	      {
		if (get(db,i,field) == NULL) continue;
		  
		output += fld;
		output += ' ';
		SString dbn(db);
		output += dbn;
		//		output += db;
		output += ' ';
		SString dbi(i);
		output += dbi;
		output += ' ';
		SString dbf(field);
		output += dbf;
		output += ' ';

		if (getfieldtype(db, field) == 0) 
		  {
		    SString m((char*)get(db,i,field));
		    output += m;
		  }
		if (getfieldtype(db, field) == 1) 
		  {
		    SString m(*(int*)get(db,i,field));
		    output += m;
		  }		

		output += '\n';
	      }
	    
	  }
      else
	{
	  if (field == -1)
	    for(int j = 0; j < lastfield; j++)  
	      {
		if (get(db,rec,j) == NULL) continue;
		  
		output += fld;
		output += ' ';
		//	output += db;
		SString dbn(db);
		output += dbn;
		output += ' ';
		SString dbr(rec);
		output += dbr;
		output += ' ';
		SString dbj(j);
		output += dbj;
		output += ' ';

		if (getfieldtype(db, j) == 0) 
		    {
		      SString m((char*)get(db,rec,j));
		      output += m;
		    }
		  if (getfieldtype(db, j) == 1) 
		    {
		      SString m(*(int*)get(db,rec,j));
		      output += m;
		    }

		output += '\n';
	      }
	  else
	    {
	      if (get(db,rec,field) != NULL)
		{
		  output += fld;
		  output += ' ';
		  //output += db;
		  SString dbn(db);
		  output += dbn;
		  output += ' ';
		  SString dbr(rec);
		  output += dbr;
		  output += ' ';
		  SString dbf(field);
		  output += dbf;
		  output += ' ';
		  
		  if (getfieldtype(db, field) == 0) 
		    {
		      SString m((char*)get(db,rec,field));
		      output += m;
		    }
		  if (getfieldtype(db, field) == 1) 
		    {
		      SString m(*(int*)get(db,rec,field));
		      output += m;
		    }

		  output += '\n';
		}
	    } 
	}
      output += '\0';
      return output.GetC();	 
    }


  char * getmod(int db, int time)
    {
      //int num = 1;
      //if (rec == -1) num *= getrecnum(db);
      //if (field == -1) num *= getfieldnum(db);
      
      SString output("NRF ",false);
      SString dbtime(databases.gettime());
      output += dbtime;
      output += ' ';
      //SString dbnum(num);
      //output += dbnum;
      output += '\n';
      

      int lastrec = databases.getmaxrec(db);
      int lastfield = getfieldnum(db);
      SString fld("FLD");
      
      for(int i = 0; i < lastrec; i++)
	{
	  if (get(db,i,0) == NULL) continue;
	  if (databases.getmod(db, i) < time) continue;
	  for(int j = 0; j < lastfield; j++)  
	    {
	      
	      output += fld;
	      output += ' ';
	      SString dbn(db);
	      output += dbn;
	      output += ' ';
	      SString dbi(i);
	      output += dbi;
	      output += ' ';
	      SString dbj(j);
	      output += dbj;
	      output += ' ';		  
	      
	      if (getfieldtype(db, j) == 0) 
		{
		  SString m((char*)get(db,i,j));
		  output += m;
		}
	      if (getfieldtype(db, j) == 1) 
		{
		  SString m(*(int*)get(db,i,j));
		  output += m;
		}
	      output += '\n';
	      
	    }
	}
      output += '\0';
      return output.GetC();	 
    }



};




