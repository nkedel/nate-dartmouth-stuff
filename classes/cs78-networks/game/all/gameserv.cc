#include <stdio.h>
#include <iostream.h>
#include "sockserver.h"

int 
main(int argc, char *argv[])
{
  if (argc < 2)
    {
      cout << "Usage: " << argv[0] << " port\n";
      exit(1);
    }

  int pnum = atoi(argv[1]);

  Sockserver a;
  Players * people;

  int num = a.Start(pnum, people);
  cout << "Number of socks: " << num << " DONE!!!";

}
