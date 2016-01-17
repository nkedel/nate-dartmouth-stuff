CFLAGS = -g -ggdb -Wall 
LFLAGS = -ggdb
DEFINES  = 
CC =	g++
LD =	g++
RM =	rm -f
CP =	cp
LIBS =	-lcurses

# Files ------------------------

SRCS = game.cc player.cc db.cc die.cc gamec.cc String.cc sockaddr.cc sockcom.cc
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
game.o: /usr/include/stdio.h sockcom.h sockaddr.h /usr/include/netinet/in.h
game.o: /usr/include/sys/types.h /usr/include/errno.h /usr/include/fcntl.h
game.o: /usr/include/sys/fcntl.h /usr/include/netdb.h /usr/include/pwd.h
game.o: /usr/include/signal.h /usr/include/sys/siginfo.h
game.o: /usr/include/sys/sysmisc.h /usr/include/machine/signal.h
game.o: /usr/include/stdlib.h /usr/include/string.h /usr/include/sys/socket.h
game.o: /usr/include/sys/uio.h /usr/include/sys/time.h
game.o: /usr/include/sys/select.h /usr/include/unistd.h
game.o: /usr/include/sys/access.h String.h /usr/include/ctype.h gamec.h
game.o: /usr/include/curses.h /usr/include/stddef.h /usr/include/termios.h
game.o: /usr/include/va_list.h /usr/include/stdarg.h /usr/include/time.h
game.o: player.h globals.h
player.o: game.h /usr/include/assert.h /usr/include/standards.h
player.o: /usr/include/stdio.h sockcom.h sockaddr.h /usr/include/netinet/in.h
player.o: /usr/include/sys/types.h /usr/include/errno.h /usr/include/fcntl.h
player.o: /usr/include/sys/fcntl.h /usr/include/netdb.h /usr/include/pwd.h
player.o: /usr/include/signal.h /usr/include/sys/siginfo.h
player.o: /usr/include/sys/sysmisc.h /usr/include/machine/signal.h
player.o: /usr/include/stdlib.h /usr/include/string.h
player.o: /usr/include/sys/socket.h /usr/include/sys/uio.h
player.o: /usr/include/sys/time.h /usr/include/sys/select.h
player.o: /usr/include/unistd.h /usr/include/sys/access.h String.h
player.o: /usr/include/ctype.h gamec.h /usr/include/curses.h
player.o: /usr/include/stddef.h /usr/include/termios.h /usr/include/va_list.h
player.o: /usr/include/stdarg.h /usr/include/time.h player.h globals.h
db.o: game.h /usr/include/assert.h /usr/include/standards.h
db.o: /usr/include/stdio.h sockcom.h sockaddr.h /usr/include/netinet/in.h
db.o: /usr/include/sys/types.h /usr/include/errno.h /usr/include/fcntl.h
db.o: /usr/include/sys/fcntl.h /usr/include/netdb.h /usr/include/pwd.h
db.o: /usr/include/signal.h /usr/include/sys/siginfo.h
db.o: /usr/include/sys/sysmisc.h /usr/include/machine/signal.h
db.o: /usr/include/stdlib.h /usr/include/string.h /usr/include/sys/socket.h
db.o: /usr/include/sys/uio.h /usr/include/sys/time.h
db.o: /usr/include/sys/select.h /usr/include/unistd.h
db.o: /usr/include/sys/access.h String.h /usr/include/ctype.h gamec.h
db.o: /usr/include/curses.h /usr/include/stddef.h /usr/include/termios.h
db.o: /usr/include/va_list.h /usr/include/stdarg.h /usr/include/time.h
db.o: player.h globals.h
die.o: game.h /usr/include/assert.h /usr/include/standards.h
die.o: /usr/include/stdio.h sockcom.h sockaddr.h /usr/include/netinet/in.h
die.o: /usr/include/sys/types.h /usr/include/errno.h /usr/include/fcntl.h
die.o: /usr/include/sys/fcntl.h /usr/include/netdb.h /usr/include/pwd.h
die.o: /usr/include/signal.h /usr/include/sys/siginfo.h
die.o: /usr/include/sys/sysmisc.h /usr/include/machine/signal.h
die.o: /usr/include/stdlib.h /usr/include/string.h /usr/include/sys/socket.h
die.o: /usr/include/sys/uio.h /usr/include/sys/time.h
die.o: /usr/include/sys/select.h /usr/include/unistd.h
die.o: /usr/include/sys/access.h String.h /usr/include/ctype.h gamec.h
die.o: /usr/include/curses.h /usr/include/stddef.h /usr/include/termios.h
die.o: /usr/include/va_list.h /usr/include/stdarg.h /usr/include/time.h
die.o: player.h globals.h
gamec.o: sockcom.h sockaddr.h /usr/include/netinet/in.h
gamec.o: /usr/include/sys/types.h /usr/include/standards.h
gamec.o: /usr/include/stdio.h /usr/include/errno.h /usr/include/fcntl.h
gamec.o: /usr/include/sys/fcntl.h /usr/include/netdb.h /usr/include/pwd.h
gamec.o: /usr/include/signal.h /usr/include/sys/siginfo.h
gamec.o: /usr/include/sys/sysmisc.h /usr/include/machine/signal.h
gamec.o: /usr/include/stdlib.h /usr/include/string.h
gamec.o: /usr/include/sys/socket.h /usr/include/sys/uio.h
gamec.o: /usr/include/sys/time.h /usr/include/sys/select.h
gamec.o: /usr/include/unistd.h /usr/include/sys/access.h String.h
gamec.o: /usr/include/ctype.h gamec.h /usr/include/assert.h
String.o: /usr/include/string.h /usr/include/standards.h
String.o: /usr/include/sys/types.h String.h /usr/include/stdio.h
sockaddr.o: /usr/include/stdio.h /usr/include/standards.h
sockaddr.o: /usr/include/string.h /usr/include/sys/types.h
sockaddr.o: /usr/include/netinet/in.h /usr/include/netdb.h
sockaddr.o: /usr/include/sys/socket.h /usr/include/sys/uio.h sockaddr.h
sockaddr.o: /usr/include/errno.h /usr/include/fcntl.h
sockaddr.o: /usr/include/sys/fcntl.h /usr/include/pwd.h /usr/include/signal.h
sockaddr.o: /usr/include/sys/siginfo.h /usr/include/sys/sysmisc.h
sockaddr.o: /usr/include/machine/signal.h /usr/include/stdlib.h
sockaddr.o: /usr/include/sys/time.h /usr/include/sys/select.h
sockaddr.o: /usr/include/unistd.h /usr/include/sys/access.h
sockcom.o: sockcom.h sockaddr.h /usr/include/netinet/in.h
sockcom.o: /usr/include/sys/types.h /usr/include/standards.h
sockcom.o: /usr/include/stdio.h /usr/include/errno.h /usr/include/fcntl.h
sockcom.o: /usr/include/sys/fcntl.h /usr/include/netdb.h /usr/include/pwd.h
sockcom.o: /usr/include/signal.h /usr/include/sys/siginfo.h
sockcom.o: /usr/include/sys/sysmisc.h /usr/include/machine/signal.h
sockcom.o: /usr/include/stdlib.h /usr/include/string.h
sockcom.o: /usr/include/sys/socket.h /usr/include/sys/uio.h
sockcom.o: /usr/include/sys/time.h /usr/include/sys/select.h
sockcom.o: /usr/include/unistd.h /usr/include/sys/access.h String.h
sockcom.o: /usr/include/ctype.h
