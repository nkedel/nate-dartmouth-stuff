#include "sockaddr.h"
#include <iostream.h>
#include "String.h"
#include "list.h"
#include <ctype.h>

const int MAX_LISTEN = 8;

class Players
{
public:
  int socknum;
  char * name;
  char * charfile;
  char * password;
};


class Sockserver
{
public:
  Sockserver();
  Sockserver(int maxlisten);
  ~Sockserver();  

  int Start(int port, Players *);

private:

  List clients;
  int sock;
  Players * people;
  int numlisten;
  int listenport;

  void initializeServer(int port);
  void runServer();
  // Announces a new user
  void announce(SString n);

  // Shuts down the server safely
  int shutdownServer();

  // handles incoming strings
  bool parse();
  // Disconnects a user
  void drop_user();

};
