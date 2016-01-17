%{
#include <stdio.h>
#include "Calc.tab.h"
%}

%%

[ \n\t]+          ;
[0-9][0-9]* { sscanf(yytext, "%d", &yylval); return NUM; }
"+"         return '+';
"-"         return '-';
"*"         return '*';
"("         return '(';
")"         return ')';
<<EOF>>     return 0;

%%

