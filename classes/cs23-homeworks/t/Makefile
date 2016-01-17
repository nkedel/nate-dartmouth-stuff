CXXFLAGS =	-g -gstabs -Wall -O2
#DEFINES =	-DPROFILE
#DEFINES =	-DDEBUG -DPROFILE
DEFINES =	
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
#XINCDIR =	-I/usr/include/tcl
XINCDIR =
XLIBDIR =	
THREADLIBS =	
XLIBS =		-ltk -ltcl -lm -lX11

# Files ------------------------

SRCS =	nate.cc ttt3D.cc
#SRCS = nate_anl.cc nate_ttt.cc nate_utl.cc nate_won.cc ttt3D.cc

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

