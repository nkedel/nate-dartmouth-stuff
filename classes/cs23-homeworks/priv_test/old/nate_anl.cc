#include "nate.h"
int maxDepth = 2;

//  
// selectRandom: selects a random, valid move
//
int selectRandom(char *board) { 
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
//    if (x == -2) break; 
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
// Returns: 2 if there is an opponent win within depth
//          1 if there is nothing visible within depth
//          0 if there is a tie within depth
//          -2 if there is a win/block within depth
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
//      if (x > 2) break; 
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
//          -1 if there is a block within depth
//          -2 if there is a win withing depth
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
//      if (x > 2) break; 
      }
  return (rval == 1) ? 1 : (0 - rval);
  }
