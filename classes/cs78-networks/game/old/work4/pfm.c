#ifdef MAIN
const int mapx=64, mapy=22;
static char map[64][22];
Monster monsters[16];
int mon_x[16],mon_y[16];
Player *player;
Player players[8];
int ply_x[8],ply_y[8];
int me = 0;
int n_monsters = 4;
int n_players=4;
char randomstate[64];
const int n_mon_names = 4;
const char *mon_names[] = { "Evil Homer", "Marge", "Bart", "Lisa" }; 
const char mon_tiles[]  = { 'h', 'm', 'b', 'l' };
#else
extern const int mapx, mapy;
extern char map[64][22];
extern Monster monsters[16];
extern int mon_x[16], mon_y[16];
extern Player players[8];
extern int ply_x[8], ply_y[8];
extern int n_monsters;
extern int n_players;
extern char randomstate[64];
extern const int n_mon_names;
extern const char *mon_names;
extern const char mon_tiles;
#endif

const int mfX = 0, mfY = 1, mfHP = 2, mfAC = 3, mfDMG= 4, mfname= 5, mftile =6, mflive = 7
