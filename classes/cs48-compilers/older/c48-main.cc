/* main-test.cc
** Nate Edel, 14 Apr 1999
**        mod 27 Apr 1999
**
** contains a simple main function to test yyparse();
*/

#include <stdio.h>
#include "TreeNode.h"

extern "C" int yylex(void);
extern "C" char *yytext;
extern "C" FILE *yyin;
extern int yyparse(void);
extern "C" int yydebug;

bool      c48Debug = false;
pTreeNode root;

// syntax_tree sTree;

int main(int argc, char **argv) { 
    if (argc==4) yydebug = 1;
		else yydebug=0;
    if (argc>2) c48Debug = true;
	if (argc>=2) yyin = fopen(argv[1],"r");
		else yyin = stdin;
	yyparse();
//    root->Print(0);
     }
