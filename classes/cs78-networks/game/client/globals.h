#ifdef MAIN
int mapx=64, mapy=22;
char map[64][22];
Player *player;
Player players[8];
int me = 255;
int turns = 0;
const int n_players=4;
char randomstate[64];
#else
extern int mapx, mapy;
extern char map[64][22];
extern int me;
extern Player *player;
extern Player players[8];
const int n_players = 4;
extern char randomstate[64];
extern int turns;
#endif


