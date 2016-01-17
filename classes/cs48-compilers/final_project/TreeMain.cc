#include <stdlib.h>
#include <stdio.h>
#include "TreeNode.h"

TreeNodePtr root = NULL;

extern int yyparse();
extern int yydebug;
bool parseError = false;

int main()
{
  int haveRoot = 0;		// 0 means we will have a root

  yydebug = 0;
  haveRoot = yyparse();

  if (parseError)
    fprintf(stderr, "WARNING: There were parse errors.\nParse tree may be ill-formed.\n");

  if (haveRoot == 0)
    root->Print(0);

  return 0;
}
