%{
// initial lib choices snarfed from class examples
// much of this initial code based on class "tree" examples  

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "astNode.h"
#include "idTab.h"

#define YYSTYPE pAstNode
#define YYDEBUG 1

extern "C" int lines;
extern "C" int yylex();
extern "C" void yyerror(char *);
extern "C" char *yytext;
extern "C" char lastLIT[128];
extern "C" pListNode lastID;

extern void DMESG(pAstNode curr, int t, const char *m);
extern pAstNode root;
extern bool Error;
// IDTable tIdent;
%}

%token INT VOID DOUBLE CHAR
%token ID NUM FNUM CNUM
%token IF ELSE FOR DO WHILE
%token RETURN READ WRITE BREAK CONTINUE

%left     L_OR
%left     L_AND
%left     NEQ CEQ
%left     LTE GTE '<' '>' 
%left     '+' '-'
%left     '*' '/' '%'
%nonassoc UMINUS '!'
%nonassoc DECR INCR

// pseudotokens for nonterminals sans directly representable tokens
// (function call, subscript)
%token  SUB CALL FN_DEF FN_DECL CSTMT

/* expect one shift-reduce for the IF/ELSE */
%expect 1
%%
program : decl_list     {
    root = $$ = $1;
    DMESG($$, 0, "program -> decl_list");
    }
;

decl_list : decl_list decl      {
    pAstNode tNode = $1;
    if (tNode == NULL) {
      $$ = $2;
      DMESG($$, 0, "decl_list -> NULL decl");
      }
	  else {
		  while (tNode->getSib() != NULL) tNode = tNode->getSib();
      tNode->addSib($2);
      $$ = $1;
      DMESG($$, 0, "decl_list -> decl_list decl");
      }
    }
| decl           {
    $$ = $1;
    DMESG($$, 0, "decl_list -> decl");
}
;

decl : var_decl  { $$ = $1; DMESG($$, 0, "decl -> var_decl"); }
| fun_defn       { $$ = $1; DMESG($$, 0, "decl -> fun_defn"); }
| fun_decl       { $$ = $1; DMESG($$, 0, "decl -> fun_decl"); }
;

var_decl : type_spec id_list ';'        {
        $$ = $1;
        $$->addChild($2);
        DMESG($$, 0, "var_decl -> type_spec id_list"); 
        }
;

type_spec: INT {  $$ = new astNode(INT);    DMESG($$, INT,    "type_spec -> INT"); } 
| VOID         {  $$ = new astNode(VOID);   DMESG($$, VOID,   "type_spec -> VOID"); }
| DOUBLE       {  $$ = new astNode(DOUBLE); DMESG($$, DOUBLE, "type_spec -> DOUBLE"); }
| CHAR         {  $$ = new astNode(CHAR);   DMESG($$, CHAR,   "type_spec -> CHAR"); }
;

id_list : id_list ',' id_decl {
        pAstNode tNode = $1;
        if (tNode == NULL) {
          $$ = $3;
          DMESG($$, 0, "id_list -> NULL id_decl");
          }
	      else {
		      while (tNode->getSib() != NULL) tNode = tNode->getSib();
          tNode->addSib($3);
          $$ = $1;
          DMESG($$, 0, "id_list -> id_list, id_decl");
          }
        }
| id_decl  {  $$ = $1;  DMESG($$, 0, "id_list -> id_decl"); }
;

id_decl : id    {
            $$ = $1;
            DMESG($$, 0, "id_list -> id_list, id_decl");
            }
| id '[' NUM ']' {
        $$ = $1;
        $$->resetNType(SUB);
        $$->addChild(new astNode(NUM, (int) atoi(lastLIT)));
        DMESG($$, SUB, getName($$->getID()));
        }
;

fun_decl : type_spec id '(' params ')' ';' { 
        $$ = $2;
        $$ -> resetNType(FN_DECL);
        $$ -> addChild($1);
        $$ -> addChild($4);
        DMESG($$, FN_DECL, getName($$->getID()));
        }
;

