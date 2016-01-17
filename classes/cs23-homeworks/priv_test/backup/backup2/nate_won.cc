#include "nate.h"

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
bool wonAdj(const char *board, int pos) {
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
char winner(const char *board) {
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
