/* main-test.c
** Nate Edel, 14 Apr 1999
**
** contains a simple main function to test yylex();
*/

#include <stdio.h>
#include "parse.tab.h"

extern int yylex();
extern char *yytext;
extern FILE *yyin;
extern lines;

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
		if (token > 255) printf("(%d) %d: \"%s\"\n\n",lines,token,yytext);
			else     printf("(%d) \'%d: \"%s\"\n\n",lines,token,yytext);
		}
     }
