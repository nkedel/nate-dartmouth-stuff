#include "nate.h"

//
// winGen 
//
inline void winGen(char *board, int startPt, int obc, char c) {
  board[startPt] =c;
  board[startPt + obc]=c;
  board[startPt + (obc + obc)]=c;
  board[startPt + (obc + obc + obc)]=c;
  } 

inline void clear(char *board) {
  for (int i = 0 ; i < 64 ; i++) board[i] = ' ';
  }

//
// genWins -- Will generate all 76 wins to a .tcl file
//
void main(void) {
  char board[65];
  int boards = 0, 
      i, j;
 
  clear(board);
  // Test horizontal lines on planes
  for (i = 0; i < BOARD_SZ; i+=BOARD_Y) {
    clear(board);
    winGen(board, i, 1, 'x');
    cout << "set board(" << boards++ << ") \"" << board << "\"" << endl;
    clear(board);
    winGen(board, i, 1, 'o');
    cout << "set board(" << boards++ << ") \"" << board << "\"" << endl;
    }
    
  clear(board);
  // Test vertical lines on planes  
  for (i = 0; i < BOARD_SZ; i+=PLANE_SZ)
    for (j = 0; j < BOARD_X; j++) {
      clear(board);
    winGen(board, (i+j), BOARD_X, 'x');
      cout << "set board(" << boards++ << ") \"" << board << "\"" << endl;
      clear(board);
    winGen(board, (i+j), BOARD_X, 'o');
      cout << "set board(" << boards++ << ") \"" << board << "\"" << endl;
      }
 
  clear(board);     
  // Test plane-plane straight lines
  for (i = 0; i < PLANE_SZ; i++) {
      clear(board);
    winGen(board, (i), PLANE_SZ, 'x');
      cout << "set board(" << boards++ << ") \"" << board << "\"" << endl;
      clear(board);
    winGen(board, (i), PLANE_SZ, 'o');
      cout << "set board(" << boards++ << ") \"" << board << "\"" << endl;
      }

  clear(board);
  // Test for same plane diagonals
  for (i = 0; i < BOARD_SZ; i+=PLANE_SZ) {
      clear(board);
    winGen(board, (i), (BOARD_X + 1), 'x');
      cout << "set board(" << boards++ << ") \"" << board << "\"" << endl;
      clear(board);
    winGen(board, (i), (BOARD_X + 1), 'o');
      cout << "set board(" << boards++ << ") \"" << board << "\"" << endl;
      }
  
  clear(board);
  for (i = (BOARD_X - 1); i < BOARD_SZ; i+=PLANE_SZ){
      clear(board);
    winGen(board, (i), (BOARD_X - 1), 'x');
      cout << "set board(" << boards++ << ") \"" << board << "\"" << endl;
      clear(board);
    winGen(board, (i), (BOARD_X - 1), 'o');
      cout << "set board(" << boards++ << ") \"" << board << "\"" << endl;
      }

  clear(board); 
  // Test for horizontal plane-plane diagonals
  for (i = 0; i < PLANE_SZ; i+=BOARD_X) {
      clear(board);
    winGen(board, (i), (PLANE_SZ + 1), 'x');
      cout << "set board(" << boards++ << ") \"" << board << "\"" << endl;
      clear(board);
    winGen(board, (i), (PLANE_SZ + 1), 'o');
      cout << "set board(" << boards++ << ") \"" << board << "\"" << endl;
      } 

  clear(board);
  for (i = (BOARD_X - 1); i < PLANE_SZ; i+=BOARD_X) {
      clear(board);
    winGen(board, (i), (PLANE_SZ - 1), 'x');
      cout << "set board(" << boards++ << ") \"" << board << "\"" << endl;
      clear(board);
    winGen(board, (i), (PLANE_SZ - 1), 'o');
      cout << "set board(" << boards++ << ") \"" << board << "\"" << endl;
      }
    
  // Test for vertical plane-plane diagonals 
  clear(board);
  for (i = (PLANE_SZ - BOARD_X); i < BOARD_X; i++) {
      clear(board);
    winGen(board, (i), (PLANE_SZ + BOARD_X), 'x');
      cout << "set board(" << boards++ << ") \"" << board << "\"" << endl;
      clear(board);
    winGen(board, (i), (PLANE_SZ + BOARD_X), 'o');
      cout << "set board(" << boards++ << ") \"" << board << "\"" << endl;
      }
  clear(board);
  for (i = (PLANE_SZ - BOARD_X); i < BOARD_X; i++) {
      clear(board);
    winGen(board, (i), (PLANE_SZ - BOARD_X), 'x');
      cout << "set board(" << boards++ << ") \"" << board << "\"" << endl;
      clear(board);
    winGen(board, (i), (PLANE_SZ - BOARD_X), 'o');
      cout << "set board(" << boards++ << ") \"" << board << "\"" << endl;
      }
      
  // Test for 4 corner-corner 3d diagonals
  clear(board);
  clear(board);
    winGen(board, 0, (PLANE_SZ + BOARD_X + 1), 'x');
  cout << "set board(" << boards++ << ") \"" << board << "\"" << endl;
  clear(board);
    winGen(board, 0, (PLANE_SZ + BOARD_X + 1), 'o');
  cout << "set board(" << boards++ << ") \"" << board << "\"" << endl;

  clear(board);
  clear(board);
    winGen(board, (BOARD_X - 1), (PLANE_SZ + BOARD_X - 1), 'x');
  cout << "set board(" << boards++ << ") \"" << board << "\"" << endl;
  clear(board);
    winGen(board, (BOARD_X - 1), (PLANE_SZ + BOARD_X - 1), 'o');
  cout << "set board(" << boards++ << ") \"" << board << "\"" << endl;

  clear(board);
  clear(board);
    winGen(board, (PLANE_SZ - BOARD_X), (PLANE_SZ - BOARD_X + 1), 'x');
  cout << "set board(" << boards++ << ") \"" << board << "\"" << endl;
  clear(board);
    winGen(board, (PLANE_SZ - BOARD_X), (PLANE_SZ - BOARD_X + 1), 'o');
  cout << "set board(" << boards++ << ") \"" << board << "\"" << endl;

  clear(board);
  clear(board);
    winGen(board, (PLANE_SZ - 1), (PLANE_SZ - BOARD_X - 1), 'x');
  cout << "set board(" << boards++ << ") \"" << board << "\"" << endl;
  clear(board);
    winGen(board, (PLANE_SZ - 1), (PLANE_SZ - BOARD_X - 1), 'o');
  cout << "set board(" << boards++ << ") \"" << board << "\"" << endl;
  }
  
