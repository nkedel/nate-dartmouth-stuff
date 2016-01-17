/*
 * ttt.cc, derived from
 *
 * myAppInit.cc, a copy of tkAppInit.cc extended with the eq command.
 * but with the eq Command removed.
 *
 * compilation line:
 *   g++ -g -gstabs -Wall myAppInit.cc -o myApp -ltk -ltcl -lX11 -lm
 *
 * Nathan Edel 4/24/96
 */


/* 
 * tkAppInit.c --
 *
 *	Provides a default version of the Tcl_AppInit procedure for
 *	use in wish and similar Tk-based applications.
 *
 * Copyright (c) 1993 The Regents of the University of California.
 * Copyright (c) 1994 Sun Microsystems, Inc.
 *
 * See the file "license.terms" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 */

// #ifndef lint
// static char sccsid[] = "@(#) tkAppInit.c 1.15 95/06/28 13:14:28";
// #endif /* not lint */

#include <tk.h>
#include <iostream.h>
#include <ctype.h>

// X and Y dimensions of the board.
const int BOARD_X = 3,
          BOARD_Y = 3,
          BOARD_SZ = BOARD_X * BOARD_Y;

int tttCmd(ClientData clientData, Tcl_Interp *interp, int argc, char **argv); 
char whoTurn(const char *board);
bool isValid(const char *board);
int findBest(char forWho, char *newBoard);
int analyzeX(char *newBoard);
int analyzeO(char *newBoard);
int analyze(char forWho, char *newBoard);
int wonCmd(ClientData clientData, Tcl_Interp *interp, int argc, char **argv); 
char winner(const char *board);
void str2upper(char *mystr);

// 
// findBest: find a good move for playe 'forWho' on the board realBoard, 
//           recursively. Returns a number (1..BOARD_SZ) for the best move found.
//
int findBest(char forWho, char *newBoard) {
  int move = 0,
      best = 3,
      x = 0;
  for (int i = 0; i < BOARD_SZ ; i++) {
    if (newBoard[i] != ' ') continue;
    newBoard[i] = forWho;
    if (forWho == 'X') x = analyzeO(newBoard);
                  else x = analyzeX(newBoard);
    newBoard[i] = ' ';
    if (x < best) {     // changed from x<2 so that we have something
      best = x;         // even if we're about to lose
      move = i;
      }
    if (x == 0) break; 
    }
  return move;
  }
  
// 
// analyzeX: analyze a proposed move for player 'X' on the board realBoard, 
//           recursively. returns 2 if a win is possible, 1 if a tie is possible
//           and 0 if neither is possible.
//
int analyzeX(char *newBoard) {
  int x, 
      rval = 2;
  char c = winner(newBoard);
  if (c == 'X') return 2; 
  if (c == 'T') return 1; 
  if (c == 'O') return 0; 
  for (int i = 0; i < BOARD_SZ ; i++) {
    if (newBoard[i] != ' ') continue;
    newBoard[i] = 'X';
    x = analyzeO(newBoard);
    newBoard[i] = ' ';
    if (x < rval) rval = x;
    if (x == 0) break; 
    }
  return (2 - rval);
  }

// 
// analyzeO: analyze a proposed move for player 'O' on the board realBoard, 
//           recursively. returns 2 if a win is possible, 1 if a tie is possible
//           and 0 if neither is possible.
//
int analyzeO(char *newBoard) {
  int x,
      rval = 2;
  char c = winner(newBoard);
  if (c == 'O') return 2; 
  if (c == 'T') return 1; 
  if (c == 'X') return 0; 
  for (int i = 0; i < BOARD_SZ ; i++) {
    if (newBoard[i] != ' ') continue;
    newBoard[i] = 'O';
    x = analyzeX(newBoard);
    newBoard[i] = ' ';
    if (x < rval) rval = x;
    if (x == 0) break; 
    }
  return (2 - rval);
  }

  
