// TreeNode.cc
// Implementation of TreeNode class

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "TreeNode.h"
extern char *yytext;

TreeNode::TreeNode(TreeNodeType t, char *s)
{
  type = t;
  sibling = NULL;

  switch (type)
    {
    case OP_ASSN:
    case OP_ADD:
    case OP_SUB:
    case OP_MULT:
    case OP_NEG:
    case IF_STMT:
    case IF_ELSE_STMT:
      for (int j = 0; j < MAXTREENODEKIDS; j++)
	child[j] = NULL;
      break;

    case NUM_NODE:
      value = atoi(s);
      break;

    case ID_NODE:
      idName = strdup(s);
      break;

    default:
      fprintf(stderr, "Bad node type in TreeNode constructor: %d\n", type);
      break;
    }
}

void TreeNode::AddChild(TreeNodePtr x, int j)
{
  if (j >= 0 && j < MAXTREENODEKIDS)
    child[j] = x;
  else
    fprintf(stderr, "Bad TreeNode::AddChild call: j = %d\n", j);
}

void TreeNode::AddSibling(TreeNodePtr x)
{
  sibling = x;
}

TreeNodePtr TreeNode::GetSibling()
{
  return sibling;
}

void TreeNode::Print(int level)
{
  static char *types[] = {
    "ASSN", "ADD", "SUB", "MULT", "NEG",
    "NUM", "ID", "IF-STMT", "IF-ELSE-STMT" };

  static char *ifParts[] = {"Expr", "Then", "Else"};

  // Indent according to the level.
  for (int i = 0; i < level; i++)
    printf("  ");

  // Print the type.
  if (type >= OP_ASSN && type <= IF_ELSE_STMT)
    printf("%s", types[type]);
  else
    printf("bad type!!");

  // Now do the type-specific stuff.
  switch (type)
    {
    case OP_ASSN:
    case OP_ADD:
    case OP_SUB:
    case OP_MULT:
    case OP_NEG:
      printf("\n");
      // Recurse on the children.
      for (int j = 0; j < MAXTREENODEKIDS; j++)
	if (child[j] != NULL)
	  child[j]->Print(level+1);
      break;

    case IF_STMT:
    case IF_ELSE_STMT:
      printf("\n");
      for (int j = 0; j < MAXTREENODEKIDS; j++)
	{
	  if (child[j] != NULL)
	    {
	      for (int i = 0; i < level+1; i++)
		printf("  ");
	      printf("%s:\n", ifParts[j]);
	      child[j]->Print(level+2);
	    }
	}
      break;

    case NUM_NODE:
      // Print the numeric value.
      printf(": %d\n", value);
      break;

    case ID_NODE:
      // Print the id name.
      printf(": %s\n", idName);
      break;

    default:
      break;
    }

  // Print the next sibling, if there is one.
  if (sibling != NULL)
    sibling->Print(level);
}
