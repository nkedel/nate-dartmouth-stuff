/* main-test.c
** Nate Edel, 14 Apr 1999
**
** contains a simple main function to test yylex();
*/

#include <stdio.h>
#include "tokens.h"

const char *token_text[] = { "INVALID", "IDENT", "INTEGER", "FLOAT", "IF",
	"ELSE", "INT", "VOID", "CHAR", "DOUBLE", "RETURN", "WHILE", "DO", 
	"FOR", "LTOE", "GTOE", "CEQUAL", "NEQUAL", "INCR", "DECR", "LAND",
	"LOR" };

extern int yylex();
extern char *yytext;
extern FILE *yyin;

void main(int argc, char **argv) { 
	int token;
	printf("----------------\n");
	if (argc==2) yyin = fopen(argv[1],"r");
		else yyin = stdin;
	while(1) {
		token = yylex();
		if (token == 0) { 
			printf("End of file reached.\n");
			break;
			}
		if (token > 255) printf("%s: \"%s\"\n\n",token_text[token-BASE],yytext);
			else     printf("\'%c\': \"%s\"\n\n",token,yytext);
		}
     }
