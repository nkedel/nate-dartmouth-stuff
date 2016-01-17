#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "astNode.h"
#include "idTab.h"
#include "symTab.h"
#include "parse.tab.h"

extern void DMESG(pAstNode curr, int t, const char *m);
extern pAstNode root;
extern bool Error;
extern bool c48SDebug;
extern int  global_var_sz;

SymbolTable SymTable;

void SC_Err(char *s) {
  fprintf(stderr, "Semantic Error: %s\n", s);
  }
  
int current_return_type;                // we can make this global, since we don't support nested funcs
  
bool Eval_VAR_DECL(pAstNode type_node);
bool Eval_FN_DECL(pAstNode type_node);
bool Eval_FN_DEF(pAstNode type_node);
bool Eval_FN_PARAM(pAstNode type_node);
bool Eval_CSTMT(pAstNode cst_node);
bool Eval_STMT(pAstNode stm_n);
bool Eval_EXPR(pAstNode stm_n);
bool Eval_IF(pAstNode stm_n);
bool Eval_FOR(pAstNode stm_n);
bool Eval_WHILE(pAstNode stm_n);
bool Eval_DO(pAstNode stm_n);
bool Eval_RET(pAstNode stm_n);
bool Eval_READ(pAstNode stm_n);
bool Eval_WRITE(pAstNode stm_n);
bool Eval_BREAK(pAstNode stm_n);
bool Eval_CONT(pAstNode stm_n);
bool Eval_CALL(pAstNode stm_n);
bool Eval_ASN(pAstNode stm_n);
bool Eval_BIN(pAstNode stm_n);
bool Eval_UN(pAstNode stm_n);
bool Eval_VAR_REF(pAstNode stm_n);
bool Eval_CONST(pAstNode stm_n);

int getType(pAstNode type_node) {
  int i;
  switch ( type_node-> getNType() ) {
    case VOID:    return t_VOID;
    case INT:     return t_INT;
    case CHAR:    return t_CHAR;
    case DOUBLE:  return t_DOUBLE;
    default: return -1;
    }    
  }
  
extern const char* t_text;

void sdm(pAstNode node, char *s) {
  if (c48SDebug) {
    fprintf(stderr, "Called: %s on node\n",s );
    node->Print(1);
    getchar();
    }
  }
 
bool Eval_Global(pAstNode global) {
  sdm(global, "Global");
  int i = global->getNType();
  switch(i) {
    case FN_DEF:  if (Eval_FN_DEF(global)  == false) return false;   
                  break;
    case FN_DECL: if (Eval_FN_DECL(global) == false) return false;  
                  break;
    case INT:
    case DOUBLE:
    case CHAR:    if (Eval_VAR_DECL(global)== false) return false;
                  break;
    case VOID:    fprintf(stderr, "Variable defined as VOID on line %d\n", global->getLine());
                  return false; 
    default:      SC_Err("Illegal node in global scope");
                  fprintf(stderr, "->statement other than FN_DEF, FN_DECL, or");
                  fprintf(stderr, "VAR_DECL at global scope!\n");
                  return false;
    }
  if (global->getSib() != NULL) return Eval_Global(global->getSib());
  global_var_sz = SymTable.getDataSz();
  return true;
  }

