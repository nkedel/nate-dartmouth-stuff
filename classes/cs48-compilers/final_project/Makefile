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

SRCS = c48-main.cc error.c parse.y lexer.l astNode.cc idTab.c symTab.cc semant.cc label.cc code.cc
OBJS = c48-main.o error.o parse.tab.o lexer.o astNode.o idTab.o symTab.o semant.o label.o code.o

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
	$(RM) parse.tab.h parse.tab.cc parse.tab.c parse.output
	$(RM) Makefile.bak
	$(RM) *~

depend:

	makedepend -s "# DO NOT DELETE" -- $(SRCS)

# Dependencies -----------------
# DO NOT DELETE THIS LINE -- make depend depends on it.

c48-main.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/features.h
c48-main.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
c48-main.o: /usr/include/_G_config.h /usr/include/gnu/types.h
c48-main.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
c48-main.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
c48-main.o: /usr/include/stdio_lim.h astNode.h idTab.h /usr/include/stdlib.h
c48-main.o: /usr/include/sys/types.h /usr/include/time.h
c48-main.o: /usr/include/endian.h /usr/include/bytesex.h
c48-main.o: /usr/include/sys/select.h /usr/include/selectbits.h
c48-main.o: /usr/include/alloca.h label.h
error.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/features.h
error.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
error.o: /usr/include/_G_config.h /usr/include/gnu/types.h
error.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
error.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
error.o: /usr/include/stdio_lim.h
parse.o: /usr/include/stdlib.h /usr/include/features.h
parse.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
parse.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
parse.o: /usr/include/sys/types.h /usr/include/gnu/types.h
parse.o: /usr/include/time.h /usr/include/endian.h /usr/include/bytesex.h
parse.o: /usr/include/sys/select.h /usr/include/selectbits.h
parse.o: /usr/include/alloca.h /usr/include/string.h /usr/include/stdio.h
parse.o: /usr/include/libio.h /usr/include/_G_config.h
parse.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
parse.o: /usr/include/stdio_lim.h astNode.h idTab.h
lexer.o: lexer.h idTab.h /usr/include/stdlib.h /usr/include/features.h
lexer.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
lexer.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
lexer.o: /usr/include/sys/types.h /usr/include/gnu/types.h
lexer.o: /usr/include/time.h /usr/include/endian.h /usr/include/bytesex.h
lexer.o: /usr/include/sys/select.h /usr/include/selectbits.h
lexer.o: /usr/include/alloca.h
astNode.o: /usr/include/stdlib.h /usr/include/features.h
astNode.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
astNode.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
astNode.o: /usr/include/sys/types.h /usr/include/gnu/types.h
astNode.o: /usr/include/time.h /usr/include/endian.h /usr/include/bytesex.h
astNode.o: /usr/include/sys/select.h /usr/include/selectbits.h
astNode.o: /usr/include/alloca.h /usr/include/stdio.h /usr/include/libio.h
astNode.o: /usr/include/_G_config.h
astNode.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
astNode.o: /usr/include/stdio_lim.h /usr/include/string.h astNode.h idTab.h
idTab.o: /usr/include/stdlib.h /usr/include/features.h
idTab.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
idTab.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
idTab.o: /usr/include/sys/types.h /usr/include/gnu/types.h
idTab.o: /usr/include/time.h /usr/include/endian.h /usr/include/bytesex.h
idTab.o: /usr/include/sys/select.h /usr/include/selectbits.h
idTab.o: /usr/include/alloca.h idTab.h
symTab.o: /usr/include/stdlib.h /usr/include/features.h
symTab.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
symTab.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
symTab.o: /usr/include/sys/types.h /usr/include/gnu/types.h
symTab.o: /usr/include/time.h /usr/include/endian.h /usr/include/bytesex.h
symTab.o: /usr/include/sys/select.h /usr/include/selectbits.h
symTab.o: /usr/include/alloca.h /usr/include/string.h /usr/include/stdio.h
symTab.o: /usr/include/libio.h /usr/include/_G_config.h
symTab.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
symTab.o: /usr/include/stdio_lim.h symTab.h
semant.o: /usr/include/stdlib.h /usr/include/features.h
semant.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
semant.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
semant.o: /usr/include/sys/types.h /usr/include/gnu/types.h
semant.o: /usr/include/time.h /usr/include/endian.h /usr/include/bytesex.h
semant.o: /usr/include/sys/select.h /usr/include/selectbits.h
semant.o: /usr/include/alloca.h /usr/include/string.h /usr/include/stdio.h
semant.o: /usr/include/libio.h /usr/include/_G_config.h
semant.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
semant.o: /usr/include/stdio_lim.h astNode.h idTab.h symTab.h
label.o: /usr/include/stdlib.h /usr/include/features.h
label.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
label.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
label.o: /usr/include/sys/types.h /usr/include/gnu/types.h
label.o: /usr/include/time.h /usr/include/endian.h /usr/include/bytesex.h
label.o: /usr/include/sys/select.h /usr/include/selectbits.h
label.o: /usr/include/alloca.h /usr/include/stdio.h /usr/include/libio.h
label.o: /usr/include/_G_config.h
label.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
label.o: /usr/include/stdio_lim.h /usr/include/string.h label.h
code.o: /usr/include/stdlib.h /usr/include/features.h
code.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
code.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stddef.h
code.o: /usr/include/sys/types.h /usr/include/gnu/types.h /usr/include/time.h
code.o: /usr/include/endian.h /usr/include/bytesex.h
code.o: /usr/include/sys/select.h /usr/include/selectbits.h
code.o: /usr/include/alloca.h /usr/include/string.h /usr/include/stdio.h
code.o: /usr/include/libio.h /usr/include/_G_config.h
code.o: /usr/lib/gcc-lib/i386-redhat-linux/2.7.2.3/include/stdarg.h
code.o: /usr/include/stdio_lim.h astNode.h idTab.h symTab.h label.h
