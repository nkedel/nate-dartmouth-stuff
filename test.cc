#include <stdio.h>
#ifdef LINUX
#include <ncurses/curses.h>
#else
#include <curses.h>
#endif

int myx, myy, mxx, mxy;

void init_screen(void);
void play_game(void);
inline void blankme(void) { mvaddch(myy, myx, ' '); }
inline void drawme(void) { mvaddch(myy, myx, '@'); refresh(); }

int main(int argc, char **argv) {
	printf("Welcome to my silly game!\n");
	init_screen();
	myx=0;
	myy=0;
#ifdef LINUX
	getmaxyx(stdscr, mxy, mxx);
#else
  mxx=COLS;
  mxy=LINES;
#endif
	play_game();
	endwin();
	}

void play_game(void) {
	bool done = FALSE;
	int x;
	while (done != TRUE) {
		x = getch();
		switch (x) {
			case 'q':
			case 'Q': 
					done = TRUE;
					break;
			case KEY_UP: 
					blankme();
					if (myy > 0) myy--;
					drawme();
					break;
			case KEY_DOWN: 
					blankme();
					if (myy < mxy) myy++;
					drawme();
					break;
			case KEY_LEFT: 
					blankme();
					if (myx > 0) myx--;
					drawme();
					break;
			case KEY_RIGHT: 
					blankme();
					if (myx < mxx) myx++;
					drawme();
					break;
			case KEY_A1: 
					blankme();
					if (myy > 0) myy--;
					if (myx > 0) myx--;
					drawme();
					break;
			case KEY_A3: 
					blankme();
					if (myy > 0) myy--;
					if (myx < mxx) myx++;
					drawme();
					break;
			case KEY_C1: 
					blankme();
					if (myy < mxy) myy++;
					if (myx > 0) myx--;
					drawme();
					break;
			case KEY_C3: 
					blankme();
					if (myy < mxy) myy++;
					if (myx < mxx) myx++;
					drawme();
					break;
			default: break;
			}
		}
	}
	
void init_screen(void) {
	initscr(); 
	cbreak(); 
	noecho();
	nonl();
	intrflush(stdscr, FALSE);
	keypad(stdscr, TRUE);
	clear();
	refresh();
	}
