// ast.cc
// Implementation of abstract syntax tree class
// based on "TreeNode" class originally provided by Prof. Cormen
// modified by N. Edel

#define ASTNODE_CC
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "astNode.h"
#include "parse.tab.h"

// note that these are for debugging only
// and will need to be manually corrected if tokens are added to or
// removed from the grammer 
#define BASE 256
const char *dt_text[] = {"NONE", "VOID", "CHAR", "INT", "DOUBLE"};
const char *t_text[] = {
 "", "INT", "VOID", "DOUBLE", "CHAR", "ID", "NUM", "FNUM", "CNUM", "IF", 
 "ELSE", "FOR", "DO", "WHILE", "RETURN", "READ", "WRITE", "BREAK", "CONTINUE", 
 "L_OR", "L_AND", "NEQ", "CEQ", "LTE", "GTE", "UMINUS", "DECR", "INCR", 
 "SUB", "CALL", "FN_DEF", "FN_DECL", "CSTMT" };

extern int num_nodes;

extern char *yytext;
extern bool c48GDebug;
extern "C" int lines;
extern pAstNode root;

// very fucking klugy debug output
void DMESG(pAstNode curr, int t, const char *m) {
    if (c48GDebug != false) {
       char foo[16];
       if (t > BASE) strcpy(foo, t_text[t-BASE]);
       else { foo[0] = (char) t; foo[1] = 0; }
       fprintf(stderr, "Ln#%d: %s ('%s')\n", lines, foo, m);
       if (curr != NULL) curr->Print(0);
       getchar();
       }
    }

astNode::astNode(int set_type, pListNode pId) {
  type=set_type;
  node_n = num_nodes++;
  sib = NULL;
  n_kids = 0;
  def_line = lines;
  for (int i = 0; i < max_n_kids; i++) kid[i] = NULL;
  id = pId;
  nval = 0;
  fval = 0.0;
  offset = -1;
  outp_type = -1;
  real_type = -1;
  level = -1;
  offset = -1;
  }

astNode::astNode(int set_type, double dval) {
  type=set_type;
  node_n = num_nodes++;
  sib = NULL;
  n_kids = 0;
  def_line = lines;
  for (int i = 0; i < max_n_kids; i++) kid[i] = NULL;
  id = NULL;
  fval = dval;
  nval = 0;
  outp_type = -1;
  real_type = -1;
  level = -1;
  offset = -1;
  }

astNode::astNode(int set_type, int ival)
{
  type=set_type;
  node_n = num_nodes++;
  sib = NULL;
  n_kids = 0;
  def_line = lines;
  for (int i = 0; i < max_n_kids; i++) kid[i] = NULL;
  id = NULL;
  fval = 0.0;
  nval = ival;
  outp_type = -1;
  real_type = -1;
  level = -1;
  offset = -1;
  }

void astNode::addChild(pAstNode x) {
  if (n_kids < max_n_kids) kid[n_kids++] = x;
  else DMESG(NULL, type, "Warning! max_n_kids exceeded");
  }

void astNode::addSib(pAstNode x) {
  sib = x;
  }

pAstNode astNode::getSib() {
  return sib;
  }

pAstNode astNode::getChild(int x) {
  if (x < n_kids) return kid[x];
  else DMESG(NULL, 0, "getChild called with illegal X value");
  return NULL;
  }

void astNode::Print(int plevel) {
  // Indent according to the level.
  printf("* ");
  for (int i = 0; i < plevel; i++)
    printf("  ");

  // Print the type.
  if (type < BASE) {
    char foo[3] = " ";   // this is a kluge til I can look up %c?
    foo[0] = (char) type; 
    printf("'%s'\t\t(", foo);
    }
  else {
    if (type == FNUM)     printf("%s (%f)\t\t(", t_text[type-BASE], fval);
    else if (type == NUM) printf("%s (%d)\t\t(", t_text[type-BASE], nval);
    else if (type == CNUM) printf("%s (\0x0%h)\t\t(", t_text[type-BASE], nval);
    else if (id != NULL)  printf("%s (%s)\t\t(", t_text[type-BASE], getName(id));
    else                  printf("%s \t\t\t(", t_text[type-BASE]);
    }
  printf("L#: %d ", def_line);
  printf("N#: %d ", node_n);
  if (real_type > -1) printf("RT: %s ", dt_text[real_type]);
  if (outp_type > -1) printf("OT: %s ", dt_text[outp_type]);
  if (offset != -1)    printf("OF: %d ", offset);
  if (level  != -1)    printf("LV: %d ", level);
  printf(")\n");
  for (int j = 0; j < n_kids; j++)
          if (kid[j]!=NULL) kid[j]->Print(plevel+1);
  else  {
        printf("*");
        for (int i = 0; i < plevel + 1; i++) printf("  ");
        printf("NULL\n");
        }
  if (sib != NULL) {
    printf("\n");
    sib->Print(plevel);
    }
  }

void astNode::resetNType(int s_type) {
  type = s_type;
  }

void astNode::setDType(bool real, int s_type) {  // set type; real = true, outp = false
  if (real) real_type = s_type;
  else      outp_type = s_type;
  }

int  astNode::getDType(bool real) {              // get type; real = true, outp = false
  if (real) return real_type;
  else      return outp_type;
  }

const char* astNode::getDTText(bool real) {              // get type; real = true, outp = false
  if (real) return (const char *) dt_text[real_type];
  else      return (const char *) dt_text[outp_type];
  }

int astNode::getNKids(void) { 
  return n_kids;
  }


pListNode astNode::getID(void) { 
  return id;
  }
  
int astNode::getNType(void) {
  return type;
  }

int astNode::getLine(void) {
  return def_line;
  }

int astNode::getNVal(void) {
  return nval;
  }

int astNode::getOffset(void) {
  return offset;
  }

void astNode::setOffset(int s_off) {
  offset = s_off;
  }
  
int astNode::getLevel(void) {
  return level;
  }

void astNode::setLevel(int s_lvl) {
  level = s_lvl;
  }

double astNode::getFVal(void) {
  return fval;
  }
