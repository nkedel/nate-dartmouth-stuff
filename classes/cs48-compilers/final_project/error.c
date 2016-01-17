/* yyerror must be in C -- callback from bison.simple lib... */

#include <stdio.h>

extern int lines;

void yyerror(char *s) {
        printf("Syntax Error (line %d): %s",lines,s);
        }
                  