bool Eval_VAR_DECL(pAstNode type_node) { 
  sdm(type_node, "VAR_DECL");
  int out_t = getType(type_node);
  if  (out_t == -1 || out_t == t_VOID ) { SC_Err("Variable Declaration not a TYPE"); return false; }  
                                                            // must be a valid type & not VOID
  if (type_node->getNKids() != 1) { SC_Err("Must be 1 child for a variable declaration"); return false; } 
  pAstNode kid = type_node->getChild(0);
  if (kid == NULL) { SC_Err("No NULL variable names."); return false; } // and at least 1 must be valid  
  while (kid != NULL) {
    const char *name;
    int sz, offset;
    SymNode    *sNode;
    switch(kid->getNType()) {
      case SUB: 
      case ID:  name = getName(kid->getID());
                if ((sNode = SymTable.Insert(name, out_t)) == NULL) {
                  fprintf(stderr, "Var '%s' redeclared on line %d\n", name, kid->getLine());
                  return false;
                  }
                sz = 8;
                if (kid->getNType() == SUB) {
                  if (kid->getNKids()  != 1)  { SC_Err("Variable w/o dimension or w/ more than 1 dimension."); return false; } 
                  if ((kid->getChild(0))->getNType() != NUM) { SC_Err("Variable w/ dimension set other than NUM"); return false; } 
                  sNode->setArray();
                  sNode->setDim( (kid->getChild(0)) -> getNVal());  
                  if (sNode->getDim() <= 0) { 
                    fprintf(stderr, "array %s created with illegal (<= 0) dimension\n");
                    return false;
                    }
                  if (out_t = t_CHAR) sz=1; 
                  else if (out_t = t_INT) sz=4; 
                  else sz = 8;
                  sz = (sz * sNode->getDim()) + 8;
                  }
                break;                 
      default:  SC_Err("Variable name on definition not ID or SUB"); return false;
      }
    kid->setDType(true,  out_t);
    kid->setDType(false, out_t);
    offset = SymTable.addVar(sz);
    kid->setOffset(offset);
    sNode->setOffset(offset);
    sNode->setSize(sz);
    kid->setLevel(sNode->getLevel());
    kid = kid->getSib();
    }
  type_node->setDType(true,  out_t);
  type_node->setDType(false, out_t);
  return true;
  }

bool Eval_FN_DECL(pAstNode type_node) {
  sdm(type_node, "FN_DECL");
  int out_t = -1;
  if (type_node->getNType() != FN_DECL) { SC_Err("Eval_FN_DECL called on something other than a FN_DECL node."); return false; } 
  if (type_node->getNKids() != 2)       { SC_Err("Eval_FN_DECL called on node w/ children != 2"); return false; } 
  out_t = getType(type_node->getChild(0));
  if (out_t == -1) { SC_Err("FN_DECL must have a type."); return false; }  

  const char *name = getName(type_node->getID());
  SymNode *sNode = SymTable.Insert(name, out_t, true, false);
  if (sNode == NULL) {
    fprintf(stderr, "FN '%s' redeclared on line %d\n", name, type_node->getLine());
    return false;
    }
// build list of parameters
  pAstNode kid = type_node->getChild(1);
  if (kid == NULL) { SC_Err("Must have at least 1 Param: Use VOID for no params."); return false; }      
  if (getType(kid) != t_VOID)
    while (kid != NULL) {
      int tPar = getType(kid);
      if (tPar == -1 || tPar == t_VOID) {
        fprintf(stderr,"Illegal param type, or more than one void in FN %s defn on line $d\n", name, kid->getLine()); 
        return false;
        }
      sNode->addParam(tPar);
      kid->setDType(true,  tPar);
      kid->setDType(false, tPar);
      kid = kid->getSib();
      }
  type_node -> setDType(true,  out_t);
  type_node -> setDType(false, out_t);  
  return true;
  }  

bool Eval_FN_PARAM(pAstNode type_node) { 
  sdm(type_node, "FN_PARAM");
  int out_t = getType(type_node);
  if  (out_t == -1 || out_t == t_VOID ) { SC_Err("Either we've got a syntax error,\nor a VOID param was not caught by Eval_FN_DECL."); return false; }  
  if (type_node->getNKids() != 1)       { SC_Err("Parameters must be defined one at a time.\n"); return false; } 
  pAstNode kid = type_node->getChild(0);
  if (kid == NULL) { SC_Err("NULL Child on Eval_FN_PARAM"); return false; } // and at least 1 must be valid  
  
  const char *name;
  int sz, offset;
  SymNode    *sNode;
  switch(kid->getNType()) {
    case SUB:
    case ID:  name = getName(kid->getID());
              if ((sNode = SymTable.Insert(name, out_t)) == NULL) {
                fprintf(stderr, "Param '%s' redeclared on line %d\n", name, kid->getLine());
                return false;
                }
              sz = 8;
     
                break;                 
      default:  SC_Err("Child not SUB or ID for Eval_FN_PARAM"); return false;
      }
  kid->setDType(true,  out_t);
  kid->setDType(false, out_t);
  offset = SymTable.addVar(sz);
  kid -> setOffset(offset);
  sNode->setOffset(offset);
  sNode->setSize(sz);
  type_node->setDType(true,  out_t);
  type_node->setDType(false, out_t);
  return true;
  }

