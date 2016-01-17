#include "ansi.h"

char *getcolor(uchar color){
    switch (color){
        // also NO_COLOR, they are the same #
        case BLACK:return(ANSI_BLACK);
        case RED : return(ANSI_RED);
        case GREEN:return(ANSI_GREEN);
        case BROWN:return(ANSI_BROWN);
        case BLUE:return(ANSI_BLUE);
        case MAGENTA:return(ANSI_MAGENTA);
        case CYAN:return(ANSI_CYAN);
        case BRIGHT_BLACK:return(ANSI_BRIGHT_BLACK);
        case GRAY:return(ANSI_GRAY);
        case ORANGE:return(ANSI_ORANGE);
        case BRIGHT_GREEN:return (ANSI_BRIGHT_GREEN);
        case YELLOW:return(ANSI_YELLOW);
        case BRIGHT_BLUE:return(ANSI_BRIGHT_BLUE);
        case BRIGHT_MAGENTA:return(ANSI_BRIGHT_MAGENTA);
        case BRIGHT_CYAN:return(ANSI_BRIGHT_CYAN);
        case WHITE:return(ANSI_WHITE);
    };
}
