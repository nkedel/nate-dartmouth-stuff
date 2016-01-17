@echo off
del *.o
del parse_tab.*
del parse.tab.*
del parser.exe
bison -d -v -t parse.y
move parse_tab.c parse.cc
move parse_tab.h parsetab.h
flex -olexer.c lexer.l
gcc -c -g -ggdb -Wall error.c
gxx -c -g -ggdb -Wall c48-main.cc
gxx -c -g -ggdb -Wall treenode.cc
gxx -c -g -ggdb -Wall parse.cc
gcc -c -g -ggdb -Wall lexer.c
gxx -ggdb -Wall -oparser.exe c48-main.o treenode.o parse.o error.o lexer.o -lfl 