bool Eval_FN_DEF(pAstNode type_node) {
  sdm(type_node, "FN_DEF");
  int out_t = -1;
  if (type_node->getNType() != FN_DEF) { SC_Err("Eval_FN_DEF called on node type != FN_DEF"); return false; } 
  if (type_node->getNKids() != 3)      { SC_Err("Eval_FN_DEF called on node n_kids != 3"); return false; } 
  out_t = getType(type_node->getChild(0));
  if (out_t == -1) { SC_Err("Eval_FN_DEF w/o return type"); return false; }
  current_return_type = out_t;                                   // set type for return stmt 
  
  const char *name = getName(type_node->getID());
  SymNode *sNode = SymTable.Insert(name, out_t, true, true);
  if (sNode != NULL) { // NOT YET DECLARED, handle declaration
    // build list of parameters
    pAstNode kid = type_node->getChild(1);
    if (kid == NULL) { SC_Err("No children on params for Eval_FN_DEF"); return false; }
    if (getType(kid) != t_VOID) {
      while (kid != NULL) {
        int tPar = getType(kid);
        if (tPar == -1 || tPar == t_VOID) {
          fprintf(stderr,"Illegal param type, or more than one void in FN %s defn on line $d\n", name, kid->getLine()); 
          return false;
          }
        sNode->addParam(tPar);
        kid->setDType(true,  tPar);
        kid->setDType(false, tPar);
        kid = kid->getSib();
        }
      }
    type_node -> setDType(true,  out_t);
    type_node -> setDType(false, out_t);  
    }
  // we're declared, either above, or manually
  sNode = SymTable.Lookup(name);
  if (sNode == NULL) { SC_Err("Eval_FN_DEF can't find itself!"); return false; }
  if (sNode->isFunc() != true) { SC_Err("Redefining a variable as a function"); return false; }
  if (sNode->beenDefn() == false) {
    pAstNode k_p = type_node->getChild(1);
    if (getType(k_p) == t_VOID) {
       if (sNode->getNParam() != 0) { 
        fprintf(stderr,"FN %s declared as non-void, redef. as VOID on line %d", name, type_node->getLine()); 
        return false;
        }
      }
    else 
      for (int j=0; j < sNode->getNParam(); j++) {
        if (k_p==NULL) { SC_Err("Too few parameters on function!"); return false; }
        if (getType(k_p) != sNode->getParamT(j)) {
          fprintf(stderr, "FN %s param %d mismatch %s != %s", name, j, dt_text[getType(k_p)], dt_text[sNode->getParamT(j)]);
          return false;
          }
        k_p = k_p->getSib();
        } 
    sNode->setDefn();     // mark that we now have definition
    }
  
  // build stack frame offsets and create parameter variables.
  SymTable.EnterScope();         // enter parameter scope (should always be lv 1)
  pAstNode kid = type_node->getChild(1);
  if (getType(kid) != t_VOID) 
    while (kid != NULL) {
      if (Eval_FN_PARAM(kid) == false) return false;
      kid = kid->getSib();
      }
  
  kid = type_node->getChild(2);
  if (kid->getNType() != CSTMT) { SC_Err("Child 3 is NOT a CSTMT for this FN!"); return false; }
  if (Eval_CSTMT(kid) == false) return false;
  type_node->setOffset(SymTable.getDataSz());
  SymTable.LeaveScope();
  return true;
  }  
  
bool Eval_CSTMT(pAstNode cst_node) {
  sdm(cst_node, "CSTMT");
  if (cst_node->getNType() != CSTMT) { SC_Err("Eval_CSTMT called on node type != CSTMT"); return false; } 
  if (cst_node->getNKids() != 2)     { SC_Err("Eval_FN_DEF called on node n_kids != 2"); return false; } 
  SymTable.EnterScope();
  pAstNode kid = cst_node->getChild(0);
  while (kid!=NULL) {
    if (Eval_VAR_DECL(kid) == false) return false;
    kid = kid->getSib();
    }

  kid = cst_node->getChild(1);
  while (kid!=NULL) {
    if (Eval_STMT(kid) == false) return false;
    kid = kid->getSib();
    }
  cst_node->setOffset(SymTable.getDataSz());
  SymTable.LeaveScope();
  return true;  
  }

