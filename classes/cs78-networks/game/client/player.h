/* this is our player class which defines the datatype for a player, and is
   an interface between the implemnetation of the player records on the
   database server and the logical view of the player object by the client. */

  const int dbP = 0;

// setting constant database fields
  const int pfX = 0, pfY = 1, pfHP = 2, pfXP = 3, 
            pfAC = 4, pfDMG = 5, pfkills = 6 ,
            pfname = 7, pfweapon = 8, pfarmor = 9, 
            pflive = 10;


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
    if (net.put(dbP, whichrec, pfX, text) == FALSE) { 
      printf("setX failed in instance %d",whichrec); 
      assert(FALSE);
      }
    }

  void setY(int newy) {
    char text[16];
    y = newy; 
    sprintf(text,"%d",y);
    if (net.put(dbP, whichrec, pfY, text)  == FALSE) { 
      printf("setY failed in instance %d\n",whichrec);
      assert(FALSE);
      }
  }
    
  int getX(void) { 
    if (token) return x;
    char *text;
    // Access database to get player's x position
    text = net.get(dbP, whichrec, pfX); 
    if (text == NULL) { 
      printf("getX failed in instance %d\n",whichrec);
      assert(FALSE);
      }
    x = atoi(text);
    delete [] text;
    return x; 
    }
    
int getY(void) { 
    if (token) return y;
    char *text;
/* Access database to get player's x position */
    text = net.get(dbP, whichrec, pfY);
    if (text == NULL) { 
      printf("getY failed in instance %d\n",whichrec);
      assert(FALSE);
      }
    y = atoi(text);
    delete [] text;    
    return y; 
    }

  void setHP(int newHP)   {
    char text[16];
    hp = newHP; 
    sprintf(text,"%d",hp);
    if (net.put(dbP, whichrec, pfHP, text)== FALSE) { 
      printf("setHP failed in instance %d\n",whichrec);
      assert(FALSE);
      }
    }

  int  getHP(void)        {
    char *text;
    text = net.get(dbP, whichrec, pfHP);
    if (text == NULL) { 
      printf("gethp failed in instance %d\n",whichrec);
      assert(FALSE);
      }
    hp = atoi(text);
    delete [] text;
    return hp; 
    }

  void setXP(int newXP)   {
    char text[16];
    xp = newXP; 
    sprintf(text,"%d",xp);
    if (net.put(dbP, whichrec, pfXP, text) == FALSE) { 
      printf("setXP failed in instance %d\n",whichrec);
      assert(FALSE);
      }
    }
    
  int  getXP(void)        {
    char *text;
    text = net.get(dbP, whichrec, pfXP);
    if (text == NULL) { 
      printf("getxp failed in instance %d\n",whichrec);
      assert(FALSE);
      }
    xp = atoi(text);
    delete [] text;
    return xp; 
    }

  void setAC(int newAC)   { 
    char text[16];
    ac = newAC; 
    sprintf(text,"%d",ac);
    if (net.put(dbP, whichrec, pfAC, text)== FALSE) { 
      printf("setAC failed in instance %d\n",whichrec);
      assert(FALSE);
      }
    }
    
  int  getAC(void)        {
    char *text;
    text = net.get(dbP, whichrec, pfAC);
    if (text == NULL) { 
      printf("getAC failed in instance %d\n",whichrec);
      assert(FALSE);
      }
    ac = atoi(text);
    delete [] text;
    return ac; 
    }

  void setDMG(int newDMG) { 
    char text[16];
    dmg = newDMG; 
    sprintf(text,"%d",dmg);
    if (net.put(dbP, whichrec, pfDMG, text) == FALSE) {
      printf("setDMG failed in instance %d\n",whichrec);
      assert(FALSE);
      }
    }

  int  getDMG(void)       {
    char *text;
    text = net.get(dbP, whichrec, pfDMG);
    if (text == NULL) { 
      printf("getDMG failed in instance %d\n",whichrec);
      assert(FALSE);
      }
    dmg = atoi(text);
    delete [] text;
    return dmg; 
    }

  void setkills(int newkills) {
    char text[16];
    kills = newkills; 
    sprintf(text,"%d",kills);
    if (net.put(dbP, whichrec, pfkills, text) == FALSE) { 
      printf("setkills failed in instance %d\n",whichrec);
      assert(FALSE);
      }
    }

  int  getkills(void)    {
    char *text;
    text = net.get(dbP, whichrec, pfkills);
    if (text == NULL) { 
      printf("getkills failed in instance %d\n",whichrec);
      assert(FALSE);
      }
    kills = atoi(text);
    delete [] text;
    return kills; 
    }

  void setlive(bool alive) {
    live = alive;
    if (alive) {
      if (net.put(dbP, whichrec, pflive, "1") == FALSE) { 
        printf("setlive failed in instance %d\n",whichrec);
        assert(FALSE);
        }
      }
    else 
      if (net.put(dbP, whichrec, pflive, "0") == FALSE) { 
        printf("setlive failed in instance %d\n",whichrec);
        assert(FALSE);
        }
    }

  bool getlive(void)       {
    char *text;
    text = net.get(dbP, whichrec, pflive);
    if (text == NULL) { 
      printf("getlive failed in instance %d\n",whichrec);
      assert(FALSE);
      }
    if (atoi(text) != 0) live=TRUE; else live=FALSE;
    delete [] text;
    return live; 
    }


  char* getarmor(void) {
    char *text;
    text = net.get(dbP, whichrec, pfarmor);
    if (text == NULL) { 
      printf("getarmor failed in instance %d\n",whichrec);
      assert(FALSE);
      }
    strcpy(armor, text);
    delete [] text;
    return armor; 
    }

  char* getweapon(void)  {
    char *text;
    text = net.get(dbP, whichrec, pfweapon);
    if (text == NULL) { 
      printf("get weapon failed in instance %d\n",whichrec);
      assert(FALSE);
      }
    strcpy(weapon, text);
    delete [] text;
    return weapon; 
    }

  char* getname(void)    {
    char *text;
    text = net.get(dbP, whichrec, pfname);
    if (text == NULL) { 
      printf("getname failed in instance %d\n",whichrec);
      assert(FALSE);
      }
    strcpy(name, text);
    delete [] text;
    return name; 
    }

  void setname(const char *newname)     {
    if (newname == NULL) return;
    strcpy(name, newname); 
    if (net.put(dbP, whichrec, pfname, name) == FALSE) { 
      printf("setname failed in instance %d\n",whichrec);
      assert(FALSE);
      }
    }         

  void setarmor(const char *newarmor)   {
    if (newarmor == NULL) return;
    strcpy(armor, newarmor); 
    if (net.put(dbP, whichrec, pfarmor, armor) == FALSE) { 
      printf("setarmor failed in instance %d\n",whichrec);
      assert(FALSE);
      }
    }

  void setweapon(const char *newweapon) {
    if (newweapon == NULL) return;
    strcpy(weapon, newweapon); 
    if (net.put(dbP, whichrec, pfweapon, weapon) == FALSE) { 
        printf("setweapon failed in instance %d\n",whichrec);
      assert(FALSE);
      }
    }
 
  void do_move(int x, int y);
  void resolve_move(int dx, int dy);
  void attack_player(int i);
  void attack_monster(int i);
  };

