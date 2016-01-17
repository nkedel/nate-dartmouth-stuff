#include "nate.h"

//
// BoardFull - is the Board Full?
//
bool BoardFull(const char *board) {
  for (int i = 0; i < 64; i++) 
    if (board[i] == ' ') return FALSE;
  return TRUE;
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
// isValid: tests the board for validity. Returns true or false.
//          an invalid board is: an empty board, one which is the wrong length
//          contains a win or tie condition or which contains an invalid char.
//
bool isValid(const char *board) {
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
char whoTurn(const char *board) {
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