bool Eval_STMT(pAstNode stm_n) {
  // gah! this isn't so easy, since we can have virtually anything here!
  sdm(stm_n, "STMT");
  int tok = stm_n->getNType();
  bool res;
  switch(tok) {
    case '=': 
    case '+':
    case '-':
    case '*':
    case '/':
    case '%': 
    case '>': 
    case '<': 
    case GTE: 
    case LTE: 
    case NEQ: 
    case CEQ:
    case L_AND:
    case L_OR:
    case '!': 
    case DECR:
    case INCR:
    case CALL:  
    case ID:
    case SUB:
    case NUM:
    case CNUM:
    case FNUM:
    case UMINUS: res = Eval_EXPR(stm_n);
                 break;
    case CSTMT: 
              res = Eval_CSTMT(stm_n); 
              break;
    case ELSE:
    case IF:  res = Eval_IF(stm_n);
              break;
    case FOR: res = Eval_FOR(stm_n);
              break;
    case WHILE:
              res = Eval_WHILE(stm_n);
              break;
    case DO:  res = Eval_DO(stm_n);
              break;
    case RETURN:  
              res = Eval_RET(stm_n);
              break;
    case READ:
              res = Eval_READ(stm_n);
              break;
    case WRITE:
              res = Eval_WRITE(stm_n);
              break;
    case BREAK:
              res = Eval_BREAK(stm_n);
              break;
    default:  SC_Err("I don't understand this expression!");
              stm_n->Print(0);
              return false;    
    }
  return res;
  }  

bool Eval_EXPR(pAstNode stm_n) {
  sdm(stm_n, "EXPR");  
  // gah! this isn't so easy, since we can have virtually anything here!
  int tok = stm_n->getNType();
  bool res;
  switch(tok) {
    case '=':     res = Eval_ASN(stm_n);
                  break;
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
    case '>': 
    case '<': 
    case GTE: 
    case LTE: 
    case NEQ: 
    case CEQ:
    case L_AND:
    case L_OR:    res = Eval_BIN(stm_n);
                  break;
    case '!':
    case DECR:
    case INCR:
    case UMINUS:  res = Eval_UN(stm_n);
                  break;

    case CALL:  res = Eval_CALL(stm_n); 
                break;
    case ID:    
    case SUB:   res = Eval_VAR_REF(stm_n);
                break;
    case NUM:
    case CNUM:   
    case FNUM:  res = Eval_CONST(stm_n);
                break;
                
    default:  SC_Err("This may not be an expression!");
              return false;
    }          
  return res; 
  }
  
bool Eval_IF(pAstNode stm_n) { 
  pAstNode kid;
  int tok = stm_n->getNType();
  if (tok == IF) {
    sdm(stm_n, "IF");
    if (stm_n->getNKids() != 2)     { SC_Err("Eval_IF called on IF node w/ n_kids != 2"); return false; }

    // eval condition
    kid = stm_n->getChild(0);    
    if (Eval_EXPR(kid) == false) return false;
    
    // set condition to output an int
    kid->setDType(false, t_INT);

    // eval payload
    kid = stm_n->getChild(1);
    if (Eval_STMT(kid) == false) return false;
    }
  else 
  if (tok == ELSE) {
    sdm(stm_n, "ELSE");
    if (stm_n->getNKids() != 3)     { SC_Err("Eval_IF called on ELSE node w/ n_kids != 3"); return false; }

    // eval condition
    kid = stm_n->getChild(0);    
    if (Eval_EXPR(kid) == false) return false;
    
    // set condition to output an int
    kid->setDType(false, t_INT);

    // eval true payload    
    kid = stm_n->getChild(1);
    if (Eval_STMT(kid) == false) return false;
    
    // eval false payload    
    kid = stm_n->getChild(2);
    if (Eval_STMT(kid) == false) return false;
    }
  else { SC_Err("Eval_IF called on non-IF/ELSE nodes."); return false; }
  return true; 
  }
  
bool Eval_FOR(pAstNode stm_n) { 
  sdm(stm_n, "FOR");
  pAstNode kid;
  if (stm_n->getNType() != FOR)   { SC_Err("Eval_FOR called on non-FOR node"); return false; }
  if (stm_n->getNKids() != 4)     { SC_Err("Eval_FOR called w/ n_kids != 4"); return false; }

  // eval condition1
  kid = stm_n->getChild(0);    
  if (kid != NULL)
    if (Eval_EXPR(kid) == false) return false;
    
  // eval condition2
  kid = stm_n->getChild(1);    
  if (kid != NULL)
    if (Eval_EXPR(kid) == false) return false;
  kid->setDType(false, t_INT);
    
  // eval condition3
  kid = stm_n->getChild(2);    
  if (kid != NULL)
    if (Eval_EXPR(kid) == false) return false;

  // eval payload    
  kid = stm_n->getChild(3);
  if (Eval_STMT(kid) == false) return false;
  return true; 
  }
  
