/* this is our player class which defines the datatype for a player, and is an interface between the implemnetation of the player records on the database server and the logical ile 


class Player {
private:
  int x, y;
  int whichrec;
  int old_x, old_y;
  int hp, xp, ac, dmg, kills;
  bool live;
  bool token;
  char * name;
  char * weapon;
  char * armor;
  const int pfX = 0, pfY = 1, pfHP = 2, pfXP = 3, 
            pfAC = 4, pfDMG = 5, pfkills = 6 , pflive = 7, 
            pfname = 8, pfweapon = 9, pfarmor = 10;
            
public:
  Player()  {
             token  = false;
             name   = new char[32]; name[0] = 0;
             weapon = new char[32]; weapon[0] = 0;
             armor  = new char[32]; armor[0] = 0;
             live = false;
            }
            
  ~Player() {delete [] name; delete [] weapon; delete [] armor; }

  void settoken(void) { token = true; }
  void cleartoken(void) { token = false; }
  
  void setX(int newx) { 
    assert (token == true);
    char text[16];
    x = newx; 
    sprintf(text,"%d",x);
    if (put(dbP, whichrec, pfX, text) == false) die("setX failed!");
    }

  void setY(int newy) {
    assert (token == true);
    char text[16];
    y = newy; 
    sprintf(text,"%d",y);
    put(dbP, whichrec, pfY, text);
    }
    
  int getX(void) { 
    if (token) return x;
    char *text;
    text = get(dbP, whichrec, pfX);
    x = atoi(text);
    delete [] text;
    return x; 
    }
    
  int getY(void) { 
    if (token) return y;
    char *text;
    text = get(dbP, whichrec, pfY);
    y = atoi(text);
    delete [] text;    
    return y; 
    }

  void setHP(int newHP)   {
    assert (token == true);
    char text[16];
    hp = newhp; 
    sprintf(text,"%d",y);
    put(dbP, whichrec, pfHP, text);
    }

  int  getHP(void)        {
    char *text;
    text = get(dbP, whichrec, pfHP);
    hp = atoi(text);
    delete [] text;
    return hp; 
    }

  void setXP(int newXP)   {
    assert (token == true);
    char text[16];
    hp = newxp; 
    sprintf(text,"%d",y);
    put(dbP, whichrec, pfXP, text);
    }
    
  int  getXP(void)        {
    char *text;
    text = get(dbP, whichrec, pfXP);
    xp = atoi(text);
    delete [] text;
    return xp; 
    }

  void setAC(int newAC)   { 
    assert (token == true);
    char text[16];
    hp = newxp; 
    sprintf(text,"%d",y);
    put(dbP, whichrec, pfXP, text);
    }
    
  int  getAC(void)        {
    char *text;
    text = get(dbP, whichrec, pfXP);
    xp = atoi(text);
    delete [] text;
    return xp; 
    }

  void setDMG(int newDMG) { 
    assert (token == true);
    char text[16];
    dmg = newDMG; 
    sprintf(text,"%d",y);
    if (put(dbP, whichrec, pfDMG, text) == false) die("setDMG Failed");
    }

  int  getDMG(void)       {
    char *text;
    text = get(dbP, whichrec, pfDMG);
    dmg = atoi(text);
    delete [] text;
    return dmg; 
    }

  void setkills(int newkills) {
    assert (token == true);
    char text[16];
    kills = newkills; 
    sprintf(text,"%d",y);
    if (put(dbP, whichrec, pfkills, text) == false) die("setkills Failed");
    }

  int  getkills(void)    {
    char *text;
    text = get(dbP, whichrec, pfkills);
    xp = atoi(text);
    delete [] text;
    return xp; 
    }

  void setlive(bool alive) {
    assert (token == true);
    char text[16];
    live = alive;; 
    if (alive) {
      if (put(dbP, whichrec, pflive, "1") == false) die("setkills Failed");
      }
    else 
      if (put(dbP, whichrec, pflive, "0") == false) die("setkills Failed");
    }

  bool getlive(void)       {
    char *text;
    text = get(dbP, whichrec, pflive);
    if (atoi(text) != 0) live=true; else live=false;
    delete [] text;
    return live; 
    }


  char* getarmor(void) 
    char *text;
    text = get(dbP, whichrec, pfarmor);
    strcpy(armor, text)
    delete [] text;
    return armor; 
    }

  char* getweapon(void)  {return weapon; }
    char *text;
    text = get(dbP, whichrec, pfweapon);
    strcpy(weapon, text);
    delete [] text;
    return weapon; 
    }

  char* getname(void)    {return name;   }
    char *text;
    text = get(dbP, whichrec, pfname);
    strcpy(name, text);
    delete [] text;
    return name; 
    }

  void setname(const char *newname)     {
    if (newname == NULL) return;
    assert (token == true);
    strcpy(name, newname); 
    if (put(dbP, whichrec, pfname, name) == false) die("setname Failed");
    }         
    }

  void setarmor(const char *newarmor)   {
    if (newarmor == NULL) return;
    assert (token == true);
    strcpy(armor, newarmor); 
    if (put(dbP, whichrec, pfarmor, armor) == false) die("setarmor Failed");
    }

  void setweapon(const char *newweapon) {
    if (newweapon == NULL) return;
    assert (token == true);
    strcpy(weapon, newweapon); 
    if (put(dbP, whichrec, pfweapon, weapon) == false) die("setweapon Failed");
    }
 
  void do_move(int x, int y);
  void resolve_move(int dx, int dy);
  void attack_player(int i);
  void attack_monster(int i);
  };

