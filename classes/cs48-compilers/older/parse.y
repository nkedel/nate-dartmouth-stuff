%{
// initial lib choices snarfed from class examples
// much of this initial code based on class "tree" examples  

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "TreeNode.h"

#define YYSTYPE pTreeNode
#define YYDEBUG 1

extern "C" int lines;
extern "C" int yylex();
extern "C" void yyerror(char *);
extern "C" char *yytext;
extern "C" char *lastLIT;
extern "C" char *lastID;

extern void DMESG(int t, const char *m);
extern pTreeNode root;
extern bool Error;
%}

%token INT
%token VOID
%token DOUBLE
%token CHAR
%token ID
%token NUM
%token FNUM
%token IF
%token ELSE
%token FOR
%token DO
%token WHILE
%token RETURN

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
%token  SUB
%token  CALL
%token  FN_DEF
%token  FN_DECL


/* expect one shift-reduce for the IF/ELSE */
%expect 1
%%
program : decl_list     {
    root = $$ = $1;
    DMESG(0, "program -> decl_list");
    }
;

decl_list : decl_list decl      {
        DMESG(0, "decl_list -> decl_list decl");
        pTreeNode tNode = $1;
        if (tNode == NULL) {
                while (tNode->getSib() != NULL)
                        tNode = tNode->getSib();
                tNode->addSib($2);
                $$ = $1;
                }
        else $$ = $2;
        }
| decl           {
    DMESG(0, "decl_list -> decl");
    $$ = $1;
    }
;

decl : var_decl  { DMESG(0, "decl -> var_decl"); $$ = $1; }
| fun_defn       { DMESG(0, "decl -> fun_defn"); $$ = $1; }
| fun_decl       { DMESG(0, "decl -> fun_decl"); $$ = $1; }
;

var_decl : type_spec id_list ';'        {
        DMESG(0, "var_decl -> type_spec id_list"); 
        $$ = $1;
        $$->addChild($2);
        }
;

type_spec: INT { $$ = new TreeNode(INT);   DMESG(INT,    "type_spec -> INT"); } 
| VOID         { $$ = new TreeNode(VOID);  DMESG(VOID,   "type_spec -> VOID"); }
| DOUBLE       { $$ = new TreeNode(DOUBLE);DMESG(DOUBLE, "type_spec -> DOUBLE"); }
| CHAR         { $$ = new TreeNode(CHAR);  DMESG(CHAR,   "type_spec -> CHAR"); }
;

id_list : id_list ',' id_decl {
        pTreeNode tNode = $1;
        DMESG(0, "id_list -> id_list, id_decl");
        if (tNode == NULL) {
                while (tNode->getSib() != NULL)
                        tNode = tNode->getSib();
                tNode->addSib($3);
                $$ = $1;
                }
        else $$ = $3;
        }
| id_decl  { $$ = $1; DMESG(0, "id_list -> id_decl"); }
;

id_decl : ID    {
            $$ = new TreeNode(ID, lastID);
            DMESG(0, "id_list -> id_list, id_decl");
            }
| ID '[' NUM ']' {
        DMESG(SUB, lastID);
        $$ = new TreeNode(SUB, lastID);
        $$->addChild(new TreeNode(NUM, lastLIT));
        }
;

fun_decl : type_spec ID '(' params ')' ';' { 
        DMESG(FN_DECL, lastID);
        $$ = new TreeNode(FN_DEF, lastID);
        $$ -> addChild($1);
        $$ -> addChild($4);
        }
;

fun_defn : type_spec ID '(' params ')' comp_stmt {
        DMESG(FN_DEF, lastID);
        $$ = new TreeNode(FN_DEF, lastID);
        $$ -> addChild($1);
        $$ -> addChild($4);
        $$ -> addChild($6);
        }
;

params : param_list {
    DMESG(0, "params->param_list");
    $$ = $1;
    }
| VOID {
    DMESG(VOID, "params -> VOID");
    $$ = new TreeNode(VOID);
    }
;

param_list : param_list ',' param { 
        DMESG(VOID, "param_list -> param_list, param");
        pTreeNode tNode = $1;
        if (tNode == NULL) {
                while (tNode->getSib() != NULL)
                        tNode = tNode->getSib();
                tNode->addSib($3);
                $$ = $1;
                }
        else $$ = $3;
        }
| param {
    DMESG(VOID, "param_list -> param");
    $$ = $1;
    }
;

param : type_spec ID {
        DMESG(VOID, "param -> type_spec ID");
        $$ = $1;
        $$->addChild(new TreeNode(ID, lastID));
        }
