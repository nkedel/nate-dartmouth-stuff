%{
#include "tree.tab.h"

int lineNumber = 1;
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
[A-Za-z_][A-Za-z0-9_]*   return ID;
[0-9]+      return NUM;
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
