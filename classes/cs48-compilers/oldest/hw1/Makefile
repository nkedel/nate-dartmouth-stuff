# This Makefile a distant descendant of one provided by Fred Henle to 
# the W96 CS23 class...

CFLAGS = -g -ggdb 
# -Wall 
LFLAGS = -ggdb
DEFINES  = 
CC =	gcc
LD =	gcc
RM =	rm -f
CP =	cp
LIBS =	-lfl
LEX =   /usr/local/bin/flex  
# this is path reported by "which flex" on tahoe; change if necessary. 

# Files ------------------------

SRCS = sub.l
OBJS = $(SRCS:.l=.yy.o)

# Build Targets ----------------

PROGRAMS = sub

all:	$(PROGRAMS)

%.o:	%.c
	$(CC) -c $(CFLAGS) $(DEFINES) $(@:.o=.c)

%.yy.c: %.l
	$(LEX) -o$@ $(@:.yy.c=.l) 

sub: $(OBJS)
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