bool Eval_WHILE(pAstNode stm_n) { 
  sdm(stm_n, "WHILE");
  pAstNode kid;
  if (stm_n->getNType() != WHILE)   { SC_Err("Eval_WHILE called on non-WHILE node"); return false; }
  if (stm_n->getNKids() != 2)     { SC_Err("Eval_WHILE called w/ n_kids != 2"); return false; }

  // eval condition
  kid = stm_n->getChild(0);    
  if (Eval_EXPR(kid) == false) return false;
  kid->setDType(false, t_INT);
    
  // eval payload    
  kid = stm_n->getChild(1);
  if (Eval_STMT(kid) == false) return false;
  return true; 
  }
  
bool Eval_DO(pAstNode stm_n) { 
  sdm(stm_n, "DO");
  pAstNode kid;
  if (stm_n->getNType() != DO)   { SC_Err("Eval_DO called on non-DO node"); return false; }
  if (stm_n->getNKids() != 2)     { SC_Err("Eval_DO called w/ n_kids != 2"); return false; }

  // eval condition
  kid = stm_n->getChild(1);    
  if (Eval_EXPR(kid) == false) return false;
  kid->setDType(false, t_INT);
    
  // eval payload    
  kid = stm_n->getChild(0);
  if (Eval_STMT(kid) == false) return false;
  return true; 
  }
  
bool Eval_RET(pAstNode stm_n) { 
  sdm(stm_n, "RETURN");
  if (stm_n->getNType() != RETURN)   { SC_Err("Eval_RETURN called on non-RETURN node"); return false; }
  if (stm_n->getNKids() > 1)     { SC_Err("Eval_RETURN called w/ n_kids > 1"); return false; }
  if (stm_n->getNKids() == 0 && current_return_type != t_VOID) {
    fprintf(stderr,"Warning: VOID return type on FN with %s return type...", dt_text[current_return_type]);
    }
  else {
    pAstNode kid = stm_n->getChild(0);
    if (kid != NULL) {
      if (Eval_EXPR(kid) == false) return false;
      if (kid->getDType(true) == t_VOID) {
        fprintf(stderr,"Warning: VOID return type on FN with %s return type...", dt_text[current_return_type]);
        kid->setDType(false, t_VOID);
        stm_n->setDType(false, t_VOID);
        return true;
        }
      else {
        kid   -> setDType(false, current_return_type);
        stm_n -> setDType(false, current_return_type);
        }
      }
    else
      if (current_return_type != t_VOID)
        fprintf(stderr,"Warning: VOID return type on FN with %s return type...", dt_text[current_return_type]);
    }
  
  return true; 
  }
  
bool Eval_READ(pAstNode stm_n) { 
  sdm(stm_n, "READ");
  if (stm_n->getNType() != READ) { SC_Err("Eval_READ called on non-READ node"); return false; }
  if (stm_n->getNKids() != 1)    { SC_Err("Eval_READ called w/ n_kids != 1"); return false; }
  pAstNode kid = stm_n->getChild(0);
  if (kid == NULL)              { SC_Err("Eval_READ called w/ NULL child"); return false; } 
  int in_t = kid->getNType();
  if (in_t != ID && in_t != SUB) { SC_Err("Eval_READ called on non-variable"); return false; }
  if (Eval_VAR_REF(kid) == false) return false;
  in_t = kid->getDType(true);
  stm_n->setDType(true, in_t);
  stm_n->setDType(false, in_t);
  return true; 
  }
  
bool Eval_WRITE(pAstNode stm_n) { 
  sdm(stm_n, "WRITE");
  if (stm_n->getNType() != WRITE) { SC_Err("Eval_READ called on non-READ node"); return false; }
  if (stm_n->getNKids() != 1)    { SC_Err("Eval_READ called w/ n_kids != 1"); return false; }
  pAstNode kid = stm_n->getChild(0);
  if (kid == NULL)              { SC_Err("Eval_READ called w/ NULL child"); return false; }
  if (Eval_EXPR(kid) == false) return false;
  int in_t = kid->getDType(true);
  stm_n->setDType(true, in_t);
  stm_n->setDType(false, in_t);
  return true; 
  }
  