fun_defn : type_spec id '(' params ')' comp_stmt {
        $$ = $2;
        $$ -> resetNType(FN_DEF);
        $$ -> addChild($1);
        $$ -> addChild($4);
        $$ -> addChild($6);
        DMESG($$, FN_DEF, getName($$->getID()));
        }
;

params : param_list {
    $$ = $1;
    DMESG($$, 0, "params->param_list");
    }
| VOID {
    $$ = new astNode(VOID);
    DMESG($$, VOID, "params -> VOID");
    }
;

param_list : param_list ',' param { 
        pAstNode tNode = $1;
        if (tNode == NULL) {
          $$ = $3;
          DMESG($$, VOID, "param_list -> NULL, param");
          }
	      else {
		      while (tNode->getSib() != NULL) tNode = tNode->getSib();
          tNode->addSib($3);
          $$ = $1;
          DMESG($$, VOID, "param_list -> param_list , param");
          }
        }
| param {
    $$ = $1;
    DMESG($$, VOID, "param_list -> param");
    }
;

param : type_spec id {
        $$ = $1;
        $$->addChild($2);
        DMESG($$, VOID, "param -> type_spec ID");
        }
| type_spec id '['']' {
        $$ = $1;
        $$->addChild($2);
        $2->resetNType(SUB);
        DMESG($$, VOID, "param -> type_spec ID[]");
        }
;

comp_stmt : '{' local_decl stmt_list '}' {
        $$ = new astNode(CSTMT);
        $$->addChild($2);
        $$->addChild($3);
      	DMESG($$, 0, "comp_stmt -> { local decl stmt_list }");
        }
;

local_decl : { DMESG(NULL, 0, "local_decl -> /e/");  $$ = NULL  } 
| local_decl var_decl {
        pAstNode wNode = $1;
        if (wNode != NULL) {
           while (wNode->getSib() !=NULL) wNode = wNode->getSib();
           wNode->addSib($2);
           $$ = $1;
        DMESG($$, 0, "local_decl -> local_decl var_decl");
           }
        else $$ = $2;
        DMESG($$, 0, "local_decl -> /e/ var_decl");
        }
;

stmt_list :  { DMESG(NULL, 0, "stmt_list -> /e/");  $$ = NULL;  }
| stmt_list statement {
        pAstNode wNode = $1;
        if (wNode != NULL) {
           while (wNode->getSib() !=NULL) wNode = wNode->getSib();
           wNode->addSib($2);
           $$ = $1;
        DMESG($$, 0, "stmt_list -> stmt_list statement");
           }
        else $$ = $2;
        DMESG($$, 0, "stmt_list -> NULL statement");
        }
; 

statement : expr_stmt { $$ = $1; DMESG($$, 0, "statement -> expr_stmt");  } 
| comp_stmt           { $$ = $1; DMESG($$, 0, "statement -> comp_stmt");  }
| if_stmt             { $$ = $1; DMESG($$, 0, "statement -> if_stmt");    }
| for_stmt            { $$ = $1; DMESG($$, 0, "statement -> for_stmt");   }
| do_stmt             { $$ = $1; DMESG($$, 0, "statement -> do_stmt");    }
| while_stmt          { $$ = $1; DMESG($$, 0, "statement -> while_stmt"); }
| return_stmt         { $$ = $1; DMESG($$, 0, "statement -> return_stmt");} 
| read_stmt           { $$ = $1; DMESG($$, 0, "statement -> read_stmt");  } 
| write_stmt          { $$ = $1; DMESG($$, 0, "statement -> write_stmt"); } 
| break_stmt          { $$ = $1; DMESG($$, 0, "statement -> break_stmt"); }
| continue_stmt       { $$ = $1; DMESG($$, 0, "statement -> continue_stmt"); }
;

expr_stmt : expr ';' { $$ = $1;   DMESG($$, 0, "expr_stmt -> expr"); }
| ';'                { $$ = NULL; DMESG($$, 0, "expr_stmt -> /e/");  }
;

