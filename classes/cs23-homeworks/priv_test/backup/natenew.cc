#include "nate.h"

// tttCmd: play tic-tac-toe, with the assumptions provided by prof. danskin 
//         ie, that X always goes first
//

inline int BoardFull(const char *board) {
  for (int i = 0; i < 64; i++) 
    if (board[i] == ' ') return FALSE;
  return true;
  }


inline int estQuality(char *newBoard, char player) {
  (void) newBoard;
  (void) player;
  return 2;
  }

//
// isValid: tests the board for validity. Returns true or false.
//          an invalid board is: an empty board, one which is the wrong length
//          contains a win or tie condition or which contains an invalid char.
//
inline bool isValid(const char *board) {
  if (board == NULL) { 
#ifdef DEBUG
    cerr << "*** NULL BOARD" << endl;
#endif 
    return FALSE; 
    }
  if (strlen(board) != BOARD_SZ)  {
#ifdef DEBUG
    cerr << "*** WRONG BOARD SIZE" << endl;
#endif
    return FALSE; 
    }
  int c;
  for ( int i = 0 ; i < (int) strlen(board) ; i++) { 
    c = (char) tolower(board[i]);
    if (c != 'x' && c != 'o' && c != ' ') {
#ifdef DEBUG
      cerr << "*** INVALID CHARCTER ON BOARD!" << endl;
#endif
      return FALSE;
      }
    }
  return TRUE;
  }

//
// whoTurn: figures out whose turn it is; returns the player's letter.
//
inline char whoTurn(const char *board) {
  int numx = 0, 
      numo = 0, 
      i;
  for ( i = 0; i < (int) strlen(board) ; i++) {
    if (board[i] == 'x') numx++;
    else if (board[i] == 'o') numo++;
    }
  if (numx == numo+1) return 'o';
#ifndef DEBUG
  if (numx != numo) { cerr << "*** invalid board found by whoTurn!" << endl; }
#endif 
  return 'x';
  }

//
// winCheck -- check for a winning pattern starting at offset startPt with 
//             characters obc points apart, returns TRUE or FALSE indicating
//             a win or the lack thereof.
//
inline bool winCheck(const char *board, int startPt, int obc) {
  assert((startPt < BOARD_SZ) && (startPt >= 0) && (obc >= 1) && (obc < BOARD_SZ));
  char c = board[startPt];
  if (c == ' ') return FALSE;
  if ((c == board[startPt + obc]) &&
      (c == board[startPt + (obc + obc)]) &&
      (c == board[startPt + (obc + obc + obc)])) 
        return True;
  return FALSE;
  } 
  
//
// winner: return a string value of 'x' 'o' 't' or 'n' to indicate
//         whether X or O has won, the game is tied (with a full board)
//         or whether the game is still contested
//
// Performs checking if board is valid only if DEBUG is defined.
//
inline char winner(const char *board) {
  int i, j;
  
  assert(isValid(board));
  
  if (BoardFull(board)) return 't';
  
  // Test horizontal lines on planes
  for (i = 0; i < BOARD_SZ; i+=BOARD_Y) 
    if (winCheck(board, i, 1)) return board[i];

  // Test vertical lines on planes  
  for (i = 0; i < BOARD_SZ; i+=PLANE_SZ)
    for (j = 0; j < BOARD_X; j++) 
      if (winCheck(board, (i+j), BOARD_X)) return board[i+j];
  
  // Test plane-plane straight lines
  for (i = 0; i < PLANE_SZ; i++) 
    if (winCheck(board, i, PLANE_SZ)) return board[i];

  // Test for same plane diagonals
  for (i = 0; i < BOARD_SZ; i+=PLANE_SZ) 
    if (winCheck(board, i, (BOARD_X + 1))) return board[i];
  for (i = (BOARD_X - 1); i < BOARD_SZ; i+=PLANE_SZ)
    if (winCheck(board, i, (BOARD_X - 1))) return board[i];
 
  // Test for horizontal plane-plane diagonals
  for (i = 0; i < PLANE_SZ; i+=BOARD_X) 
    if (winCheck(board, i, (PLANE_SZ + 1))) return board[i];
  for (i = (BOARD_X - 1); i < PLANE_SZ; i+=BOARD_X) 
    if (winCheck(board, i, (PLANE_SZ - 1))) return board[i];
    
  // Test for vertical plane-plane diagonals 
  for (i = (PLANE_SZ - BOARD_X); i < BOARD_X; i++) 
    if (winCheck(board, i, (PLANE_SZ + BOARD_X))) return board[i];
  for (i = (PLANE_SZ - BOARD_X); i < BOARD_X; i++) 
    if (winCheck(board, i, (PLANE_SZ - BOARD_X))) return board[i];

  // Test for 4 corner-corner 3d diagonals
  if (winCheck(board, 0,                    (PLANE_SZ + BOARD_X + 1))) return board[0];
  if (winCheck(board, (BOARD_X  - 1),       (PLANE_SZ + BOARD_X - 1))) return board[BOARD_X - 1];
  if (winCheck(board, (PLANE_SZ - BOARD_X), (PLANE_SZ - BOARD_X + 1))) return board[PLANE_SZ - BOARD_X];
  if (winCheck(board, (PLANE_SZ - 1),       (PLANE_SZ - BOARD_X - 1))) return board[PLANE_SZ - 1];

  return 'n';
  }

