/*************************************************
file: sockcom.h
by Dave Marmaros
5/23/98
CS 78

A c++ socket class that encapsulates all 
socket features
*************************************************/


#include "sockaddr.h"
#include <iostream.h>
#include "String.h"
#include "msleep.h"
#include <ctype.h>




class Sock
{
public:
  Sock();
  Sock(char *, int); 
  ~Sock();

  int Preconnect(int, int maxl = 1);
  int Check();

  int Connect(char *, int);
  int Disconnect();

  char * Reader();
  int Writer(char *, int);

  bool Open() 
    {return valid;}

private:
  int wait;
  int socknum;
  bool valid;
  struct sockaddr_in sa;
  SString input;
};