if_stmt : IF '(' expr ')' statement {
        $$ = new astNode(IF);
        $$->addChild($3);
        $$->addChild($5);
        DMESG($$, IF, "if_stmt -> IF (expr) statement");
        }
| IF '(' expr ')' statement ELSE statement {
        $$ = new astNode(ELSE);
        $$->addChild($3);
        $$->addChild($5);
        $$->addChild($7);
        DMESG($$, ELSE, "if_stmt -> IF (expr) statement");
        }
;

for_stmt : FOR '(' for_expr ';' for_expr ';' for_expr ')' statement {
        $$ = new astNode(FOR);
        $$->addChild($3);
        $$->addChild($5);
        $$->addChild($7);
        $$->addChild($9);
        DMESG($$, FOR, "for_stmt -> FOR (expr;expr;expr) statement");
        }
;

for_expr : /* empty */  {
    $$ = NULL; 
    DMESG($$, 0, "for_expr -> /e/");
    }
| expr                 {
    $$ = $1;
    DMESG($$, 0, "for_expr -> expr");
    }
;

do_stmt : DO statement WHILE '(' expr ')' ';' {
        $$ = new astNode(DO);
        $$->addChild($2);
        $$->addChild($5);
        DMESG($$, DO, "do_stmt -> DO statement WHILE (expr)");
        }
;

while_stmt : WHILE '(' expr ')' statement {
        $$ = new astNode(WHILE);
        $$->addChild($3);
        $$->addChild($5);
        DMESG($$, WHILE, "while_stmt -> WHILE (expr) statement");
        }
;

// doesn't straight C require parens around the expression on a return? 
return_stmt : RETURN ';' {
    $$ = new astNode(RETURN);
    DMESG($$, RETURN, "return_stmt -> RETURN");
    }
| RETURN expr ';'{
    $$ = new astNode(RETURN);
    $$->addChild($2);
    DMESG($$, RETURN, "return_stmt -> RETURN expr");
    }
;

read_stmt : READ var ';' {
    $$ = new astNode(READ);
    $$->addChild($2);
    DMESG($$, READ, "read_stmt -> READ var");
    }

write_stmt : WRITE expr ';' {
    $$ = new astNode(WRITE);
    $$->addChild($2);
    DMESG($$, WRITE, "write_stmt -> WRITE expr");
    }

break_stmt : BREAK ';' {
    $$ = new astNode(BREAK);
    DMESG($$, BREAK, "break_stmt -> BREAK");
    }

continue_stmt : CONTINUE ';' {
    $$ = new astNode(CONTINUE);
    DMESG($$, CONTINUE, "continue_stmt -> CONTINUE");
    }

var : id                {
    $$ = $1;
    DMESG($$, ID, getName($$->getID()));
    }
| ID '[' expr ']'       {
    $$ = $1;
    $$ -> resetNType(SUB);
    $$->addChild($3);
    DMESG($$, SUB, getName($$->getID()));
    }
;

expr : var '=' expr     {
    $$ = new astNode('=');
    $$->addChild($1);
    $$->addChild($3);
    DMESG($$, '=', "expr -> var = expr");
    }
| exp   {
    $$ = $1;
    DMESG($$, 0, "expr -> exp");
    }
;

exp: exp '+' exp {
        $$ = new astNode('+');
        $$->addChild($1);
        $$->addChild($3);
        DMESG($$, '+', "exp -> exp + exp");
        }
| exp '-' exp {
        $$ = new astNode('-');
        $$->addChild($1);
        $$->addChild($3);
        DMESG($$, '-', "exp -> exp - exp");
        }
| exp '*' exp {
        $$ = new astNode('*');
        $$->addChild($1);
        $$->addChild($3);
        DMESG($$, '*', "exp -> exp * exp");
        }
| exp '/' exp {
        $$ = new astNode('/');
        $$->addChild($1);
        $$->addChild($3);
        DMESG($$, '/', "exp -> exp / exp");
        }
| exp '%' exp {
        $$ = new astNode('%');
        $$->addChild($1);
        $$->addChild($3);
        DMESG($$, '%', "exp -> exp % exp");
        }
