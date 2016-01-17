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
LEX =   flex  

# Files ------------------------

SRCS = main-test.c
LSRCS = lexer.l
OBJS = $(SRCS:.c=.o) $(LSRCS:.l=.o)

# Build Targets ----------------

PROGRAMS = lexer

all:	$(PROGRAMS)

%.o:	%.c
	$(CC) -c $(CFLAGS) $(DEFINES) $(@:.o=.c)

%.c: %.l
	$(LEX) -o$@ $(@:.c=.l) 

lexer: $(OBJS)
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

