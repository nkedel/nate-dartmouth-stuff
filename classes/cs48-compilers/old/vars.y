%{
#include <stdio.h>

int yylex();
extern char *yytext;
int yyerror(char *s);

static int curtype;

%}

%token INT FLOAT ID

%%

decl : type { curtype = $1; } vars ';'
;

type : INT { $$ = 1; }
| FLOAT { $$ = 2; }
;

vars : vars ',' ID { printf("%s %d\n", yytext, curtype); }
| ID { printf("%s %d\n", yytext, curtype); }
;

%%

int yyerror(char *s)
{
  fprintf(stderr, "%s\n", s);
  return 0;
}

int main()
{
  yyparse();
  return 0;
}

