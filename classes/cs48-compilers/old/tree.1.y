%{
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "TreeNode.h"

#define YYSTYPE TreeNodePtr
#define YYDEBUG 1

extern "C" int yylex();
int yyerror(char *s);

extern int lineNumber;
extern TreeNodePtr root;

%}

%token NUM ID BADTOKEN
%left '='
%left '+' '-'
%left '*'
%left UMINUS

%%

code : stmtList { root = $$ = $1; }
;

stmtList : /* empty */ { $$ = NULL; }
| stmtList stmt { TreeNodePtr t = $1;
                  if (t != NULL)
		    {
		      while (t->GetSibling() != NULL)
			t = t->GetSibling();
		      t->AddSibling($2);
		      $$ = $1;
		    }
		  else
		    $$ = $2;
                }
;

stmt : exprStmt { $$ = $1; }
| compoundStmt  { $$ = $1; }
;

exprStmt : expr ';' { $$ = $1; }
| ';'               { $$ = NULL; }
;

compoundStmt : '{' stmtList '}' { $$ = $2; }
;

expr :
  expr '=' expr { $$ = new TreeNode(OP_ASSN);
                  $$->AddLeft($1);
                  $$->AddRight($3); }
| expr '+' expr { $$ = new TreeNode(OP_ADD);
                  $$->AddLeft($1);
                  $$->AddRight($3); }
| expr '-' expr { $$ = new TreeNode(OP_SUB);
                  $$->AddLeft($1);
                  $$->AddRight($3); }
| expr '*' expr { $$ = new TreeNode(OP_MULT);
                  $$->AddLeft($1);
                  $$->AddRight($3); }
| '-' expr %prec UMINUS
                { $$ = new TreeNode(OP_NEG);
		  $$->AddLeft($2); }
| '(' expr ')'  { $$ = $2; }
| NUM           { $$ = new TreeNode(NUM_NODE); }
| ID            { $$ = new TreeNode(ID_NODE); }
;

%%

int yyerror(char *s)
{
  fprintf(stderr, "%s at line %d\n", s, lineNumber);
  return 0;
}