//
// wonAdj -- Will determine if there is a winning combination in board "board" through point pos
//
//    Hopefully someday, will be more efficient than winner. Isn't at present.
//
inline int wonAdj(const char *board, int pos) {
  char c = winner(board); 
  (void) pos;
  if (c=='x' || c=='o') return TRUE;
  return FALSE;
  }


int 
ttt3D_nate_Cmd(ClientData clientData, Tcl_Interp *interp,
      int argc, char **argv) 
{
  int move;
  char who;
  (void) clientData;
  char *newBoard;
  if (argc != 2) { 
    interp->result = "wrong # args";
    return TCL_ERROR;
    }
  if (isValid((const char *) argv[1]) != TRUE) { 
    interp->result = "board is invalid.";
    return TCL_ERROR;
    }
  newBoard = new char [strlen(argv[1]) + 1];
  strcpy(newBoard, argv[1]);
  str2lower(newBoard);
  if (winner(newBoard) != 'n') { 
    interp->result = "board is full or won.";
    return TCL_ERROR;
    }
  who = whoTurn((const char *) newBoard);
  move = findBest(who, newBoard);
  if (strcmp(newBoard, argv[1])!=0) {
    strcpy(newBoard, argv[1]);
    cerr << "** Corrupted Board! -- Attempting to repair." << endl;
    if (newBoard[move]!=' ') { 
      interp->result = "Illegal move!";
      return TCL_ERROR;
      }
    }
  newBoard[move] = who;
  interp->result = newBoard;
  return TCL_OK;
}


// 
// findBest: find a good move for playe 'forWho' on the board realBoard, 
//           recursively. Returns a number (1..BOARD_SZ) for the best move found.
//
int findBest(char forWho, char *newBoard) {
  int move = 0,
      best = 4,
      x = 0;
  for (int i = 0; i < BOARD_SZ ; i++) {
    if (newBoard[i] != ' ') continue;
    newBoard[i] = forWho;
    if (forWho == 'x') x = analyzeO(newBoard, 0);
                  else x = analyzeX(newBoard, 0);
    newBoard[i] = ' ';
    if (x < best) {     // changed from x<1 so that we have something
      best = x;         // even if we're about to lose
      move = i;
      }
    if (x == 3) break; 
    }
#ifdef DEBUG
  if (best == 4) { 
    if (BoardFull((const char *) newBoard)) {
      cerr << "** Full board detected in findBest!" << endl;
      assert(0);
      }
    cerr << "** No moves found on unfull board in findBest!" << endl;
    assert(0);
    }
#endif
  return move;
  }
  
