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
extern bool parseError;

static TreeNodePtr savedId;

%}

%token NUM ID BADTOKEN IF ELSE 
%left '='
%left '+' '-'
%left '*'
%left UMINUS

%expect 2

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
| ifStmt        { $$ = $1; }
;

exprStmt : expr ';' { $$ = $1; }
| ';'               { $$ = NULL; }
| error ';'         { $$ = NULL; }
;

compoundStmt : '{' stmtList '}' { $$ = $2; }
;

ifStmt : IF '(' expr ')' stmt { $$ = new TreeNode(IF_STMT);
                                $$->AddChild($3, 0);
				$$->AddChild($5, 1); }
| IF '(' expr ')' stmt ELSE stmt { $$ = new TreeNode(IF_ELSE_STMT);
                                   $$->AddChild($3, 0);
				   $$->AddChild($5, 1);
				   $$->AddChild($7, 2); }
| IF '(' error ')' stmt { $$ = NULL; }
| IF '(' error ')' stmt ELSE stmt { $$ = NULL; }
;

expr : ID { savedId = new TreeNode(ID_NODE); } '=' expr
                { $$ = new TreeNode(OP_ASSN);
                  $$->AddChild(savedId, 0);
                  $$->AddChild($4, 1); }
| expr '+' expr { $$ = new TreeNode(OP_ADD);
                  $$->AddChild($1, 0);
                  $$->AddChild($3, 1); }
| expr '-' expr { $$ = new TreeNode(OP_SUB);
                  $$->AddChild($1, 0);
                  $$->AddChild($3, 1); }
| expr '*' expr { $$ = new TreeNode(OP_MULT);
                  $$->AddChild($1, 0);
                  $$->AddChild($3, 1); }
| '-' expr %prec UMINUS
                { $$ = new TreeNode(OP_NEG);
		  $$->AddChild($2, 0); }
| '(' expr ')'  { $$ = $2; }
| NUM           { $$ = new TreeNode(NUM_NODE); }
| ID            { $$ =  new TreeNode(ID_NODE); }
| '(' error ')' { $$ = NULL; }
;

%%

int yyerror(char *s)
{
  parseError = true;
  fprintf(stderr, "%s at line %d\n", s, lineNumber);
  return 0;
}
