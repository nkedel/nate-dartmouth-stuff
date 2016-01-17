#include "nate.h"

int estQuality(char *newBoard, char player) {
  (void) newBoard;
  (void) player;
  return (rand() % 3) - 1;
  }

// 
// findBest: find a good move for playe 'forWho' on the board realBoard, 
//           recursively. Returns a number (1..BOARD_SZ) for the best move found.
//
int findBest(char forWho, char *newBoard) {
  int move = 0,
      best = 4,
      x = 0,
      i = 0;
#ifdef DEBUG
  time_t starttime, endtime;
  time(&starttime);
  cerr << "Junk." << endl;
#endif
  if (forWho == 'x') 
    for (i = 0; i < BOARD_SZ ; i++) {
      if (newBoard[i] != ' ') continue;
      newBoard[i] = forWho;
      cerr << newBoard << endl;
        x = analyzeO(newBoard, 0);
      newBoard[i] = ' ';
      if (x < best) {     // changed from x<1 so that we have something
        best = x;         // even if we're about to lose
        move = i;
        }
      if (x == 0) break; 
      }
  else 
    for (i = (BOARD_SZ - 1); i >= 0 ; i--) {
      if (newBoard[i] != ' ') continue;
      newBoard[i] = forWho;
      x = analyzeX(newBoard, 0);
      newBoard[i] = ' ';
      if (x < best) {     // changed from x<1 so that we have something
        best = x;         // even if we're about to lose
        move = i;
        }
      if (x == 0) break; 
      }
#ifdef DEBUG
  time(&endtime);
  cerr << "-- Time to move: " << (endtime - starttime) << " Seconds" << endl;
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
      rval = 4;
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
    if (x == 0) break; 
    }
  return (3 - rval);
  }

// 
// analyzeO: analyze a proposed move for player 'o' on the board realBoard, 
//           recursively. returns 1 if a win is possible, 0 if a tie is possible
//           and -1 if neither is possible.
//
int analyzeO(char *newBoard, int depth) {
  int x,
      rval = 4;
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
    if (x == 0) break; 
    }
  return (3 - rval);
  }
