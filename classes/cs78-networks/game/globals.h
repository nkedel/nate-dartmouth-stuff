// global variables.

// defined locally if this is our main module
#ifdef MAIN
int mapx=64, mapy=22;  // size of map, for reference
char map[64][22];      // map array
Player *player;        // pointer to our local player's Player object
Player players[8];     // array of all player objects -- these refer up 
                       //                                to the server
int me = 255;          // initialize who we are to a bogus value
int turns = 0;         // turn counter
const int n_players=4; // number of players
char randomstate[64];  // state for Digital Unix's RNG

// define these as externs, otherwise...
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


