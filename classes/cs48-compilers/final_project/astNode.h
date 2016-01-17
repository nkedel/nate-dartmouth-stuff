// astNode.h
// Declaration of abstract syntax tree class 
// based on "TreeNode" class originally provided by Prof. Cormen,
// modified N. Edel

#include "idTab.h"

#ifndef ASTNODE_H
#define ASTNODE_H 
const int t_UNDEF  = -1;
const int t_NONE   = 0;
const int t_VOID   = 1;
const int t_CHAR   = 2;
const int t_INT    = 3;
const int t_DOUBLE = 4;

#ifndef ASTNODE_CC
extern char *dt_text[];
#endif 

extern char *yytext;

class astNode;
typedef astNode *pAstNode;

class astNode {
public:
  astNode(int set_type, pListNode pId = NULL);      // constructor
  astNode(int set_type, int ival);      // constructor
  astNode(int set_type, double dval);   // constructor
  void addChild(pAstNode x);            // make x the next child
  void addSib(pAstNode x);              // make x the sibling
  pAstNode getSib();                    // return the sibling
  pAstNode getChild(int x);             // return the sibling
  void Print(int level);                // print the astNode
  int  getNType(void);                  // returns the type
  void resetNType(int s_type);          // changes the node type to s_type
  void setDType(bool real, int s_type); // set type; real = true, outp = false
  int  getDType(bool real);             // get type; real = true, outp = false
  const char* getDTText(bool real);     // get type as text
  int  getNKids(void);                  // return number of children
  pListNode getID(void);                // return the associated ID
  int  getLine(void);                   // return the line node was created at
  int  getNVal(void);                   // return the integer value
  double getFVal(void);                 // return the FP value
  void setOffset(int s_off);            // set the offset
  int  getOffset(void);                 // get the offset
  void setLevel(int s_lvl);             // set the level
  int  getLevel(void);                  // get the level
  
private:
  static const int max_n_kids = 5;
  int type;              // type of astNode
  int n_kids;            // number of Child Nodes
  pAstNode sib;          // right sibling
  pListNode id;              
  int real_type;
  int outp_type;
  int nval;
  double fval;
  int def_line;
  int node_n;
  int offset;
  int level;
  pAstNode kid[max_n_kids];
  };

#endif

