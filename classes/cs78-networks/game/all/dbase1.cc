#include "dbase1.h"
#include <stdlib.h>

Dbase::Dbase()
{
  active = false;
}

Dbase::Dbase(int maxrec, int fperrec)
{
  Init(maxrec, fperrec);
}

Dbase::~Dbase()
{
  // haha!
}


// Initialize a database
int
Dbase::Init(int maxrec, int fperrec)
{
  // If the database has already been initialized, destroy it
  if ((maxrec < 1) || (fperrec < 1))
    return -1;

  if (active)
    Destroy();
  
  // Set the vital stats of the matrix
  maxrecords = maxrec;
  fieldperrec = fperrec;

  // Set it as being on
  active = true;
  
  // Create the records
  matrix = new void ** [maxrec];
  
  // Create the fields
  for (int i = 0; i < maxrec; i++)
    matrix[i] = new void * [fperrec];

  // Create the lastmod and valid (they'll be set to -1 and false later)
  lastmod = new int[maxrec];
  valid = new bool[maxrec];

  // Create the fieldnames and set them to NULL
  fieldname = new char*[fieldperrec];
  for(int i = 0; i < fieldperrec; i++)
    fieldname[i] = NULL;

  // Set the fieldtypes to -1 as default
  fieldtype = new int[fieldperrec];
  for(int i = 0; i < fieldperrec; i++)
    fieldtype[i] = -1;

  // Set other things, including having the matrix entries set to NULL
  for(int i = 0; i < maxrec; i++)
    {
      lastmod[i] = -1;
      valid[i] = false;
       for(int j = 0; j < fperrec; j++)
	 matrix[i][j] = NULL;
    }
  return 0;
}

// Delete an record in the matrix
int
Dbase::Del(int rec)
{
  // If the record exists, then delete it, if not, our work here is done
  if ((rec >= maxrecords) || (rec < 0)) // Is requested record invalid
    return -1;

  if (valid[rec])
    for (int j = 0; j < fieldperrec; j++)
      {
	// Parse fields, delete depending on field type
	if (fieldtype[j] == 0) delete [] matrix[rec][j]; // Array (ie - char *)
	if (fieldtype[j] == 1) delete matrix[rec][j];    // Normal (ie - int) 
      }
  valid[rec] = false; // Set record to false
  return 0;
}

// Destroy an database
void
Dbase::Destroy()
{
  // If it was never created, then our work here is done
  if (!active) 
    return;

  // Parse records and delete them
  for (int i = 0; i < maxrecords; i++)
    Del(i);

  // Delete the fields
  for (int i = 0; i < maxrecords; i++)
    delete [] matrix[i];

  // Delete the records
  delete [] matrix;

  // Delete reocrd markers
  delete [] lastmod;
  delete [] valid;

  // Delete fieldnames as necesary
  for (int i = 0; i < fieldperrec; i++)
    if (fieldname[i]) delete [] fieldname[i];
  delete [] fieldname;

  delete [] fieldtype;

  // Dbase is now off
  active = false;
}
