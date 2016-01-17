#include "nate.h"

// tttCmd: play tic-tac-toe, with the assumptions provided by prof. danskin 
//         ie, that X always goes first
//

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
