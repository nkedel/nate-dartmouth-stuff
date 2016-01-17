CFLAGS = -g -ggdb -Wall 
LFLAGS = -ggdb
DEFINES  = 
CC =	gcc
LD =	gcc
RM =	rm -f
CP =	cp
LIBS =	-lcurses

# Files ------------------------

SRCS = game.cc player.cc db.cc die.cc
OBJS = $(SRCS:.cc=.o)

# Build Targets ----------------

PROGRAMS = game

all:	$(PROGRAMS)

%.o:	%.cc
	$(CC) -c $(CFLAGS) $(DEFINES) $(@:.o=.cc)

game: $(OBJS)
	$(RM) $@
	$(LD) $(LFLAGS) -o $@ $(OBJS) $(LIBS)

# Install Targets --------------

# Utility Targets --------------

clean:
	$(RM) $(PROGRAMS)
	$(RM) *.o
	$(RM) core
	$(RM) *~

depend:

	makedepend -s "# DO NOT DELETE" -- $(SRCS)

# Dependencies -----------------
# DO NOT DELETE

game.o: game.h /usr/include/assert.h /usr/include/standards.h
game.o: /usr/include/stdio.h /usr/include/curses.h /usr/include/stddef.h
game.o: /usr/include/termios.h /usr/include/sys/types.h
game.o: /usr/include/va_list.h /usr/include/stdarg.h /usr/include/unistd.h
game.o: /usr/include/sys/access.h /usr/include/stdlib.h /usr/include/time.h
game.o: player.h globals.h
player.o: game.h /usr/include/assert.h /usr/include/standards.h
player.o: /usr/include/stdio.h /usr/include/curses.h /usr/include/stddef.h
player.o: /usr/include/termios.h /usr/include/sys/types.h
player.o: /usr/include/va_list.h /usr/include/stdarg.h /usr/include/unistd.h
player.o: /usr/include/sys/access.h /usr/include/stdlib.h /usr/include/time.h
player.o: player.h globals.h
db.o: game.h /usr/include/assert.h /usr/include/standards.h
db.o: /usr/include/stdio.h /usr/include/curses.h /usr/include/stddef.h
db.o: /usr/include/termios.h /usr/include/sys/types.h /usr/include/va_list.h
db.o: /usr/include/stdarg.h /usr/include/unistd.h /usr/include/sys/access.h
db.o: /usr/include/stdlib.h /usr/include/time.h player.h globals.h
die.o: game.h /usr/include/assert.h /usr/include/standards.h
die.o: /usr/include/stdio.h /usr/include/curses.h /usr/include/stddef.h
die.o: /usr/include/termios.h /usr/include/sys/types.h /usr/include/va_list.h
die.o: /usr/include/stdarg.h /usr/include/unistd.h /usr/include/sys/access.h
die.o: /usr/include/stdlib.h /usr/include/time.h player.h globals.h
