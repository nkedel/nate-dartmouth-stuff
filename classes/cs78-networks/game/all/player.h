class Player {
private:
  int x, y;
  int hp, xp, ac, dmg, kills;
  bool live;
  char * name;
  char * weapon;
  char * armor;
public:
  Player()  {name   = new char[32]; name[0] = 0;
             weapon = new char[32]; weapon[0] = 0;
             armor  = new char[32]; armor[0] = 0;
             live = false;
            }
            
  ~Player() {delete [] name; delete [] weapon; delete [] armor; }

  void setXY(int newx, int newy) { x = newx; y = newy; }
  void setX(int newx) { x = newx; }
  void setY(int newy) { y = newy; }
  void getXY(int& newx, int& newy) { newx = x; newy = y; }
  int getX(void) { return x; }
  int getY(void) { return y; }
  void setHP(int newHP)   { hp = newHP; }
  int  getHP(void)        { return hp;  }
  void setXP(int newXP)   { xp = newXP; }
  int  getXP(void)        { return xp;  }
  void setAC(int newAC)   { ac = newAC; }
  int  getAC(void)        { return ac;  }
  void setDMG(int newDMG) { dmg = newDMG; }
  int  getDMG(void)       { return dmg;   }
  void setkills(int newkills) 
                         { kills = newkills; }
  void setlive(bool alive) { live = alive; }
  bool getlive(void)       { return live; }
  int  getkills(void)    { return kills; }
  char* getarmor(void)   {return armor;  }
  char* getweapon(void)  {return weapon; }
  char* getname(void)    {return name;   }
  void setname(const char *newname)     { if (newname != NULL)   strcpy(name, newname);     }
  void setarmor(const char *newarmor)   { if (newarmor != NULL)  strcpy(armor, newarmor);   }
  void setweapon(const char *newweapon) { if (newweapon != NULL) strcpy(weapon, newweapon); }
  void do_move(int x, int y);
  void resolve_move(int dx, int dy);
  void attack_player(int i);
  void attack_monster(int i);
  };

