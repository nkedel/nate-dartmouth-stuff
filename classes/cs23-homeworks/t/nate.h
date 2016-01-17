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

const int maxDepth = 2;

//
// BoardFull - is the Board Full?
//
bool BoardFull(const char *board);
  
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

//
// wonCmd: return a string value of 'x' "o" "t" or "n" in Tcl_Interp to
//         indicate whether X or O has won, the game is tied (with a full board)
//         or whether the game is still contested; return TCL_OK or TCL_ERROR
//
int won3D_nate_Cmd(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);

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
// winner: return a string value of 'x' 'o' 't' or 'n' to indicate
//         whether X or O has won, the game is tied (with a full board)
//         or whether the game is still contested
//
// Performs checking if board is valid only if DEBUG is defined.
//
char winner(const char *board);
  
//  
// selectRandom: selects a random, valid move
//
int selectRandom(char *board); 

// 
// findBest: find a good move for playe 'forWho' on the board realBoard, 
//           recursively. Returns a number (1..BOARD_SZ) for the best move found.
//
int findBest(char forWho, char *newBoard);

// 
// analyzeX: analyze a proposed move for player 'x' on the board newBoard, 
// analyzeY  recursively. 
//
// Returns: 3 if there is nothing visible within depth
//          2 if there is an opponent win within depth
//          0 if there is a tie within depth
//          -2 if there is a block within depth
//          -3 if there is a win withing depth
//
int analyzeX(char *newBoard, int depth, int last);
int analyzeO(char *newBoard, int depth, int last);

// tttCmd: play tic-tac-toe, with the assumptions provided by prof. danskin 
//         ie, that X always goes first
//

int ttt3D_nate_Cmd(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);
