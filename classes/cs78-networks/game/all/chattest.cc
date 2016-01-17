#include "chats.h"

void main()
{
  cout << "Which port: ";
  int p;
  cin >> p;
  Sockserver a;
  Players * people;
  int num = a.Start(p, people);
  cout << "Number of socks: " << num << " DONE!!!";
}
