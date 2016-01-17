%{
#include <stdio.h>
#include <ctype.h>

int yylex();
int yyerror(char *s);

#define YYDEBUG 1
%}

%token NUM

%%

command : exp { printf("%d\n", $1); }
;

exp : exp '+' term { $$ = $1 + $3; }
| exp '-' term { $$ = $1 - $3; }
| term { $$ = $1; }
;

term : term '*' factor { $$ = $1 * $3; }
| factor { $$ = $1; }
;

factor : NUM { $$ = $1; }
| '(' exp ')' { $$ = $2; }
;

%%

int main()
{
  extern int yydebug;
  yydebug = 1;
  yyparse();
  return 0;
}

int yyerror(char *s)
{
  fprintf(stderr, "%s\n", s);
  return 0;
}
