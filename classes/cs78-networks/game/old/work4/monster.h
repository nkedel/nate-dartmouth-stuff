const int dbM = 2;
const int mfX = 0, mfY = 1, mfHP = 2, mfAC = 3, mfDMG= 4, mfname= 5, 
          mftile =6, mflive = 7;

class Monster {
 public:
  Monster()  {name = new char[32]; name[0] = 0; live = false;}
  ~Monster() {delete [] name;}


  void setXY(int newx, int newy) { x = newx; y = newy; }
  int setX(int newx) { x = newx; return x;}
  int setY(int newy) { y = newy; return y;}
  void getXY(int& newx, int& newy) { newx = x; newy = y; }
  int  getX(void) { return x; }
  int  getY(void) { return y; }
  void setHP(int newHP)   { hp = newHP; }
  int  getHP(void)        { return hp;  }
  void setAC(int newAC)   { ac = newAC; }
  int  getAC(void)        { return ac;  }
  void setlive(bool alive) { live = alive; }
  bool getlive(void)       { return live; }
  void setDMG(int newDMG) { dmg = newDMG; }
  int  getDMG(void)       { return dmg;   }

  void settile(int ntile) { tile = ntile; }
  int  gettile(void)       { return tile;   }

  char* getname(void)    {return name;}
  void setname(const char *newname)     { if (newname != NULL)   strcpy(name, newname);     }
  void do_move();
  void moveto(int dx, int dy);
  void attack(int i);

 private:
  int x, y;
  int hp, ac, dmg;
  char * name;
  int tile;
  bool live;
  };