| type_spec ID '['']' {
        DMESG(VOID, "param -> type_spec ID[]");
        $$ = $1;
        $$->addChild(new TreeNode(SUB, lastID));
        }
;

comp_stmt : '{' local_decl stmt_list '}' {
        DMESG(0, "comp_stmt -> { local decl stmt_list }");
        if ( $1 == NULL ) $$ = $2;
        else {
             $$ = $1;
             $1->addSib($2);
             }
        }
;

local_decl : { DMESG(0, "local_decl -> /e/"); $$ = NULL } 
| local_decl var_decl {
        pTreeNode wNode = $1;
        DMESG(0, "local_decl -> local_decl var_decl");
        if (wNode != NULL) {
           while (wNode->getSib() !=NULL) wNode = wNode->getSib();
           wNode->addSib($2);
           $$ = $1;
           }
        else $$ = $2;
        }
;

stmt_list :  { DMESG(0, "stmt_list -> /e/"); $$ = NULL; }
| stmt_list statement {
        pTreeNode wNode = $1;
        DMESG(0, "stmt_list -> stmt_list statement");
        if (wNode != NULL) {
           while (wNode->getSib() !=NULL) wNode = wNode->getSib();
           wNode->addSib($2);
           $$ = $1;
           }
        else $$ = $2;
        }
; 

statement : expr_stmt { DMESG(0, "statement -> expr_stmt");   $$ = $1 } 
| comp_stmt           { DMESG(0, "statement -> comp_stmt");   $$ = $1 }
| if_stmt             { DMESG(0, "statement -> if_stmt");     $$ = $1 }
| for_stmt            { DMESG(0, "statement -> for_stmt");    $$ = $1 }
| do_stmt             { DMESG(0, "statement -> do_stmt");     $$ = $1 }
| while_stmt          { DMESG(0, "statement -> while_stmt");  $$ = $1 }
| return_stmt         { DMESG(0, "statement -> return_stmt"); $$ = $1 }
;

expr_stmt : expr ';' { DMESG(0, "expr_stmt -> expr"); $$ = $1;   }
| ';'                { DMESG(0, "expr_stmt -> /e/");  $$ = NULL; }
;

if_stmt : IF '(' expr ')' statement {
        DMESG(IF, "if_stmt -> IF (expr) statement");
        $$ = new TreeNode(IF);
        $$->addChild($3);
        $$->addChild($5);
        }
| IF '(' expr ')' statement ELSE statement {
        DMESG(ELSE, "if_stmt -> IF (expr) statement");
        $$ = new TreeNode(ELSE);
        $$->addChild($3);
        $$->addChild($5);
        $$->addChild($7);
        }
;

for_stmt : FOR '(' for_expr ';' for_expr ';' for_expr ')' statement {
        DMESG(FOR, "for_stmt -> FOR (expr;expr;expr) statement");
        $$ = new TreeNode(FOR);
        $$->addChild($3);
        $$->addChild($5);
        $$->addChild($7);
        $$->addChild($9);
        }
;

for_expr : /* empty */ {
    DMESG(0, "for_expr -> /e/");
    $$ = NULL; }
| expr                 {
    DMESG(0, "for_expr -> expr");
    $$ = $1;
    }
;

do_stmt : DO statement WHILE '(' expr ')' ';' {
        DMESG(DO, "do_stmt -> DO statement WHILE (expr)");
        $$ = new TreeNode(DO);
        $$->addChild($2);
        $$->addChild($5);
        }
;

while_stmt : WHILE '(' expr ')' statement {
        DMESG(WHILE, "while_stmt -> WHILE (expr) statement");
        $$ = new TreeNode(WHILE);
        $$->addChild($3);
        $$->addChild($5);
        }
;

/* doesn't straight C require parens around the expression on a return? */
return_stmt : RETURN ';' {
    DMESG(RETURN, "return_stmt -> RETURN");
    $$ = new TreeNode(RETURN);
    }
| RETURN expr ';'{
    DMESG(RETURN, "return_stmt -> RETURN expr");
    $$ = new TreeNode(RETURN);
    $$->addChild($3);
    }
;

var : ID                {
    DMESG(ID, lastID);
    $$ = new TreeNode(ID, lastID);
    }
| ID '[' expr ']'       {
    DMESG(SUB, lastID);
    $$ = new TreeNode(SUB, lastID);
    $$->addChild($3);
    }
;

expr : var '=' expr     {
    DMESG('=', "expr -> var = expr");
    $$ = new TreeNode('=');
    $$->addChild($1);
    $$->addChild($3);
    }
