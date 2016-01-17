#include <assert.h>
#include <iostream.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <tk.h>

// X, Y and Z dimensions of the board.
const int BOARD_X = 4,
          BOARD_Y = 4,
          BOARD_Z = 4,
          BOARD_SZ = BOARD_X * BOARD_Y * BOARD_Z,
          PLANE_SZ = BOARD_X * BOARD_Y;          

//#ifndef PROFILE
//const int maxDepth = 2;
//#endif

//#ifdef PROFILE
//int moves = 0;
//int maxDepth = 1;
//int seconds[BOARD_SZ];
//#endif

//
// BoardFull -- Returns True if the Board is Full
//
bool BoardFull(const char *board);

//
// estQuality -- Estimates the quality of a terminal board.
//
int estQuality(char *newBoard, char player);

// 
// findBest: find a good move for playe 'forWho' on the board realBoard, 
//           recursively. Returns a number (1..BOARD_SZ) for the best move found.
//
int findBest(char forWho, char *newBoard);
  
// 
// analyzeX: analyze a proposed move for player 'x' on the board realBoard, 
//           recursively. returns 2 if a win is possible, 1 if a tie is possible
//           and 0 if neither is possible.
//
int analyzeX(char *newBoard, int depth, int last);

// 
// analyzeO: analyze a proposed move for player 'o' on the board realBoard, 
//           recursively. returns 2 if a win is possible, 1 if a tie is possible
//           and 0 if neither is possible.
//
int analyzeO(char *newBoard, int depth, int last);

// 
// str2lower:
// takes string "instr" and copies it, converting upper case letters (if any) to lower case, to "outstr"
//
void str2lower(char *mystr);

//
// isValid: tests the board for validity. Returns true or false.
//          an invalid board is: an empty board, one which is the wrong length
//          contains a win or tie condition or which contains an invalid char.
//
bool isValid(const char *board);

//
// whoTurn: figures out whose turn it is; returns the player's letter.
//
char whoTurn(const char *board);

// tttCmd: play tic-tac-toe, with the assumptions provided by prof. danskin 
//         ie, that X always goes first
//

int 
ttt3D_nate_Cmd(ClientData clientData, Tcl_Interp *interp, int argc, char **argv); 

//
// wonCmd: return a string value of 'x' "o" "T" or "N" in Tcl_Interp to
//         indicate whether X or O has won, the game is tied (with a full board)
//         or whether the game is still contested; return TCL_OK or TCL_ERROR
//
int won3D_nate_Cmd(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);

//
// winner: return a string value of 'x' 'o' 't' or 'n' to indicate
//         whether X or O has won, the game is tied (with a full board)
//         or whether the game is still contested
//
// Performs checking if board is valid only if DEBUG is defined.
//
// not very generalized :(
//
char winner(const char *board);

//
// Find the best move for 
//
int findBest(char movefor, const char *realBoard, char *newBoard);

//
// winCheck -- check for a winning pattern starting at offset startPt with 
//             characters obc points apart, returns TRUE or FALSE indicating
//             a win or the lack thereof.
//
bool winCheck(const char *board, int startPt, int obc);

//
// wonAdj -- Will determine if there is a winning combination in board "board" through point pos
//
//    Hopefully someday, will be more efficient than winner. Isn't at present.
//
bool wonAdj(const char *board, int pos);

//  
// selectRandom: selects a random, valid move
//
int selectRandom(char *board);
