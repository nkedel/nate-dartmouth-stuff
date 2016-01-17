/* this is our player class which defines the datatype for a player, and is
   an interface between the implemnetation of the player records on the
   database server and the logical view of the player object by the client. */

  const int dbP = 0;

// setting constant database fields
  const int pfX = 0, pfY = 1, pfHP = 2, pfXP = 3, 
            pfAC = 4, pfDMG = 5, pfkills = 6 , pflive = 7, 
            pfname = 8, pfweapon = 9, pfarmor = 10;


class Player {
private:

  // variables taht define a player, i.e. hit points, experience, etc.
  int x, y;   // position of the player
  int whichrec;
  int old_x, old_y; // previous x and previous y to know whether to move player                    // on screen 
  int hp, xp, ac, dmg, kills;   
  bool live;
  bool token;
  char * name;
  char * weapon;
  char * armor;
            
public:

  Player()  { // initialize player
             token  = FALSE; // tells whether player has turn
             name   = new char[32]; name[0] = 0;
             weapon = new char[32]; weapon[0] = 0;
             armor  = new char[32]; armor[0] = 0;
             live = FALSE;
            }
            
  // destrcutor necessary since we allocate memory
  ~Player() {delete [] name; delete [] weapon; delete [] armor; }

  // Self explanatory functions that do what they are titled
  void settoken(void) { token = TRUE; }
  void cleartoken(void) { token = FALSE; }
  void whoami(int i) { whichrec = i; }
  int oldX(void) { return old_x; }
  int oldY(void) { return old_y; } 
  void set_oldX(int newx) { old_x = newx; }
  void set_oldY(int newy) { old_y = newy; } 
  
  void setX(int newx) { 
    char text[16];
    x = newx; 
    sprintf(text,"%d",x);
    // send it to the database server
    if (put(dbP, whichrec, pfX, text) == FALSE) die("setX failed!");
    }

  void setY(int newy) {
    char text[16];
    y = newy; 
    sprintf(text,"%d",y);
    if (put(dbP, whichrec, pfY, text)  == FALSE) die("setY failed!");
  }
    
  int getX(void) { 
    if (token) return x;
    char *text;
    // Access database to get player's x position
    text = get(dbP, whichrec, pfX); 
    if (text == NULL) die ("getX failed!");
    x = atoi(text);
    delete [] text;
    return x; 
    }
    
int getY(void) { 
    if (token) return y;
    char *text;
/* Access database to get player's x position */
    text = get(dbP, whichrec, pfY);
    if (text == NULL) die ("getY failed!");
    y = atoi(text);
    delete [] text;    
    return y; 
    }

  void setHP(int newHP)   {
    char text[16];
    hp = newHP; 
    sprintf(text,"%d",hp);
    if (put(dbP, whichrec, pfHP, text)== FALSE) die("setHP failed!");
    }

  int  getHP(void)        {
    char *text;
    text = get(dbP, whichrec, pfHP);
    if (text == NULL) die ("gethp failed!");
    hp = atoi(text);
    delete [] text;
    return hp; 
    }

  void setXP(int newXP)   {
    char text[16];
    xp = newXP; 
    sprintf(text,"%d",xp);
    if (put(dbP, whichrec, pfXP, text) == FALSE) die("setXP failed!");
    }
    
  int  getXP(void)        {
    char *text;
    text = get(dbP, whichrec, pfXP);
    if (text == NULL) die ("getxp failed!");
    xp = atoi(text);
    delete [] text;
    return xp; 
    }

  void setAC(int newAC)   { 
    char text[16];
    ac = newAC; 
    sprintf(text,"%d",ac);
    if (put(dbP, whichrec, pfAC, text)== FALSE) die ("setAC failed!");
    }
    
  int  getAC(void)        {
    char *text;
    text = get(dbP, whichrec, pfAC);
    if (text == NULL) die ("getAC failed!");
    ac = atoi(text);
    delete [] text;
    return ac; 
    }

  void setDMG(int newDMG) { 
    char text[16];
    dmg = newDMG; 
    sprintf(text,"%d",dmg);
    if (put(dbP, whichrec, pfDMG, text) == FALSE) die("setDMG Failed");
    }

  int  getDMG(void)       {
    char *text;
    text = get(dbP, whichrec, pfDMG);
    if (text == NULL) die ("getDMG failed!");
    dmg = atoi(text);
    delete [] text;
    return dmg; 
    }

  void setkills(int newkills) {
    char text[16];
    kills = newkills; 
    sprintf(text,"%d",kills);
    if (put(dbP, whichrec, pfkills, text) == FALSE) die("setkills Failed");
    }

  int  getkills(void)    {
    char *text;
    text = get(dbP, whichrec, pfkills);
    if (text == NULL) die ("getkills failed!");
    kills = atoi(text);
    delete [] text;
    return kills; 
    }

  void setlive(bool alive) {
    live = alive;
    if (alive) {
      if (put(dbP, whichrec, pflive, "1") == FALSE) die("setkills Failed");
      }
    else 
      if (put(dbP, whichrec, pflive, "0") == FALSE) die("setkills Failed");
    }

  bool getlive(void)       {
    char *text;
    text = get(dbP, whichrec, pflive);
    if (text == NULL) die ("getlive failed!");
    if (atoi(text) != 0) live=TRUE; else live=FALSE;
    delete [] text;
    return live; 
    }


  char* getarmor(void) {
    char *text;
    text = get(dbP, whichrec, pfarmor);
    if (text == NULL) die ("getarmor failed!");
    strcpy(armor, text);
    delete [] text;
    return armor; 
    }

  char* getweapon(void)  {
    char *text;
    text = get(dbP, whichrec, pfweapon);
    if (text == NULL) die ("get weapon failed!");
    strcpy(weapon, text);
    delete [] text;
    return weapon; 
    }

  char* getname(void)    {
    char *text;
    text = get(dbP, whichrec, pfname);
    if (text == NULL) die ("getname failed!");
    strcpy(name, text);
    delete [] text;
    return name; 
    }

  void setname(const char *newname)     {
    if (newname == NULL) return;
    strcpy(name, newname); 
    if (put(dbP, whichrec, pfname, name) == FALSE) die("setname Failed");
    }         

  void setarmor(const char *newarmor)   {
    if (newarmor == NULL) return;
    strcpy(armor, newarmor); 
    if (put(dbP, whichrec, pfarmor, armor) == FALSE) die("setarmor Failed");
    }

  void setweapon(const char *newweapon) {
    if (newweapon == NULL) return;
    strcpy(weapon, newweapon); 
    if (put(dbP, whichrec, pfweapon, weapon) == FALSE) die("setweapon Failed");
    }
 
  void do_move(int x, int y);
  void resolve_move(int dx, int dy);
  void attack_player(int i);
  void attack_monster(int i);
  };

