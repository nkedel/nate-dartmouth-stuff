#include <string.h>
#include <iostream.h>
#include <stdio.h>
#include "ac.cc"
#include "String.h"

int main()
{
  Access ac;

  char dbname[32] = "TestDB\0";
  char ** namef;
  
  cout << "Hello!" << endl;

  namef = new char*[3];

  SString s1("Field 1");
  SString s2("Field 2");
  SString s3("Field 3");

  namef[0] = s1.GetC();
  namef[1] = s2.GetC();
  namef[2] = s3.GetC();
    /*
  for (int i = 0; i < 3; i++)
    {
      namef[i] = new char[32];
      sprintf(namef[i], "Field %d", i);
    }
    */

  int ftype[3] = {1,0,1};
  
  cout << "about to initdb\n";
  ac.initdb(1);
  cout << "about to setupdb\n";
  ac.setupdb(0, 10, 3, dbname, namef, ftype);
  cout << "about to gettoken\n";
  ac.gettoken(1);
  cout << "got token\n";

  char f1[16] = "Flanders\0";
  int f2 = 42;
  char f3[16] = "dead\0";

  cout << "fun\n";

  void ** fields;

  fields = new void*[3];
  fields[0] = f1;
  fields[1] = &f2;
  cout << "field 1 " << &f2 << endl;
  fields[2] = f3;
  
  cout << "about to addrec\n";
  ac.addrec(0, fields, 3);

  cout << "about to get\n";
  void * result = ac.get(0, 0, 2);

  printf("result %s\n",((char *)result));

  cout << "result = " << *((char *)result) << endl;
  
}
  