// 
// str2upper:
// takes string "instr" and copies it, converting lower case letters to upper case, to "outstr"
//
void str2upper(char *mystr) {
#ifdef DEBUG
  if (mystr == NULL) {
    cerr << "*** str2upper called with null string!" << endl;   
    return;
    }
#endif
  int i;
  for (i = 0; i < (int) strlen(mystr); i++) 
    mystr[i] = (char) toupper(mystr[i]);
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
  for ( int i; i < (int) strlen(board) ; i++) { 
    c = (char) toupper(board[i]);
    if (c != 'X' && c != 'O' && c != ' ') {
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
    if (board[i] == 'X') numx++;
    else if (board[i] == 'O') numo++;
    }
  if (numx == numo+1) return 'O';
#ifndef DEBUG
  if (numx != numo) { cerr << "*** invalid board found by whoTurn!" << endl; }
#endif 
  return 'X';
  }


// tttCmd: play tic-tac-toe, with the assumptions provided by prof. danskin 
//         ie, that X always goes first
//

int 
tttCmd(ClientData clientData, Tcl_Interp *interp,
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
  str2upper(newBoard);
  if (winner(newBoard) != 'N') { 
    interp->result = "board is full or won.";
    return TCL_ERROR;
    }
  who = whoTurn((const char *) newBoard);
  move = findBest(who, newBoard);
  newBoard[move] = who;
  interp->result = newBoard;
  return TCL_OK;
}

//
// wonCmd: return a string value of "X" "O" "T" or "N" in Tcl_Interp to
//         indicate whether X or O has won, the game is tied (with a full board)
//         or whether the game is still contested; return TCL_OK or TCL_ERROR
//
int wonCmd(ClientData clientData, Tcl_Interp *interp, int argc, char **argv) {
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
  str2upper(newBoard);
  char c = winner(newBoard);
  switch (c) {
    case 'X': interp->result = "X"; 
              break;
    case 'O': interp->result = "O";
              break;
    case 'N': interp->result = "N";
              break;
    case 'T': interp->result = "T";
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
// winner: return a string value of "X" "O" "T" or "N" to indicate
//         whether X or O has won, the game is tied (with a full board)
//         or whether the game is still contested
//
// Performs checking if board is valid only if DEBUG is defined.
//
// not very generalized :(
//
char winner(const char *board) {
  char c;
  c = board[4];
  if (c != ' ') {
    if (c == board[0] && c == board[8]) return c;
    if (c == board[2] && c == board[6]) return c;
    if (c == board[1] && c == board[7]) return c;
    if (c == board[3] && c == board[5]) return c;
    }
  
  c = board[0];
  if (c != ' ') { 
    if (c == board[1] && c == board[2]) return c;
    if (c == board[3] && c == board[6]) return c;
    }

  c = board[8];
  if (c != ' ') { 
    if (c == board[2] && c == board[5]) return c;
    if (c == board[6] && c == board[7]) return c;
    }
    
  for (int i = 0 ; i < BOARD_SZ ; i++) 
    if (board[i] == ' ') return 'N';
  return 'T';
}



/*
 *----------------------------------------------------------------------
 *
 * main --
 *
 *	This is the main program for the application.
 *
 * Results:
 *	None: Tk_Main never returns here, so this procedure never
 *	returns either.
 *
 * Side effects:
 *	Whatever the application does.
 *
 *----------------------------------------------------------------------
 */

int
main(int argc, char **argv)
{
    Tk_Main(argc, argv, Tcl_AppInit);
    return 0;			/* Needed only to prevent compiler warning. */
}

//
// Find the best move for 
//
int findBest(char movefor, const char *realBoard, char *newBoard);


/*
 *----------------------------------------------------------------------
 *
 * Tcl_AppInit --
 *
 *	This procedure performs application-specific initialization.
 *	Most applications, especially those that incorporate additional
 *	packages, will have their own version of this procedure.
 *
 * Results:
 *	Returns a standard Tcl completion code, and leaves an error
 *	message in interp->result if an error occurs.
 *
 * Side effects:
 *	Depends on the startup script.
 *
 *----------------------------------------------------------------------
 */

int
Tcl_AppInit(Tcl_Interp *interp)
{
   Tk_Window main;
  (void) main;
  
    if (Tcl_Init(interp) == TCL_ERROR) {
	return TCL_ERROR;
    }
    if (Tk_Init(interp) == TCL_ERROR) {
	return TCL_ERROR;
    }

    /*
     * Call the init procedures for included packages.  Each call should
     * look like this:
     *
     * if (Mod_Init(interp) == TCL_ERROR) {
     *     return TCL_ERROR;
     * }
     *
     * where "Mod" is the name of the module.
     */

    /*
     * Call Tcl_CreateCommand for application-specific commands, if
     * they weren't already created by the init procedures called above.
     */

    Tcl_CreateCommand(interp, "ttt", tttCmd,
		      (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "won", wonCmd,
		      (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);

    /*
     * Specify a user-specific startup file to invoke if the application
     * is run interactively.  Typically the startup file is "~/.apprc"
     * where "app" is the name of the application.  If this line is deleted
     * then no user-specific startup file will be run under any conditions.
     */

    tcl_RcFileName = "~/.wishrc";
    return TCL_OK;
}

