#include "nate.h"

//
// BoardFull - is the Board Full?
//
inline bool BoardFull(const char *board) {
  for (int i = 0; i < 64; i++) 
    if (board[i] == ' ') return FALSE;
  return TRUE;
  }
  
// 
// str2lower:
// takes string "instr" and copies it, converting upper case letters (if any) to lower case, to "outstr"
//
inline void str2lower(char *mystr) {
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
  int c, i;
  for ( i = 0 ; i < (int) strlen(board) ; i++) { 
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
// wonAdj -- Will determine if there is a winning combination in board "board" through point pos
//
//    Hopefully someday, will be more efficient than winner. Isn't at present.
//
inline bool wonAdj(const char *board, int pos) {
  int i;
  
  assert(isValid(board));
  
  // Test horizontal lines on plane
  i = (pos - (pos % BOARD_X));
  if (winCheck(board, i, 1)) return TRUE;

  // Test vertical lines on planes  
  i = (pos % BOARD_X) + (pos - (pos % PLANE_SZ));
  if (winCheck(board, i, BOARD_X)) return TRUE;
  
  // Test plane-plane straight lines
  i = (pos % PLANE_SZ); 
  if (winCheck(board, i, PLANE_SZ)) return TRUE;

  // Test for same plane diagonals
  i = (pos - (pos % PLANE_SZ));
  if (winCheck(board, i, (BOARD_X + 1))) return TRUE;
  i += 3;
  if (winCheck(board, i, (BOARD_X - 1))) return TRUE;
 
  // Test for horizontal plane-plane diagonals
  i = (pos % PLANE_SZ) - (pos % BOARD_X); 
  if (winCheck(board, i, (PLANE_SZ + 1))) return TRUE;
  i += 3;
  if (winCheck(board, i, (PLANE_SZ - 1))) return TRUE;
    
  // Test for vertical plane-plane diagonals 
  i = (pos % BOARD_X);
  if (winCheck(board, i, (PLANE_SZ + BOARD_X))) return TRUE;
  i += 12;
  if (winCheck(board, i, (PLANE_SZ - BOARD_X))) return TRUE;

  // Test for 4 corner-corner 3d diagonals
  if (winCheck(board, 0,                    (PLANE_SZ + BOARD_X + 1))) return TRUE;
  if (winCheck(board, (BOARD_X  - 1),       (PLANE_SZ + BOARD_X - 1))) return TRUE;
  if (winCheck(board, (PLANE_SZ - BOARD_X), (PLANE_SZ - BOARD_X + 1))) return TRUE;
  if (winCheck(board, (PLANE_SZ - 1),       (PLANE_SZ - BOARD_X - 1))) return TRUE;

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
// selectRandom: selects a random, valid move
//
inline int selectRandom(char *board) { 
  int rmove = 64, 
      notfound = 1;
  while (notfound > 0) {
#ifdef DEBUG
    if ((notfound++) > 16) 
      cerr << "** More than 16 Random #'s required to find valid move!" << endl;
#endif
    rmove = (rand() % 64);
    if (board[rmove] == ' ') notfound = 0;
    }
  return rmove;
  }

// 
// findBest: find a good move for playe 'forWho' on the board realBoard, 
//           recursively. Returns a number (1..BOARD_SZ) for the best move found.
//
int findBest(char forWho, char *newBoard) {
  int bestmv  = BOARD_SZ,
      winmv   = BOARD_SZ,
      blockmv = BOARD_SZ,
      best    = 4,
      worst   = -4,
      x = 0,
      i = 0;
  char otherguy;
#ifdef PROFILE
  time_t starttime, endtime;
  time(&starttime);
  cerr << "--Starting move." << endl;
#endif
#ifdef DEBUG
  cerr << "#" << i << " " << newBoard << endl;
#endif
  otherguy = ( forWho == 'x' ) ? 'o' : 'x';
  for (i = 0; i < BOARD_SZ ; i++) {
#ifdef DEBUG
    cerr << endl << "#" << i << ": " << newBoard[i] << " ";
#endif
    if (newBoard[i] != ' ') continue;

    newBoard[i] = otherguy;
    if (wonAdj((const char *) newBoard, i)) blockmv = i;
#ifdef DEBUG
    cerr << "(" << newBoard[i] << ", "; 
    cerr << winner(newBoard) << ", ";
    cerr << (wonAdj((const char *) newBoard, i) ? "True" : "False") << ") ";
#endif

    newBoard[i] = forWho;
    if (wonAdj((const char *) newBoard, i)) winmv = i;
#ifdef DEBUG
    cerr << "(" << newBoard[i] << ", " << winner(newBoard) << ", " << (wonAdj((const char *) newBoard, i) ? "True" : "False") << ") ";
#endif
    if ((winmv == BOARD_SZ) && (blockmv == BOARD_SZ) && (best > -3))  {
      x = ((forWho == 'x') ? analyzeO(newBoard, 0, i) : analyzeX(newBoard, 0, i));
      if (x < best) {     // changed from x<1 so that we have something
        best = x;         // even if we're about to lose
        bestmv = i;
        }
      if (x > worst) worst = x;
      }
    newBoard[i] = ' ';
#ifdef DEBUG
    cerr << "(x=" << x << "/" << best << "/" << worst << ", " << bestmv << ", " << winmv << ", " << blockmv << ") ";
#endif
    if (winmv != BOARD_SZ) break;
    }
#ifdef PROFILE
  time(&endtime);
  cerr << "-- Time to move: " << (endtime - starttime) << " Seconds @ Depth " << maxDepth << endl;
#endif
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
  if (winmv != BOARD_SZ) return winmv;
  if (blockmv != BOARD_SZ) return blockmv;
  if ((worst <= best) && (best >= 1)) bestmv = selectRandom(newBoard);
#ifdef DEBUG
  cerr << endl << "Bestmv: " << bestmv << endl;
#endif
  return bestmv;
  }

// 
// analyzeX: analyze a proposed move for player 'x' on the board newBoard, 
//           recursively. 
//
// Returns: 3 if there is nothing visible within depth
//          2 if there is an opponent win within depth
//          0 if there is a tie within depth
//          -2 if there is a block within depth
//          -3 if there is a win withing depth
//
int analyzeX(char *newBoard, int depth, int last) {
  int x, 
      rval = 3;
  if (wonAdj(newBoard, last)) return -3; 
  if (depth >= maxDepth) return 1;
  if (BoardFull(newBoard)) return 0;
  for (int i = (BOARD_SZ-1); i >= 0 ; i--)
    if (newBoard[i] == ' ') {
      newBoard[i] = 'o';
      if (wonAdj(newBoard, i)) {
        newBoard[i] = ' ';
        return -2;
        }
      newBoard[i] = 'x';
      x = analyzeO(newBoard, depth + 1, i);
      newBoard[i] = ' ';
      if (x < rval) rval = x;
      }
  return (rval == 1) ? 1 : (0 - rval);
  }

// 
// analyzeO: analyze a proposed move for player 'o' on the board newBoard, 
//           recursively. 
//
// Returns: 3 if there is nothing visible within depth
//          2 if there is an opponent win within depth
//          0 if there is a tie within depth
//          -2 if there is a block within depth
//          -3 if there is a win withing depth
//
int analyzeO(char *newBoard, int depth, int last) {
  int x, 
      rval = 3;
  if (wonAdj(newBoard, last)) return -3; 
  if (depth >= maxDepth) return 1;
  if (BoardFull(newBoard)) return 0;
  for (int i = (BOARD_SZ-1); i >= 0 ; i--)
    if (newBoard[i] == ' ') {
      newBoard[i] = 'x';
      if (wonAdj(newBoard, i)) {
        newBoard[i] = ' ';
        return -2;
        }
      newBoard[i] = 'o';
      x = analyzeX(newBoard, depth + 1, i);
      newBoard[i] = ' ';
      if (x < rval) rval = x;
      }
  return (rval == 1) ? 1 : (0 - rval);
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


// tttCmd: play tic-tac-toe, with the assumptions provided by prof. danskin 
//         ie, that X always goes first
//

int ttt3D_nate_Cmd(ClientData clientData, Tcl_Interp *interp, int argc, char **argv) 
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