| exp GTE exp {
        $$ = new astNode(GTE);
        $$->addChild($1);
        $$->addChild($3);
        DMESG($$, GTE, "exp -> exp GTE exp");
        }
| exp LTE exp {
        $$ = new astNode(LTE);
        $$->addChild($1);
        $$->addChild($3);
        DMESG($$, LTE, "exp -> exp LTE exp");
        }
| exp '<' exp {
        $$ = new astNode('<');
        $$->addChild($1);
        $$->addChild($3);
        DMESG($$, '<', "exp -> exp < exp");
        }
| exp '>' exp {
        $$ = new astNode('>');
        $$->addChild($1);
        $$->addChild($3);
        DMESG($$, '>', "exp -> exp > exp");
        }
| exp CEQ exp {
        $$ = new astNode(CEQ);
        $$->addChild($1);
        $$->addChild($3);
        DMESG($$, CEQ, "exp -> exp CEQ exp");
        }
| exp NEQ exp {
        $$ = new astNode(NEQ);
        $$->addChild($1);
        $$->addChild($3);
        DMESG($$, NEQ, "exp -> exp NEQ exp");
        }
| exp L_OR  exp {
       $$ = new astNode(L_OR);
       $$->addChild($1);
       $$->addChild($3);
       DMESG($$, L_OR, "exp -> exp L_OR exp");
       }
| exp L_AND exp {
       $$ = new astNode(L_AND);
       $$->addChild($1);
       $$->addChild($3);
       DMESG($$, L_AND, "exp -> exp L_AND exp");
       }
| '-' expval %prec UMINUS {
       $$ = new astNode(UMINUS);
       $$->addChild($2);
       DMESG($$, UMINUS, "exp -> -expval");
       }
| '!' expval  {
        $$ = new astNode('!');
        $$->addChild($2);
        DMESG($$, '!', "exp -> !expval");
        }
| INCR var    {
        $$ = new astNode(INCR);
        $$->addChild($2);
        DMESG($$, INCR, "exp -> ++var");
        }
| DECR var    {
        $$ = new astNode(DECR);
        $$->addChild($2);
        DMESG($$, DECR, "exp -> --var");
        }
| expval      { $$ = $1; DMESG($$, 0, "exp -> expval"); } 

expval : '(' expr ')' {  
            $$ = $2;  DMESG($$, 0, "expval -> (expr)"); }
| var    {  $$ = $1;  DMESG($$, 0, "expval -> var");    }
| call   {  $$ = $1;  DMESG($$, 0, "expval -> call");   }
| CNUM   {  $$ = new astNode(CNUM, (int) atoi(lastLIT));    DMESG($$, CNUM, lastLIT); }
| NUM    {  $$ = new astNode(NUM, (int) atoi(lastLIT));     DMESG($$, NUM, lastLIT); }
| FNUM   {  $$ = new astNode(FNUM,(double) atof(lastLIT));  DMESG($$, FNUM, lastLIT); }
;

call : id '(' args ')' {
       $$ = $1;
       $$->resetNType(CALL);
       if ($3 != NULL) $$->addChild($3);
       DMESG($$, CALL, getName($$->getID()));
       }
;

args : /* empty */  {  $$ = NULL;  DMESG($$, 0, "args -> /e/"); }
| arg_list          {  $$ = $1;    DMESG($$, 0, "args -> arg_list"); }
;

arg_list : arg_list ',' expr  {
        pAstNode tNode = $1;
        if (tNode == NULL) {
          $$ = $3;
          DMESG($$, 0, "arg_list -> NULL expr");
          }
	      else {
		      while (tNode->getSib() != NULL) tNode = tNode->getSib();
          tNode->addSib($3);
          $$ = $1;
          DMESG($$, 0, "arg_list -> arg_list, expr");
          }
        }
| expr  {
    $$ = $1;
    DMESG($$, 0, "arg_list -> expr");
    }
;

id: ID {
  $$ = new astNode(ID, lastID);
  DMESG($$, ID, getName(lastID));
  }
;
