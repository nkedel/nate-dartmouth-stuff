void print_local(char *);

class Client {

private:
  Sock c;
  
  char * statusm;
  int turn;
  bool psetup;
  bool mode;        // 0 = init state 1 = gamestate

public:
  Client();
  ~Client();

  int Connect(char * host, int port);
  int Say(char * message);
  int Sendstats(char * name, char * charfile, char * password);

  // Returns number of records that are being returned
  char * get(int db, int rec, int field);

 // Returns number of records that are being returned
  bool put(int db, int rec, int field, char * data);

  int Ready();

  int tokendone();
  void requesttoken();
  void flush();
  
  char * Getstatus();

  bool IYTwait();

  char * parse();
  };
