%{
#include <string.h>
#include "tree.tab.h"

int lineNumber = 1;

#define MAXTOKENLENGTH 201

char savedIdText[MAXTOKENLENGTH];
char savedLiteralText[MAXTOKENLENGTH];

%}

%%

"/""*"          {
                  int prevChar = '\0', thisChar;
		  for ( ; ; )
		    {
		      if ((thisChar = input()) == EOF)
			{
			  fprintf(stderr, "EOF in comment\n");
			  return 0;
			}
		      else if (thisChar == '/' && prevChar == '*')
			break;
		      else
			{
			  if (thisChar == '\n') lineNumber++;
			  prevChar = thisChar;
			}
		    }
                }

[ \t]+      ;
\n          { lineNumber++; }
if          return IF;
else        return ELSE;
[A-Za-z_][A-Za-z0-9_]*   { strncpy(savedIdText, yytext, MAXTOKENLENGTH-1);
                           return ID; }
[0-9]+                   { strncpy(savedLiteralText, yytext, MAXTOKENLENGTH-1);
                           return NUM; }
"="         return '=';
"+"         return '+';
"-"         return '-';
"*"         return '*';
"("         return '(';
")"         return ')';
";"         return ';';
"{"         return '{';
"}"         return '}';
<<EOF>>     return 0;
.           return BADTOKEN;
