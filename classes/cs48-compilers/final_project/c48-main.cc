/* main-test.cc
** Nate Edel, 14 Apr 1999
**        mod 27 Apr 1999
**
** contains a simple main function to test yyparse();
*/

#include <stdio.h>
#include "astNode.h"
#include "idTab.h"
#include "label.h"

extern "C" int yylex(void);
extern "C" char *yytext;
extern "C" FILE *yyin;
extern int yyparse(void);
extern "C" int yydebug;
int global_var_sz;

extern bool Eval_Global(pAstNode global);
extern bool CGen_Global(pAstNode global);

int num_nodes = 0;

bool      c48GDebug = false;
bool      c48SDebug = false;
bool      c48CDebug = false;
pAstNode root = NULL;

// syntax_tree sTree;

int main(int argc, char **argv) { 
    yydebug = 0;
		yyin = fopen(argv[1],"r");
	  yyparse();
    if (Eval_Global(root)==false) {
      fprintf(stderr, "Semantic error\n");
      getchar();
      }
    if (CGen_Global(root)==false) {
      fprintf(stderr, "Code generation error\n");
      getchar();
      }
    printf("* %s\n*\n",argv[1]);
    dump_asm();
    dumpTable();
    dumpLTable();
    root->Print(0);
    }
