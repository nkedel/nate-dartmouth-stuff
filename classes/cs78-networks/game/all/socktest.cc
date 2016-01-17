#include "sockcom.h"
#include "iostream.h"

int main(int argc, char *argv[])
{
  if (argc < 3)
    {
      cout << "Usage: " << argv[0] << " hostname port\n";
      exit(1);
    }
 
  int pnum = atoi(argv[2]);
  
  cout << "Port: " << pnum << endl;
  cout << "Host: " << argv[1] << endl;

  Sock c;
  int retval;

  retval = c.Connect(argv[1], pnum);
  cout << "Return from connect: " << retval << endl;
  
  char message[10] = "GPL\n\0";
  retval = c.Writer(message, 5);
  cout << "Return from writer: " << retval << endl;
  
  char space[100];
  while(1)
    {
      c.Reader(space);
    }
  
  retval = c.Disconnect();
  cout << "Return from disconnect: " << retval << endl;

  exit(1);
}
