#include <iostream.h>

class Dbase {

private:
  void *** matrix;   // The matrix, and what a scary sight it is too
  int * lastmod;     // An array, one entry for each matrix record, show last
		     // modification to the record, default is 0
  bool * valid;      // A bool, one for each record, says if there is anything
                     // in the record
  int * fieldtype;   // What type of field is it?
		     // Used mainly for deleting purposes
                     // 0 = char *, 1 = int *, the rest are undefined 
  char ** fieldname; // One for each field, just a char * name
  bool active;       // Is the array initialized or not?
  int maxrecords;    // What is the max number of records that the array
		     // can hold? (maybe changed to double if full later
  int fieldperrec;   // Number of fields per record
  char * name;       // The name of the Dbase


public:
  Dbase();
  Dbase(int maxrec, int fperrec);
  ~Dbase();

  int Init(int maxrec, int fperrec);
  void Destroy();

  // Get an entry from the matrix
  void * get(int rec, int field) 
    {
      
      if (!active)
	return NULL;
      if ((rec >= maxrecords) || (rec < 0) || 
	  (field > fieldperrec) || (field < 0)) return NULL;
      if (!valid[rec]) return NULL; 
      
      //cout << "vol: " << /*(int)*/&matrix[rec][field] << endl;
      //cout << "vox: " <<matrix[rec][field] << endl;
      return matrix[rec][field];
    }

  // Set an entry in the matrix
  int set(int rec, int field, void * data) 
    {
      if (!active)
	return -101;
      if ((rec >= maxrecords) || (rec < 0) || 
	  (field > fieldperrec) || (field < 0)) return -126;
      matrix[rec][field] = data;
      valid[rec] = true;
      //setmod(rec,time);
      return 0;
    }

  // Set the name of the Dbase
  int setname(char * newname)
    { 
      if (!active)
	return -102;
      name = new char[strlen(newname)];
      strcpy(name, newname);
      return 0;
    }

  // Set a field name
  int setfname(int field, char * newname)
  {
    if (!active)
      return -103;
    if ((field > fieldperrec) || (field < 0)) return -125;
    fieldname[field] = new char[strlen(newname)];
    strcpy(fieldname[field], newname);
    return 0;
  }

  // Get a field name
  char * getfname(int f) 
  {
    if (!active) return NULL; 
    if ((f > fieldperrec) || (f < 0)) return NULL;
    return fieldname[f];
  }

  // Delete a record
  int Del(int rec);

  // Get the name of the dbase
  char * getname() {if (!active) return NULL; return name;}

  // Set if a field is valid or not
  int setvalid(int rec, bool val) 
  {      
    if (!active)
      return -104;
    if ((rec >= maxrecords) || (rec < 0)) // Is requested record invalid
      return -124;
    valid[rec] = val;
    return 0;
  }
    
  int getvalid(int rec) 
  {
    if (!active)
      return -105;
    if ((rec >= maxrecords) || (rec < 0)) // Is requested record invalid
      return -123;
    return valid[rec];
  } 

  // Get and set the modification time.
  int setmod(int rec, int mod) 
  {
    if (!active)
      return -106;
    lastmod[rec] = mod;
    return 0;
  }

  int getmod(int rec) 
  {
    if (!active)
      return -107;
    if ((rec >= maxrecords) || (rec < 0)) // Is requested record invalid
      return -122;
    return lastmod[rec];
  }  
 
  // Get database info
  int getfieldperrec() 
  {
    if (!active)
      return -108;
    return fieldperrec;
  }
  int getmaxrec() {
    if (!active)
      return -109;
    return maxrecords;
  }

  // Get and set the field type
  int getfieldtype(int num) 
  {
    if (!active)
      return -110;
    if ((num >= fieldperrec) || (num < 0)) // Is requested field invalid
      return -121;
    return fieldtype[num];
  }
  int setfieldtype(int num, int ntype) 
  {
    if (!active)       // Does Database exist
      return -111;     
    if ((num >= fieldperrec) || (num < 0)) // Is requested field invalid
      return -120;
    fieldtype[num] = ntype;
    return 0;
  }
  };





