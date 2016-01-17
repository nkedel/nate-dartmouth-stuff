// 
// findBest: find a good (despite the name, not necesarily best) defensive move
//           for playe 'forWho' on the board realBoard, recursively.
//           Returns a number (1..BOARD_SZ) for the best move found.
//
int findBest(char forWho, const char *realBoard, char *newBoard) {
  char c,otherGuy;
  if (forWho == 'X') otherGuy = 'O';
    else otherGuy = 'X'; 
  int winMove = -1,
      tieMove = -1,
      defMove = -1,
      safeMove= -1,
      myLookAhead = 0,
      ogLookAhead = 0;
  for (int i = 0; i < BOARD_SZ ; i++) {
    if (newBoard[i] == ' ') continue;
    newBoard[i] = forWho;
    if ((c = winner(newBoard)) != 'N') {
      if (c == forWho) winMove = i;
      if (c == 'T') tieMove = i;
      }
    else {
      myLookAhead = findBest(otherGuy, realBoard, newBoard);
      newBoard[myLookAhead] = otherGuy;
      if ((c = winner(newBoard)) != 'N') {
        if (c == otherGuy) defMove = myLookAhead;
        if (c == 'T') tieMove = i;
        }
      else {
        safeMove = i;
        ogLookAhead = findBest(forWho, realBoard, newBoard);
        newBoard[ogLookAhead] = forWho;
         if ((c = winner(newBoard)) != 'N') {
          if (c == forWho) winMove = i;
          if (c == 'T') tieMove = i;
          }
        newBoard[ogLookAhead] = ' ';
        }
      newBoard[myLookAhead] = ' ';
      } 
    newBoard[i] = ' ';
    }
  if (winMove >= 0) return winMove;
  if (defMove >= 0) return defMove;
  if (tieMove >= 0) return tieMove;
  if (safeMove >= 0) return safeMove;
  cerr << "*** MAJOR DISCREPANCY. SHUTTING DOWN." << endl;
  exit(1);
  }
