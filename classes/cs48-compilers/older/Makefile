# This Makefile a distant descendant of one provided by Fred Henle to 
# the W96 CS23 class...

CFLAGS = -g -ggdb 
# -Wall 
LFLAGS = -ggdb
DEFINES  = 
CC =	gcc
CPP =   g++
LD =	g++
MV =	mv
RM =	rm -f
CP =	cp
LIBS =	-lfl
LEX  =  flex  
BISON =  bison
BFLAGS = -d -v -t

# Files ------------------------

SRCS = c48-main.cc error.c parse.y lexer.l TreeNode.cc
OBJS = c48-main.o error.o parse.tab.o lexer.o TreeNode.o

# Build Targets ----------------

PROGRAMS = c48

all:	$(PROGRAMS)

%.o:	%.c
	$(CC) -c $(CFLAGS) $(DEFINES) $(@:.o=.c)

%.o:	%.cc
	$(CPP) -c $(CFLAGS) $(DEFINES) $(@:.o=.cc)

%.tab.cc: %.y
	$(BISON) $(BFLAGS) $(@:.tab.cc=.y)
	$(MV) $(@:.tab.cc=.tab.c) $@

%.c: %.l
	$(LEX) -o$@ $(@:.c=.l)

c48: $(OBJS)
	$(RM) $@
	$(LD) $(LFLAGS) -o $@ $(OBJS) $(LIBS)

# Install Targets --------------

# Utility Targets --------------

clean:
	$(RM) $(PROGRAMS)
	$(RM) *.o
	$(RM) core
	$(RM) parse.tab.h parse.tab.cc
	$(RM) *~

depend:

	makedepend -s "# DO NOT DELETE" -- $(SRCS)

# Dependencies -----------------
# DO NOT DELETE THIS LINE -- make depend depends on it.

c48-main.o: /usr/include/stdio.h /usr/include/features.h
c48-main.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
c48-main.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
c48-main.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
c48-main.o: /usr/include/bits/types.h /usr/include/libio.h
c48-main.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
error.o: /usr/include/stdio.h /usr/include/features.h
error.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
error.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
error.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
error.o: /usr/include/bits/types.h /usr/include/libio.h
error.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
parse.o: /usr/include/stdlib.h /usr/include/features.h
parse.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
parse.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
parse.o: /usr/include/sys/types.h /usr/include/bits/types.h
parse.o: /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h
parse.o: /usr/include/sys/select.h /usr/include/bits/select.h
parse.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
parse.o: /usr/include/alloca.h /usr/include/string.h /usr/include/stdio.h
parse.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
parse.o: /usr/include/libio.h /usr/include/_G_config.h
parse.o: /usr/include/bits/stdio_lim.h
lexer.o: lexer.h
