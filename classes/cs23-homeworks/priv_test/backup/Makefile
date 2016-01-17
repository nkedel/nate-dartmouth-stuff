CXXFLAGS =	-g -gstabs -Wall -O2
#DEFINES =	-DPROFILE
DEFINES =	-DDEBUG -DPROFILE
#DEFINES =	
CC =	g++
LD =	g++
RM =	rm -f
CP =	cp
INSTALL =	install
PREFIX =	
BINDIR =	
MANDIR =	
MANEXT =	.1
INFODIR =	$(PREFIX)/info
XINCDIR =	-I/usr/include/tcl
XLIBDIR =	
THREADLIBS =	
XLIBS =		-ltk -ltcl -lm -lX11

# Files ------------------------

SRCS =	nate_anl.cc nate_ttt.cc nate_utl.cc nate_won.cc ttt3D.cc

OBJS =	$(SRCS:.cc=.o)

# Build Targets ----------------

PROGRAMS =	ttt3D

all::	$(PROGRAMS)

%.o:	%.cc
	$(CC) -c $(CXXFLAGS) $(DEFINES) $< $(XINCDIR)

ttt3D:	$(OBJS)
	$(RM) $@
	$(LD) $(CFLAGS) -o $@ $(OBJS) $(THREADLIBS) $(XLIBDIR) $(XLIBS)

# Install Targets --------------

# Utility Targets --------------

clean:
	$(RM) $(PROGRAMS)
	$(RM) $(OBJS)
	$(RM) core

depend:

	makedepend -s "# DO NOT DELETE" -- $(SRCS)

# Dependencies -----------------
# DO NOT DELETE

nate_anl.o: nate.h /usr/include/assert.h /usr/include/standards.h
nate_anl.o: /usr/include/stdlib.h /usr/include/unistd.h
nate_anl.o: /usr/include/sys/types.h /usr/include/sys/access.h
nate_anl.o: /usr/include/ctype.h /usr/include/time.h
nate_ttt.o: nate.h /usr/include/assert.h /usr/include/standards.h
nate_ttt.o: /usr/include/stdlib.h /usr/include/unistd.h
nate_ttt.o: /usr/include/sys/types.h /usr/include/sys/access.h
nate_ttt.o: /usr/include/ctype.h /usr/include/time.h
nate_utl.o: nate.h /usr/include/assert.h /usr/include/standards.h
nate_utl.o: /usr/include/stdlib.h /usr/include/unistd.h
nate_utl.o: /usr/include/sys/types.h /usr/include/sys/access.h
nate_utl.o: /usr/include/ctype.h /usr/include/time.h
nate_won.o: nate.h /usr/include/assert.h /usr/include/standards.h
nate_won.o: /usr/include/stdlib.h /usr/include/unistd.h
nate_won.o: /usr/include/sys/types.h /usr/include/sys/access.h
nate_won.o: /usr/include/ctype.h /usr/include/time.h
ttt3D.o: nate.h /usr/include/assert.h /usr/include/standards.h
ttt3D.o: /usr/include/stdlib.h /usr/include/unistd.h /usr/include/sys/types.h
ttt3D.o: /usr/include/sys/access.h /usr/include/ctype.h /usr/include/time.h