bool Eval_BREAK(pAstNode stm_n) {
  sdm(stm_n, "BREAK");
  return true; 
  }

bool Eval_CONT(pAstNode stm_n) {
  sdm(stm_n, "CONTINUE");
  return true; 
  }

bool Eval_CALL(pAstNode stm_n) {
  sdm(stm_n, "CALL");
  const char *name;
  name = getName(stm_n->getID());
  SymNode *sNode = SymTable.Lookup(name);
  if (sNode == NULL) { 
      fprintf(stderr, "FN '%s' not declared in this scope (ln %d)\n", name, stm_n->getLine());
      return false;
      }
  if (sNode->isFunc() == false) { 
      fprintf(stderr, "Cannot treat var '%s' as a func (ln %d)\n", name, stm_n->getLine());
      return false;
      }
  if (stm_n->getNKids() == 0) {
    if (sNode->getNParam() != 0) {
      SC_Err("No parameters on function not declared as (VOID)!"); return false;   
      }
    }
  else {
    pAstNode k_p = stm_n->getChild(0);
    for (int j=0; j < sNode->getNParam(); j++) {
      if (k_p==NULL) { SC_Err("Too few parameters on function!"); return false; }
      if (Eval_EXPR(k_p) == false) return false;
      k_p->setDType(false, sNode->getParamT(j));
      k_p = k_p->getSib();
      }
    if (k_p!=NULL) { SC_Err("Too few parameters on function!"); return false; }
    } 
  stm_n->setOffset(sNode->getNParam() * 8);
  stm_n->setDType(true, sNode->getType());
  return true;
  }

bool Eval_ASN(pAstNode stm_n) {
  sdm(stm_n, "ASSIGNMENT");
  if (stm_n->getNType() != '=')  { SC_Err("Eval_ASN called on non-'=' node"); return false; }
  if (stm_n->getNKids() != 2)    { SC_Err("Eval_ASN called w/ n_kids != 2"); return false; }
  pAstNode kidl = stm_n->getChild(0);
  pAstNode kidr = stm_n->getChild(1);
  if (kidl == NULL || kidr == NULL) { SC_Err("Eval_READ called w/ NULL child"); return false; }
  if (Eval_VAR_REF(kidl) == false) return false;
  if (Eval_EXPR(kidr) == false) return false;
  int out_t = kidl->getDType(true);
  stm_n->setDType(true, out_t);
  stm_n->setDType(false, out_t);
  kidl->setDType(false, out_t);
  kidr->setDType(false, out_t);
  return true;
  }

bool Eval_BIN(pAstNode stm_n) {
  sdm(stm_n, "BINARY OP");
  if (stm_n->getNKids() != 2)     { SC_Err("Eval_BIN called w/ n_kids != 2"); return false; }
  bool logop;
  int n_t = stm_n->getNType();
  switch(n_t) {
    case '+':
    case '-':
    case '*':
    case '/':
    case '%': logop = false;
              break;
    case '>': 
    case '<': 
    case GTE: 
    case LTE: 
    case NEQ: 
    case CEQ:
    case L_AND:
    case L_OR: logop = true;
               break;
    default: SC_Err("Eval_BIN called on a node which is not a binary operator.");
             return false;
    }
  pAstNode kidl = stm_n->getChild(0); 
  pAstNode kidr = stm_n->getChild(1);
  if (kidl == NULL || kidr == NULL) {
    SC_Err("Eval_BIN: No binary ops on NULL expressions!");
    return false;
    }
  if (Eval_EXPR(kidl) == false) return false;
  if (Eval_EXPR(kidr) == false) return false;
  int n_type = stm_n->getNType();
  int l_t = kidl->getDType(true);
  int r_t = kidr->getDType(true);
  if (l_t == t_VOID || r_t == t_VOID) {
    SC_Err("Eval_BIN: No binary ops on VOID expressions!");
    return false;
    }
  int o_t;
  if (l_t > r_t) o_t = l_t; 
    else         o_t = r_t;
  if (logop) stm_n->setDType(true, t_INT);
    else stm_n->setDType(true,  o_t);
  kidl ->setDType(false, o_t);
  kidr ->setDType(false, o_t);
  return true;
  }
  
