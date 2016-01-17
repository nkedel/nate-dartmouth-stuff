// TreeNode.cc
// Implementation of TreeNode class
// originally provided by Prof. Cormen
// modified by N. Edel

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "TreeNode.h"
#include "parsetab.h"

// note that these are for debugging only
// and will need to be manually corrected if tokens are added to or
// removed from the grammer 
#define BASE 258
const char *t_text[] = {
 "INT", "VOID", "DOUBLE", "CHAR", "ID", "NUM", "FNUM", "IF", "ELSE",
 "FOR", "DO", "WHILE", "RETURN", "L_OR", "L_AND", "NEQ", "CEQ", "LTE", "GTE",
 "UMINUS", "DECR", "INCR", "SUB", "CALL" };


extern char *yytext;
extern bool c48Debug;
extern "C" int lines;

void DMESG(int t, const char *m) {
    if (c48Debug) {
       char foo[16];
       if (t > BASE) strcpy(foo, t_text[t-BASE]);
       else { foo[0] = (char) t; foo[1] = 0; }
       printf("Ln#%d: %s ('%s')", lines, foo, m);
       }
    }

List::List() {
  head = NULL;
  }

//List::~List {
//  while (head != NULL) {
//    pNode temp;
//    temp = head;
//    head = head->next;
//    delete(head);
//    }
//  }

void List::append(pTreeNode x) {
  pNode temp;
  temp = head;
  while (temp != NULL)
     temp = temp->next;
  temp = new(ListNode);
  temp->next  = NULL;
  temp->child = x;
  }

pTreeNode List::get(int n) {
  pNode temp;
  int i;
  temp = head;
  i = 0;
  while (i != n && temp != NULL) {
    i++;
    temp = temp->next;
    }
  if (i == n) return(temp->child);
  return NULL;
  }

int List::len(void) {
  pNode temp;
  int i;
  temp = head;
  i = 0;
  while (temp != NULL) {
    i++;
    temp = temp->next;
    }
  return i;
  }


TreeNode::TreeNode(int type, char *s)
{
  sib = NULL;
  kids = new(List);
  val = strdup(s);
}

void TreeNode::addChild(pTreeNode x) {
  kids->append(x);
}

void TreeNode::addSib(pTreeNode x)
{
  sib = x;
}

pTreeNode TreeNode::getSib()
{
  return sib;
}

void TreeNode::Print(int level)
{
  // Indent according to the level.
  for (int i = 0; i < level; i++)
    printf("  ");

  // Print the type.
  if (type < BASE) {
    char foo[3] = " ";   // this is a kluge til I can look up %c?
    foo[0] = (char) type; 
    printf("'%s' (%s)\n", foo, val );
    }
  else 
    printf("%s (%s)\n", t_text[type-BASE], val);

  for (int j = 0; j < kids->len(); j++)
          (kids->get(j))->Print(level+1);

  if (sib != NULL)
    sib->Print(level);
}