| exp   {
    $$ = $1;
    DMESG(0, "expr -> exp");
    }
;

exp: exp '+' exp {
        DMESG('+', "exp -> exp + exp");
        $$ = new TreeNode('+');
        $$->addChild($1);
        $$->addChild($3);
        }
| exp '-' exp {
        DMESG('-', "exp -> exp - exp");
        $$ = new TreeNode('-');
        $$->addChild($1);
        $$->addChild($3);
        }
| exp '*' exp {
        DMESG('*', "exp -> exp * exp");
        $$ = new TreeNode('*');
        $$->addChild($1);
        $$->addChild($3);
        }
| exp '/' exp {
        DMESG('/', "exp -> exp / exp");
        $$ = new TreeNode('/');
        $$->addChild($1);
        $$->addChild($3);
        }
| exp '%' exp {
        DMESG('%', "exp -> exp % exp");
        $$ = new TreeNode('%');
        $$->addChild($1);
        $$->addChild($3);
        }
| exp GTE exp {
        DMESG(GTE, "exp -> exp GTE exp");
        $$ = new TreeNode(GTE);
        $$->addChild($1);
        $$->addChild($3);
        }
| exp LTE exp {
        DMESG(LTE, "exp -> exp LTE exp");
        $$ = new TreeNode(LTE);
        $$->addChild($1);
        $$->addChild($3);
        }
| exp '<' exp {
        DMESG('<', "exp -> exp < exp");
        $$ = new TreeNode('<');
        $$->addChild($1);
        $$->addChild($3);
        }
| exp '>' exp {
        DMESG('>', "exp -> exp > exp");
        $$ = new TreeNode('>');
        $$->addChild($1);
        $$->addChild($3);
        }
| exp CEQ exp {
        DMESG(CEQ, "exp -> exp CEQ exp");
        $$ = new TreeNode(CEQ);
        $$->addChild($1);
        $$->addChild($3);
        }
| exp NEQ exp {
        DMESG(NEQ, "exp -> exp NEQ exp");
        $$ = new TreeNode(NEQ);
        $$->addChild($1);
        $$->addChild($3);
        }
| exp L_OR  exp {
        DMESG(L_OR, "exp -> exp L_OR exp");
        $$ = new TreeNode(L_OR);
        $$->addChild($1);
        $$->addChild($3);
        }
| exp L_AND exp {
        DMESG(L_AND, "exp -> exp L_AND exp");
        $$ = new TreeNode(L_AND);
        $$->addChild($1);
        $$->addChild($3);
        }
| '-' expval %prec UMINUS {
        DMESG(UMINUS, "exp -> -expval");
        $$ = new TreeNode(UMINUS);
        $$->addChild($1);
        }
| '!' expval  {
        DMESG('!', "exp -> !expval");
        $$ = new TreeNode('!');
        $$->addChild($1);
        }
| INCR var    {
        DMESG(INCR, "exp -> ++var");
        $$ = new TreeNode(INCR);
        $$->addChild($1);
        $$->addChild($2);
        }
| DECR var    {
        DMESG(DECR, "exp -> --var");
        $$ = new TreeNode(DECR);
        $$->addChild($1);
        }
| expval      { $$ = $1; DMESG(0, "exp -> expval"); } 

expval : '(' expr ')' { $$ = $2; DMESG(0, "expval -> (expr)"); }
| var    { $$ = $1; DMESG(0, "expval -> var"); }
| call   { $$ = $1; DMESG(0, "expval -> call"); }
| NUM    { $$ = new TreeNode(NUM, lastLIT);  DMESG(NUM, lastLIT);}
| FNUM   { $$ = new TreeNode(FNUM, lastLIT); DMESG(FNUM, lastLIT); }
;

call : ID '(' args ')' {
       DMESG(CALL, lastID);
       $$ = new TreeNode(CALL, lastID);
       if ($3 != NULL) $$->addChild($3);
       }
;

args : /* empty */ { $$ = NULL; DMESG(0, "args -> /e/"); }
| arg_list         { $$ = $1;   DMESG(0, "args -> arg_list"); }
;

arg_list : arg_list ',' expr  {
        DMESG(0, "arg_list -> arg_list, expr");
        pTreeNode tNode = $1;
        if (tNode == NULL) {
                while (tNode->getSib() != NULL)
                        tNode = tNode->getSib();
                tNode->addSib($3);
                $$ = $1;
                }
        else $$ = $3;
        }
| expr  {
    DMESG(0, "arg_list -> expr");
    $$ = $1;
    }
;