// 
// analyzeX: analyze a proposed move for player 'x' on the board realBoard, 
//           recursively. returns 1 if a win is possible, 0 if a tie is possible
//           and -1 if neither is possible.
//
int analyzeX(char *newBoard, int depth) {
  int x, 
      rval = 2;
  char c = winner(newBoard);
  if (c == 'x') return 3; 
  if (c == 't') return 2; 
  if (c == 'o') return 0; 
  if (depth >= maxDepth) return 1;
  for (int i = (BOARD_SZ-1); i >= 0 ; i--) {
    if (newBoard[i] != ' ') continue;
    newBoard[i] = 'x';
    x = analyzeO(newBoard, depth + 1);
    newBoard[i] = ' ';
    if (x < rval) rval = x;
    if (x == -1) break; 
    }
  return 3 - rval ;
  }

// 
// analyzeO: analyze a proposed move for player 'o' on the board realBoard, 
//           recursively. returns 1 if a win is possible, 0 if a tie is possible
//           and -1 if neither is possible.
//
int analyzeO(char *newBoard, int depth) {
  int x,
      rval = 2;
  char c = winner(newBoard);
  if (c == 'o') return 3; 
  if (c == 't') return 2; 
  if (c == 'x') return 0; 
  if (depth >= maxDepth) return 1;
  for (int i = 0; i < BOARD_SZ ; i++) {
    if (newBoard[i] != ' ') continue;
    newBoard[i] = 'o';
    x = analyzeX(newBoard, depth + 1);
    newBoard[i] = ' ';
    if (x < rval) rval = x;
    if (x == -1) break; 
    }
  return 3 - rval;
  }

  
// 
// str2lower:
// takes string "instr" and copies it, converting upper case letters (if any) to lower case, to "outstr"
//
void str2lower(char *mystr) {
#ifdef DEBUG
  if (mystr == NULL) {
    cerr << "*** str2lower called with null string!" << endl;   
    return;
    }
#endif
  int i;
  for (i = 0; i < (int) strlen(mystr); i++) 
    mystr[i] = (char) tolower(mystr[i]);
  }  



//
// wonCmd: return a string value of 'x' "o" "t" or "n" in Tcl_Interp to
//         indicate whether X or O has won, the game is tied (with a full board)
//         or whether the game is still contested; return TCL_OK or TCL_ERROR
//
int won3D_nate_Cmd(ClientData clientData, Tcl_Interp *interp, int argc, char **argv) {
  char *newBoard;
  (void) clientData;
  if (argc != 2) { 
    interp->result = "wrong # args";
    return TCL_ERROR;
    }
  if (isValid((const char *) argv[1]) != TRUE) { 
    interp->result = "board is invalid.";
    return TCL_ERROR;
    }
  newBoard = new char [strlen(argv[1]) + 1];
  strcpy(newBoard, argv[1]);
  str2lower(newBoard);
  char c = winner(newBoard);
  switch (c) {
    case 'x': interp->result = "x"; 
              break;
    case 'o': interp->result = "o";
              break;
    case 'n': interp->result = "n";
              break;
    case 't': interp->result = "t";
              break;
#ifdef DEBUG
    default:
      cerr << "winner returned invalid parameter: " << c << endl;
      return TCL_ERROR;
#endif
    }
  return TCL_OK;
}


  

// OLD CODE FOR WINNER:
//  for (i = 0; i < 16; i++) 
//    if (wonAdj(board, i))   return board[i];
//  for (i = 16; i < 64; i+=4) 
//    if (wonAdj(board, i))   return board[i];
//  for (i = 17; i < 64; i+=16) {
//    if (wonAdj(board, i))   return board[i];
//    if (wonAdj(board, i+1)) return board[i+1];
//    if (wonAdj(board, i+2)) return board[i+2];
//    }

// OLD CODE FOR WONADJ
//  char tc = board[pos];
//  if (tc == ' ') return 0;
//  for (int oz = -1; oz < 2; oz++) {
//    for (int oy = -1; oz < 2; oz++) { 
//      for (int ox = -1; oz < 2; oz++) {
//        if (tc == bpos(board, pos, ox, oy, oz)) {
//          
//          }
//        }
//      }
//    }

//inline char bpos(const char *board, int pos, int x, int y, int z) {
//  int tloc = pos + x + (y * 4) + (z * 16);
//  if ((tloc < 0 ) || (tloc >= BOARD_SZ)) return ' ';
//  return board[tloc];
//  }
