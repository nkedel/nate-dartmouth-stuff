#ifdef MAIN
const int mapx=64, mapy=22;
char map[64][22];
Monster monsters[16];
Player *player;
Player players[8];
int me = 0;
int n_monsters = 8;
int n_players=4;
char randomstate[64];
int n_mon_names = 4;
char *mon_names[] = { "Evil Homer", "Marge", "Bart", "Lisa" }; 
char mon_tiles[]  = { 'h', 'm', 'b', 'l' };
#else
const int mapx=64, mapy=22;
extern char map[64][22];
extern Monster monsters[16];
extern int me;
extern Player *player;
extern Player players[8];
extern int n_monsters;
extern int n_players;
extern char randomstate[64];
extern char **mon_names;
extern int n_mon_names;
extern char *mon_tiles;
#endif

