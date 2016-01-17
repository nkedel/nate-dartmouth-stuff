// TreeNode.cc
// Implementation of TreeNode class

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "TreeNode.h"
extern char *yytext;

TreeNode::TreeNode(TreeNodeType t)
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
      children.left = NULL;
      children.right = NULL;
      break;

    case NUM_NODE:
      value = atoi(yytext);
      break;

    case ID_NODE:
      idName = strdup(yytext);
      break;

    default:
      fprintf(stderr, "Bad node type in TreeNode constructor: %d\n", type);
      break;
    }
}

void TreeNode::AddLeft(TreeNodePtr x)
{
  children.left = x;
}
   
void TreeNode::AddRight(TreeNodePtr x)
{
  children.right = x;
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
    "ASSN", "ADD", "SUB", "MULT", "NEG", "NUM", "ID" };

  // Indent according to the level.
  for (int i = 0; i < level; i++)
    printf("  ");

  // Print the type.
  if (type >= OP_ASSN && type <= ID_NODE)
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
      if (children.left != NULL)
	children.left->Print(level+1);
      if (children.right != NULL)
	children.right->Print(level+1);
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
