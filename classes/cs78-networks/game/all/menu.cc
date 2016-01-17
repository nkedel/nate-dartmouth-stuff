#include "gamec.cc"
#include <unistd.h>
#include <ctype.h>
#include <iostream.h>

int main(int argc, char *argv[])
{
  if (argc < 3)
    {
      cout << "Usage: " << argv[0] << " hostname port\n";
      exit(1);
    }
 
  int pnum = atoi(argv[2]);
  cout << "--------------------------------------------------------------\n"; 
  cout << "Hello and welcome to the Networking Database\n";
  cout << "\n";
  cout << "The program is now connecting to: \n";
  cout << "Host: " << argv[1] << endl;
  cout << "Port: " << pnum << endl;
  cout << "--------------------------------------------------------------\n";

  int retval;

  Client cl;

  retval = cl.Connect(argv[1], pnum);
  cout << "--------------------------------------------------------------\n";
  cout << "The Return Code From Connecting: " << retval << endl;
  cout << "0            = is a good thing\n";
  cout << "negative #s  = is not (if so, then the program will end now)\n";

  if (retval < 0)
    exit(1);

  cout << "--------------------------------------------------------------\n";
  cout << "Please type in your name: " << endl;
  
  char * name;
  SString nname;
  name = new char;
    
  read(0,name,1);

  while (*name != '\n')
    {
      nname += *name;
      read(0,name,1);
    }
  cout << "Hello " << nname << "! How are you today?\n";
  cout << "I will now register you as a user of the database.\n";
  cout << "--------------------------------------------------------------\n";

  SString chars("Huh");
  SString pw("whoa");
  
  int i;
  do 
    {
      cout << "0) Quit " << endl;
      cout << "1) Put an item on the database" << endl;
      cout << "2) Get an item from the database" << endl;
      cout << "3) See Status" << endl;
      cout << "4) run IYTwait" << endl;
      cout << "eh?: ";
      //int i;
      cin >> i;
      if (i == 1)
	{
	  int ty;
	  cout << "Enter 0 for int or 1 for string: ";
	  cin >> ty;
	  cout << "Type a record: ";
	  int rec, f, in;
	  cin >> rec;
	  cout << "Type a field (0-2): ";
	  cin >> f;
	  cout << "Type an integer: ";
	  cin >> in;
	  SString st(in);
	  cl.put(0,rec,f+ty*7,st.GetC());
	  cout << "Done!\n";
	} 
      if (i == 2)
	{
	  int ty;
	  cout << "Enter 0 for int or 1 for string: ";
	  cin >> ty;
	  cout << "Type a record: ";
	  int rec, f;
	  // int in;
	  cin >> rec;
	  cout << "Type a field (0-2): ";
	  cin >> f;
	  //cout << "Type an integer: ";
	  //cin >> in;
	  //SString st(in);
	  cout << "Result: " << cl.get(0,rec,f+ty*7);
	  //cout << "Done!\n";
	} 
     if (i == 3)
	{
	  char * p = cl.Getstatus();
	  if (p != NULL)
	    cout << "Result: " << p << endl;
	  else
	    cout << "Nothing New" << endl;
	  //cout << "Done!\n";
	} 
     if (i == 4)
	{
	  cl.IYTwait();
	  cout << "GOT IYT!!!" << endl;
	  //cout << "Done!\n";
	} 

    } while (i != 0);


  cl.put(0,0,7,nname.GetC());
  //cout << cl.Sendstats(nname.GetC(), chars.GetC(), pw.GetC()) << endl;
  //cl.Ready();
  cout << "Getting: " << cl.get(0,0,7) << "]wow!" <<endl;

  char * p;
  char t = 'y';
  while (t != 'n')
    {
      while ((p = cl.parse()) == NULL);
      cout << "message: " << p << endl;
      SString test(p);
      SString mapper("/m"); 
      SString pla("/p");
      /*if (test == mapper)
	for (int i = 0; i < 22; i++)
	  {
	    //char ** t = c1.getmap()
	    cout << (cl.Getmap())[i] << endl;
	  }

     if (test == pla)
	for (int i = 0; i < 2; i++)
	  {
	    Player * t = cl.Getplayers();
	    cout << "Player " << i << ": " << t[i].getname() << " " << t[1].getX() << endl;
	  }*/

      //cout << "continue? : ";
      //cin >> t;
    }
  exit(1);
}