bool Eval_UN(pAstNode stm_n) {
  sdm(stm_n, "UNARY OP");
  if (stm_n->getNKids() != 1)     { SC_Err("Eval_UN called w/ n_kids != 2"); return false; }
  bool logop;
  int n_t = stm_n->getNType();
  switch(n_t) {
    case '!': logop = true;
              break;
    case INCR:
    case DECR:
    case UMINUS: logop = false;
                 break;
    default: SC_Err("Eval_UN called on a node which is not a unary operator.");
             return false;
    }
  pAstNode kid = stm_n->getChild(0); 
  if (kid == NULL) {
    SC_Err("Eval_UN: No unary ops on NULL expressions!");
    return false;
    }
  if (Eval_EXPR(kid) == false) return false;
  int n_type = stm_n->getNType();
  int k_t = kid->getDType(true);
  if (k_t == t_VOID) {
    SC_Err("Eval_UN: No unary ops on VOID expressions!");
    return false;
    }
  if (logop) stm_n->setDType(true, t_INT);
    else stm_n->setDType(true,  k_t);
  kid ->setDType(false, k_t);
  return true;
  }
  
bool Eval_VAR_REF(pAstNode stm_n) {
  sdm(stm_n, "VAR REF");
  int out_t = stm_n->getNType();
  if (out_t == ID) {
    if (stm_n->getNKids() != 0) { SC_Err("VAR REF: ID Must have no child"); return false; } 
    const char *name;
    name = getName(stm_n->getID());
    SymNode *sNode = SymTable.Lookup(name);
    if (sNode == NULL) { 
      fprintf(stderr, "Var '%s' not defined in this scope (ln %d)\n", name, stm_n->getLine());
      return false;
      }
    if (sNode->isArray() == true) { 
      fprintf(stderr, "Cannot reference array '%s' w/o a subscript (ln %d)\n", name, stm_n->getLine());
      return false;
      }
    if (sNode->isFunc() == true) { 
      fprintf(stderr, "Cannot treat func '%s' w/o as a Var (ln %d)\n", name, stm_n->getLine());
      return false;
      }
    stm_n->setDType(true, sNode->getType());
    stm_n->setLevel(sNode->getLevel());
    stm_n->setOffset(sNode->getOffset());
    return true;
     }
  else if (out_t == SUB) {
    if (stm_n->getNKids() != 1) { SC_Err("VAR REF: SUB Must have 1 child for a subscript"); return false; } 
    pAstNode kid = stm_n->getChild(0);
    if (kid == NULL) { SC_Err("VAR REF: No NULL expression subscripts."); return false; } // and at least 1 must be valid 
    if (Eval_EXPR(kid) == false) return false;    
    if (kid->getDType(true) == t_VOID) { SC_Err("VAR REF: Can't have a void subscript expr"); return false; } 
    kid->setDType(false, t_INT);
    const char *name;
    name = getName(stm_n->getID());
    SymNode *sNode = SymTable.Lookup(name);
    if (sNode == NULL) { 
      fprintf(stderr, "Array '%s' not defined in this scope (ln %d)\n", name, stm_n->getLine());
      return false;
      }
    if (sNode->isArray() == false) { 
      fprintf(stderr, "Var '%s' not an array in this scope (ln %d)\n", name, stm_n->getLine());
      return false;
      }
    if (sNode->isFunc() == true) { 
      fprintf(stderr, "Cannot treat func '%s' w/o as a Var (ln %d)\n", name, stm_n->getLine());
      return false;
      }
    stm_n->setDType(true, sNode->getType());
    stm_n->setLevel(sNode->getLevel());
    stm_n->setOffset(sNode->getOffset());
    return true;
    }
  else 
    SC_Err("VAR_REF called on node neither ID nor SUB"); 
    return false;
  }  

bool Eval_CONST(pAstNode stm_n) {
  sdm(stm_n, "CONSTANT");
  int c_type = stm_n->getNType();
  if (c_type != NUM && c_type != CNUM && c_type != FNUM) { SC_Err("Eval_CONST called on non-constant"); return false; }
  if (stm_n->getNKids() != 0) { SC_Err("Strange... constants should not have children..."); getchar(); }
  if (c_type == NUM)       stm_n->setDType(true, t_INT);
  else if (c_type == CNUM) stm_n->setDType(true, t_CHAR);
  else                     stm_n->setDType(true, t_DOUBLE);
  return true;
  }
