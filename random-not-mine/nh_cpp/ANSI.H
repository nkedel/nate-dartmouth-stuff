#ifndef ANSI_H
#define ANSI_H
#include "global.h"

#define ANSI_BLACK          "\x1b[0;30m"
#define ANSI_RED            "\x1b[0;31m"
#define ANSI_GREEN          "\x1b[0;32m"
#define ANSI_BROWN          "\x1b[0;33m"
#define ANSI_BLUE           "\x1b[0;34m"
#define ANSI_MAGENTA        "\x1b[0;35m"
#define ANSI_CYAN           "\x1b[0;36m"
#define ANSI_GRAY           "\x1b[0;37m"
#define ANSI_BRIGHT_BLACK   "\x1b[0;30;1m"
#define ANSI_ORANGE         "\x1b[0;31;1m"
#define ANSI_BRIGHT_GREEN   "\x1b[0;32;1m"
#define ANSI_YELLOW         "\x1b[0;33;1m"
#define ANSI_BRIGHT_BLUE    "\x1b[0;34;1m"
#define ANSI_BRIGHT_MAGENTA "\x1b[0;35;1m"
#define ANSI_BRIGHT_CYAN    "\x1b[0;36;1m"
#define ANSI_WHITE          "\x1b[0;37;1m"

#define ANSI_CLEAR          "\x1b[2J"
#define ANSI_CLEAR_LINE     "\x1b[K"

char *getcolor(uchar color);

#endif
