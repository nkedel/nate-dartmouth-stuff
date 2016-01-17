#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "astNode.h"
#include "idTab.h"
#include "symTab.h"
#include "label.h"
#include "parse.tab.h"

extern pAstNode root;
extern bool Error;
extern bool c48CDebug;

extern int global_var_sz;

extern SymbolTable SymTable;

void CG_Err(char *s) {
  fprintf(stderr, "Code-gen Error: %s\n", s);
  }
  
int current_level = 0;
  
bool CGen_VAR_DECL(pAstNode type_node);
bool CGen_FN_DECL(pAstNode type_node);
bool CGen_FN_DEF(pAstNode type_node);
bool CGen_FN_PARAM(pAstNode type_node);
bool CGen_CSTMT(pAstNode cst_node);
bool CGen_STMT(pAstNode stm_n);
bool CGen_EXPR(pAstNode stm_n);
bool CGen_IF(pAstNode stm_n);
bool CGen_FOR(pAstNode stm_n);
bool CGen_WHILE(pAstNode stm_n);
bool CGen_DO(pAstNode stm_n);
bool CGen_RET(pAstNode stm_n);
bool CGen_READ(pAstNode stm_n);
bool CGen_WRITE(pAstNode stm_n);
bool CGen_BREAK(pAstNode stm_n);
bool CGen_CONT(pAstNode stm_n);
bool CGen_CALL(pAstNode stm_n);
bool CGen_ASN(pAstNode stm_n);
bool CGen_BIN(pAstNode stm_n);
bool CGen_UN(pAstNode stm_n);
bool CGen_VAR_REF(pAstNode stm_n);
bool CGen_CONST(pAstNode stm_n);
bool CGen_LVADR(int lv, int off);
bool CGen_LSADR(int lv, int off, int in_t);

extern int getType(pAstNode type_node);
extern const char* t_text;

void cdm(pAstNode node, char *s) {
  if (c48CDebug) {
    fprintf(stderr, "Gen: %s on node\n",s );
    node->Print(1);
    getchar();
    }
  }
  
bool CGen_Global(pAstNode global) {
  emit_comment("Global");
  pAstNode work;
// emit startup code
  char temps[32];
  sprintf(temps,"%d",global_var_sz+8);
  emit("start", true, "LDC","6",temps,"0", false);
  emit(NULL, true, "LDC","5","0","0", false);
  emit(NULL, true, "LDA","4","4","7", false);
  emit(NULL, true, "ST", "4","0","6", false); 
  emit(NULL, true, "ST", "5","4","6", false); 
  emit(NULL, true, "LDA","6","8","6", false);
  emit(NULL, true, "LDC","7","main","0",true);  
  emit(NULL, false,"HALT","0","0","0", false);
  work = global;
  while (work != NULL) {
    if (work->getNType() == FN_DEF)
      if (CGen_FN_DEF(work) == false) return false;  
    work = work->getSib();
    }
  emit(NULL, false,"HALT","0","0","0", false);
  return true;
  }

bool CGen_FN_DEF(pAstNode type_node) {
  emit_comment("FN_DEF");
  char temps[16];
  current_level=1;
  const char *name;
  name = getName(type_node->getID()); 
  emit_comment(name);
  emit_comment("Here's a NO-OP to fit our label on...");
  emit(name, true, "LDA", "1","0","1", false);
  pAstNode kid = type_node->getChild(2);
  CGen_CSTMT(kid);
  emit_comment("Leave some junk in reg1 to catch missing return stmts");
  emit(NULL, true, "LDC", "1","-1","0", false);
  emit(NULL, true, "LDFC","1","-1","0", false);
  // trying caller clears stack frame
  emit_comment("And jump out.");
  emit(NULL, true, "LD", "7","-8","5", false);
  current_level=0;
  return true;
  }  
  
bool CGen_CSTMT(pAstNode cst_node) {
  emit_comment("entering CSTMT");
  char temps[32];
  current_level++;
  sprintf(temps, "level=%d", current_level);
  emit_comment(temps);
  emit(NULL, true, "ST","5","4","6", false);
  emit(NULL, true, "LDA","6","8","6", false);
  emit(NULL, true, "LDA","5","0","6", false);
  if (cst_node->getOffset() > 0) {
    emit_comment("Allocate space for local variables");
    sprintf(temps, "%d", cst_node->getOffset());
    emit(NULL, true, "LDA","6",temps,"6", false);
    }
  pAstNode kid = cst_node->getChild(1);
  while (kid!=NULL) {
     emit_comment("Here's the code inside the CSTMT");
   if (CGen_STMT(kid) == false) return false;
    kid = kid->getSib();
    }
    emit_comment("Clean up stack frame");
  emit(NULL, true, "LDA","6","0","5", false);
  emit(NULL, true, "LDA","6","-8","6", false);
  emit(NULL, true, "LD", "5","4","6", false);
  current_level--;
  return true;
  }

bool CGen_STMT(pAstNode stm_n) {
  // gah! this isn't so easy, since we can have virtually anything here!
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
    case UMINUS: res = CGen_EXPR(stm_n);
                 break;
    case CSTMT: 
              res = CGen_CSTMT(stm_n); 
              break;
    case ELSE:
    case IF:  res = CGen_IF(stm_n);
              break;
    case FOR: res = CGen_FOR(stm_n);
              break;
    case WHILE:
              res = CGen_WHILE(stm_n);
              break;
    case DO:  res = CGen_DO(stm_n);
              break;
    case RETURN:  
              res = CGen_RET(stm_n);
              break;
    case READ:
              res = CGen_READ(stm_n);
              break;
    case WRITE:
              res = CGen_WRITE(stm_n);
              break;
    case BREAK:
              res = CGen_BREAK(stm_n);
              break;
    default:  break;
    }
  return res;
  }  

bool CGen_EXPR(pAstNode stm_n) {
  // gah! this isn't so easy, since we can have virtually anything here!
  int tok = stm_n->getNType();
  bool res;
  switch(tok) {
    case '=':     res = CGen_ASN(stm_n);
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
    case L_OR:    res = CGen_BIN(stm_n);
                  break;
    case '!':
    case DECR:
    case INCR:
    case UMINUS:  res = CGen_UN(stm_n);
                  break;

    case CALL:  res = CGen_CALL(stm_n); 
                break;
    case ID:    
    case SUB:   res = CGen_VAR_REF(stm_n);
                break;
    case NUM:
    case CNUM:   
    case FNUM:  res = CGen_CONST(stm_n);
                break;
                
    default:  CG_Err("This may not be an expression!");
              return false;
    }          
  return res; 
  }
  
bool CGen_IF(pAstNode stm_n) { 
  pAstNode kid;
  int tok = stm_n->getNType();
  if (tok == IF) {
    emit_comment("IF: eval condition");
    kid = stm_n->getChild(0);    
    if (CGen_EXPR(kid) == false) return false;
    int endif = gentl();
    emit(NULL, true, "JNE", "1","1","7", false);
    emit(NULL, true, "LDC", "7",tlName(endif),"0", true);
    kid = stm_n->getChild(1);
    emit_comment("IF: do action");
    if (CGen_STMT(kid) == false) return false;
    emit(tlName(endif), true, "LDA", "1","0","1", false);
    }
  else 
  if (tok == ELSE) {
    if (stm_n->getNKids() != 3)     { CG_Err("CGen_IF called on ELSE node w/ n_kids != 3"); return false; }
    int endif, elseif;
    emit_comment("IF-ELSE: eval condtion");
    kid = stm_n->getChild(0);    
    if (CGen_EXPR(kid) == false) return false;
    endif = gentl();
    elseif = gentl();
    emit(NULL, true, "JNE", "1","1","7", false);
    emit(NULL, true, "LDC", "7",tlName(elseif),"0", true);
    kid = stm_n->getChild(1);
    emit_comment("IF-ELSE: do IF action");
    if (CGen_STMT(kid) == false) return false;
    emit(NULL, true, "LDC", "7",tlName(endif),"0", true);
    emit(tlName(elseif), true, "LDA", "1","0","1", false);
    // eval false payload    
    kid = stm_n->getChild(2);
    emit_comment("IF-ELSE: do ELSE action");
    if (CGen_STMT(kid) == false) return false;
    emit(tlName(endif), true, "LDA", "1","0","1", false);
    }
  }
  
bool CGen_FOR(pAstNode stm_n) { 
  emit_comment("FOR");
  pAstNode kid;
  int realst, cond, end;
  realst = gentl();
  cond   = gentl();
  end    = gentl();
  pushtl(cond, end);
  // eval condition1
  kid = stm_n->getChild(0);    

  emit_comment("evaluate FOR startup expression");
  if (kid != NULL)
    if (CGen_EXPR(kid) == false) return false;

  emit(tlName(realst), true, "LDA", "1","0","1", false);

  emit_comment("evaluate FOR 3rd action");
  kid = stm_n->getChild(3);
  if (CGen_STMT(kid) == false) return false;
  emit(tlName(cond), true, "LDA", "1","0","1", false);


  emit_comment("evaluate FOR condition");
  kid = stm_n->getChild(1);    
  if (CGen_EXPR(kid) == false) return false;
  emit(NULL, true, "JNE", "1","1","7", false);
  emit(NULL, true, "LDC", "7",tlName(end),"0", true);
    

  emit_comment("evaluate FOR 3rd part");
  kid = stm_n->getChild(2);    
  if (kid != NULL)
    if (CGen_EXPR(kid) == false) return false;
  emit(NULL, true, "LDC", "7",tlName(realst),"0", true);
  emit(tlName(end), true, "LDA", "1","0","1", false);
  poptl();
  return true; 
  }
  
bool CGen_WHILE(pAstNode stm_n) { 
  cdm(stm_n, "WHILE");
  pAstNode kid;
  int cond, end;
  cond   = gentl();
  end    = gentl();
  pushtl(cond, end);

  emit_comment("evaluate WHILE condition");
  emit(tlName(cond), true, "LDA", "1","0","1", false);
  kid = stm_n->getChild(0);    
  if (CGen_EXPR(kid) == false) return false;
  emit(NULL, true, "JNE", "1","1","7", false);
  emit(NULL, true, "LDC", "7",tlName(end),"0", true);

  emit_comment("evaluate WHILE action");
  kid = stm_n->getChild(1);
  if (CGen_STMT(kid) == false) return false;
  
  emit(NULL, true, "LDC", "7",tlName(cond),"0", true);
  emit(tlName(end), true, "LDA", "1","0","1", false);
  poptl();
  return true;
  }
  
bool CGen_DO(pAstNode stm_n) { 
  cdm(stm_n, "DO");
  pAstNode kid;
  int realst, cond, end;
  realst = gentl();
  cond   = gentl();
  end    = gentl();
  emit(tlName(realst), true, "LDA", "1","0","1", false);
  pushtl(cond, end);
  emit_comment("evaluate DO action");
  kid = stm_n->getChild(0);
  if (CGen_STMT(kid) == false) return false;
  emit(tlName(cond), true, "LDA", "1","0","1", false);

  emit_comment("evaluate DO condition");
  kid = stm_n->getChild(1);    
  if (CGen_EXPR(kid) == false) return false;
  emit(NULL, true, "JEQ", "1","1","7", false);
  emit(NULL, true, "LDC", "7",tlName(realst),"0", true);
  emit(tlName(end), true, "LDA", "1","0","1", false);
  poptl();
  return true;
  }
  
bool CGen_RET(pAstNode stm_n) { 
  emit_comment("RETURN");
  if (stm_n->getNKids() != 0 && stm_n->getDType(true) != t_VOID) {
    pAstNode kid = stm_n->getChild(0);
    if (kid != NULL) {
      if (CGen_EXPR(kid) == false) return false;
      emit_comment("Our return value is now in reg 1");
      }
    }
  else {
    emit_comment("No return value, so let's stick some mung in reg1");
    emit(NULL, true, "LDC", "1","-1","0", false);
    emit(NULL, true, "LDFC","1","-1","0", false);
    }
  emit_comment("return");
  emit(NULL, true, "LD", "7","-8","5", false);
  return true; 
  }
  
bool CGen_READ(pAstNode stm_n) { 
  emit_comment("READ");
  pAstNode kid = stm_n->getChild(0);
  int in_t = kid->getDType(true);
  int out_t = kid->getDType(true);
  if (kid == NULL) { CG_Err("CGen_READ called w/ NULL child"); return false; }
  emit_comment("Finding var addr to read to");
  if (kid->getNType() == SUB) CGen_LSADR(kid->getLevel(), kid->getOffset(), kid->getDType(true));
    else                      CGen_LVADR(kid->getLevel(), kid->getOffset());
  emit_comment("Reading into reg 1");
  if (in_t == t_DOUBLE)    emit(NULL, false, "INF","1","0","0", false);
  else if (in_t == t_CHAR) emit(NULL, false, "INB","1","0","0", false);
  else                     emit(NULL, false, "IN","1", "0","0", false);
  emit_comment("copying result to address in reg 4");
  emit(NULL, true, "ST", "1","0","4", false);
  return true; 
  }
  
bool CGen_WRITE(pAstNode stm_n) { 
  emit_comment("WRITE");
  pAstNode kid = stm_n->getChild(0);
  emit_comment("Getting value:");
  if (CGen_EXPR(kid) == false) return false;
  emit_comment("Writing");
  int in_t = kid->getDType(true);
  if (in_t == t_DOUBLE)    emit(NULL, false, "OUTF","1","0","0", false);
  else if (in_t == t_CHAR) emit(NULL, false, "OUTB","1","0","0", false);
  else                     emit(NULL, false, "OUT","1", "0","0", false);
  return true; 
  }
  
bool CGen_BREAK(pAstNode stm_n) {
  emit_comment("BREAK");
  int i;
  if ((i = toptl(true)) != -1) {
    emit(NULL, true, "LDC","7",tlName(i),"0", true);
    return true;
    }
  return false; 
  }

bool CGen_CONT(pAstNode stm_n) {
  emit_comment("CONTINUE");
  int i;
  if ((i = toptl(false)) != -1) {
    emit(NULL, true, "LDC","7",tlName(i),"0", true);
    return true;
    }
  return false; 
  }

bool CGen_CALL(pAstNode stm_n) {
  emit_comment("CALL");
  const char *name;
  name = getName(stm_n->getID());
  emit_comment(name);
  emit_comment("Create Stack frame");
  emit(NULL, true, "LDA","6","8","6", false);
  emit(NULL, true, "ST","5","-4","6", false);
  emit(NULL, true, "LDA","5","0","6", false);
  char temps[16];
  emit_comment("Allocate space for parameters");
  sprintf(temps, "%d", stm_n->getOffset());
  emit(NULL, true, "LDA","6",temps,"6", false);
  if (stm_n->getNKids() != 0) {
    pAstNode k_p = stm_n->getChild(0);
    char temps[16];
    int pc = 0;
    while (k_p != NULL) {
      if (CGen_EXPR(k_p) == false) return false;
      emit_comment("Storing Parameters");
      sprintf(temps,"%d", pc);
      if (k_p->getDType(false) == t_DOUBLE)
           emit(NULL, true, "STF", "1", temps, "5", false);
      else emit(NULL, true, "ST", "1", temps, "5", false);
      k_p = k_p->getSib();
      pc+=8;
      }
    }
  emit_comment("Store return address");
  emit(NULL, true, "LDA","4","2","7", false);
  emit(NULL, true, "ST", "4","-8","5", false); 
  emit_comment("make the jump");
  emit(NULL, true, "LDC","7",name,"0",true);    
  emit_comment("recover stack frame");
  emit(NULL, true, "LDA","6","0","5",false); 
  emit(NULL, true, "LD","5","-4","6",false); 
  emit(NULL, true, "LDA","6","-8","6",false); 
  }

bool CGen_ASN(pAstNode stm_n) {
  emit_comment("ASSIGNMENT");
  pAstNode kidl = stm_n->getChild(0);
  const char *name = getName(kidl->getID());
  emit_comment(name);
  pAstNode kidr = stm_n->getChild(1);
  int in_t = kidl->getDType(true);
  int out_t = kidl->getDType(true);
  if (kidl == NULL || kidr == NULL) { CG_Err("CGen_READ called w/ NULL child"); return false; }
  emit_comment("Grab recipient address");
  if (kidl->getNType() == SUB) CGen_LSADR(kidl->getLevel(), kidl->getOffset(), kidl->getDType(true));
    else                       CGen_LVADR(kidl->getLevel(), kidl->getOffset());
  emit_comment("Push address to stack");
  emit(NULL, true, "ST", "4","0","6", false);
  emit(NULL, true, "LDA","6","8","6", false);
  emit_comment("Get right value to assign");
  CGen_EXPR(kidr);
  emit_comment("pop recipient address from stack");
  emit(NULL, true, "LDA","6","-8","6", false);    
  emit(NULL, true, "LD", "4","0","6", false);    
  emit(NULL, true, "ST", "1","0","4", false);
  if (in_t == out_t) return true;          
  if ((in_t == t_CHAR && out_t == t_INT) || (in_t == t_INT && out_t == t_CHAR)) return true;          
  emit_comment("Coercing output");
  if ((in_t == t_DOUBLE && out_t == t_INT) || (in_t == t_DOUBLE && out_t == t_CHAR)) {
    emit(NULL, false, "CVTFI",  "2","1","0", false);
    emit(NULL, true, "LDA",  "1","0","2", false);
    return true;
    }
  if ((in_t == t_INT && out_t == t_DOUBLE) || (in_t == t_INT && out_t == t_DOUBLE)) {
    emit(NULL, false, "CVTIF",  "1","1","0", false);
    return true;
    }
  return true; 
  }

bool CGen_BIN(pAstNode stm_n) {
  emit_comment("BINARY OP");
  int n_t  = stm_n->getNType();
  int in_t = stm_n->getDType(true);
  int out_t = stm_n->getDType(false);
  pAstNode kidl = stm_n->getChild(0); 
  pAstNode kidr = stm_n->getChild(0); 

  if (n_t == L_AND) {
    int tl = gentl();
    emit_comment("L_AND: Get left value...");
    CGen_EXPR(kidl);
    if (in_t == t_DOUBLE) emit(NULL, false, "CVTFI","1","1","0", false);
    emit_comment("L_AND: Short cirtuit if 0");
    emit(NULL, true, "JNE","7","1","7", false);
    emit(NULL, true, "LDC","7",tlName(tl),"0", true);
    emit_comment("L_AND: Get right value");
    CGen_EXPR(kidr);
    if (in_t == t_DOUBLE) emit(NULL, false, "CVTFI","1","1","0", false);
    emit_comment("L_AND: Check 2nd value, return 0 or 1");
    emit(NULL, true, "JNE","7","2","7", false);
    emit(tlName(tl), true, "LDC","1","0","0", false);
    emit(NULL, true, "LDA","7","1","7", false);
    emit(NULL, true, "LDC","1","1","0", false);   
    }
  else if (n_t == L_OR) {
    int tl = gentl();
    emit_comment("L_OR: Get 1st value");
    CGen_EXPR(kidl);
    if (in_t == t_DOUBLE) emit(NULL, false, "CVTFI","1","1","0", false);
    emit_comment("L_OR: Short-circuit if true");
    emit(NULL, true, "JEQ","7","1","7", false);
    emit(NULL, true, "LDC","7",tlName(tl),"0", true);
        emit_comment("L_OR: Get 2nd value");
    CGen_EXPR(kidr);
    if (in_t == t_DOUBLE) emit(NULL, false, "CVTFI","1","1","0", false);
     emit_comment("L_OR: Check 2nd value, return 0 or 1");
    emit(NULL, true, "JEQ","7","2","7", false);
    emit(NULL, true, "LDC","1","0","0", false);
    emit(NULL, true, "LDA","7","1","7", false);
    emit(tlName(tl), true, "LDC","1","1","0", false);   
    }
  else {
    if (in_t != t_DOUBLE) {
      emit_comment("BINOP: get left value");
      CGen_EXPR(kidl);
      emit_comment("BINOP: store left value on stack");
      emit(NULL, true, "ST","1","0","6", false);
      emit(NULL, true, "LDA","6","8","6", false);
      emit_comment("BINOP: get right value");
      CGen_EXPR(kidr);
      emit_comment("BINOP: pop left value from stack to r2");
      emit(NULL, true, "LDA","6","-8","6", false);    
      emit(NULL, true, "LD","2","0","6", false);    
      switch(n_t) {
        case '+':
         emit_comment(" + ");
         emit(NULL, false, "ADD","1","2","1", false);    
          break;
        case '-':
          emit_comment(" - ");
          emit(NULL, false, "SUB","1","2","1", false);    
          break;
        case '*':
          emit_comment(" * ");
          emit(NULL, false, "MUL","1","2","1", false);    
          break;
        case '/':
          emit_comment(" / ");
          emit(NULL, false, "DIV","1","2","1", false);    
          break;
        case '%':
          emit_comment(" % ");
          emit(NULL, false, "DIV","3","2","1", false);    
          emit(NULL, false, "MUL","4","3","2", false);    
          emit(NULL, false, "SUB","1","2","4", false);    
          break;
        case '>':
          emit_comment(" > ");
          emit(NULL, false,"SUB","1","2","1", false);              
          emit(NULL, true, "JGT","1","2","7", false);
          emit(NULL, true, "LDC","1","0","0", false);
          emit(NULL, true, "LDA","7","1","7", false);
          emit(NULL, true, "LDC","1","1","0", false);
          break;
        case '<': 
          emit_comment(" < ");
          emit(NULL, false,"SUB","1","2","1", false);              
          emit(NULL, true, "JLT","1","2","7", false);
          emit(NULL, true, "LDC","1","0","0", false);
          emit(NULL, true, "LDA","7","1","7", false);
          emit(NULL, true, "LDC","1","1","0", false);
          break;
        case GTE: 
          emit_comment(" >= ");
          emit(NULL, false,"SUB","1","2","1", false);              
          emit(NULL, true, "JGE","1","2","7", false);
          emit(NULL, true, "LDC","1","0","0", false);
          emit(NULL, true, "LDA","7","1","7", false);
          emit(NULL, true, "LDC","1","1","0", false);
          break;
        case LTE:
          emit_comment(" <= ");
          emit(NULL, false,"SUB","1","2","1", false);              
          emit(NULL, true, "JLE","1","2","7", false);
          emit(NULL, true, "LDC","1","0","0", false);
          emit(NULL, true, "LDA","7","1","7", false);
          emit(NULL, true, "LDC","1","1","0", false);
          break; 
        case NEQ:
          emit_comment(" != ");
          emit(NULL, false,"SUB","1","2","1", false);              
          emit(NULL, true, "JNE","1","2","7", false);
          emit(NULL, true, "LDC","1","0","0", false);
          emit(NULL, true, "LDA","7","1","7", false);
          emit(NULL, true, "LDC","1","1","0", false);
          break;
        case CEQ:
          emit_comment(" == ");
          emit(NULL, false,"SUB","1","2","1", false);              
          emit(NULL, true, "JNE","1","2","7", false);
          emit(NULL, true, "LDC","1","0","0", false);
          emit(NULL, true, "LDA","7","1","7", false);
          emit(NULL, true, "LDC","1","1","0", false);
          break;
        default: break;
        }
      }
    else {
      emit_comment("BINOP: get left value (FP)");
      CGen_EXPR(kidl);
      emit_comment("BINOP: store left value on stack (FP)");
      emit(NULL, true, "STF","1","0","6", false);
      emit(NULL, true, "LDA","6","8","6", false);
      emit_comment("BINOP: get right value (FP)");
      CGen_EXPR(kidr);
      emit_comment("BINOP: pop left value from stack to r2 (FP)");
      emit(NULL, true, "LDA","6","-8","6", false);    
      emit(NULL, true, "LDF","2","0","6", false);    
      switch(n_t) {
        case '+':
          emit_comment(" +  (FP)");
          emit(NULL, false, "ADDF","1","2","1", false);    
          break;
        case '-':
           emit_comment(" -  (FP)");
          emit(NULL, false, "SUBF","1","2","1", false);    
          break;
        case '*':
           emit_comment(" *  (FP)");
           emit(NULL, false, "MULF","1","2","1", false);    
           break;
        case '/':
          emit_comment(" /  (FP)");
          emit(NULL, false, "DIVF","1","2","1", false);    
          break;
        case '%':
          emit_comment(" %  (FP w/ conversion)");
          emit(NULL, false, "CVTFI","1","1","0", false);
          emit(NULL, false, "CVTFI","2","2","0", false);
          emit(NULL, false, "DIV","3","2","1", false);    
          emit(NULL, false, "MUL","4","3","2", false);    
          emit(NULL, false, "SUB","1","2","4", false);    
          emit(NULL, false, "CVTIF","1","1","0", false);
          break;
        case '>':
          emit_comment(" >  (FP w/ conversion)");
          emit(NULL, false,"SUBF","1","2","1", false);              
          emit(NULL, false, "CVTFI","1","1","0", false);
          emit(NULL, true, "JGT","1","2","7", false);
          emit(NULL, true, "LDC","1","0","0", false);
          emit(NULL, true, "LDA","7","1","7", false);
          emit(NULL, true, "LDC","1","1","0", false);
          break;
        case '<': 
          emit_comment(" <  (FP w/ conversion)");
          emit(NULL, false,"SUBF","1","2","1", false);              
          emit(NULL, false, "CVTFI","1","1","0", false);
          emit(NULL, true, "JLT","1","2","7", false);
          emit(NULL, true, "LDC","1","0","0", false);
          emit(NULL, true, "LDA","7","1","7", false);
          emit(NULL, true, "LDC","1","1","0", false);
          break;
        case GTE: 
          emit_comment(" >= (FP w/ conversion)");
          emit(NULL, false,"SUBF","1","2","1", false);              
          emit(NULL, false, "CVTFI","1","1","0", false);
          emit(NULL, true, "JGE","1","2","7", false);
          emit(NULL, true, "LDC","1","0","0", false);
          emit(NULL, true, "LDA","7","1","7", false);
          emit(NULL, true, "LDC","1","1","0", false);
          break;
        case LTE:
          emit_comment(" <= (FP w/ conversion)");
          emit(NULL, false,"SUBF","1","2","1", false);              
          emit(NULL, false, "CVTFI","1","1","0", false);
          emit(NULL, true, "JLE","1","2","7", false);
          emit(NULL, true, "LDC","1","0","0", false);
          emit(NULL, true, "LDA","7","1","7", false);
          emit(NULL, true, "LDC","1","1","0", false);
          break; 
        case NEQ:
          emit_comment(" != (FP w/ conversion)");
          emit(NULL, false,"SUBF","1","2","1", false);              
          emit(NULL, false, "CVTFI","1","1","0", false);
          emit(NULL, true, "JNE","1","2","7", false);
          emit(NULL, true, "LDC","1","0","0", false);
          emit(NULL, true, "LDA","7","1","7", false);
          emit(NULL, true, "LDC","1","1","0", false);
          break;
        case CEQ:
          emit_comment(" == (FP w/ conversion)");
          emit(NULL, false,"SUBF","1","2","1", false);              
          emit(NULL, false, "CVTFI","1","1","0", false);
          emit(NULL, true, "JNE","1","2","7", false);
          emit(NULL, true, "LDC","1","0","0", false);
          emit(NULL, true, "LDA","7","1","7", false);
          emit(NULL, true, "LDC","1","1","0", false);
          break;
        default: break;
        }
      }
    }
  if (in_t == out_t) return true;          
  if ((in_t == t_CHAR && out_t == t_INT) || (in_t == t_INT && out_t == t_CHAR)) return true;          
  emit_comment("Coercing output.");
  if ((in_t == t_DOUBLE && out_t == t_INT) || (in_t == t_DOUBLE && out_t == t_CHAR)) {
    emit(NULL, false, "CVTFI",  "2","1","0", false);
    emit(NULL, true, "LDA",  "1","0","2", false);
    return true;
    }
  if ((in_t == t_INT && out_t == t_DOUBLE) || (in_t == t_INT && out_t == t_DOUBLE)) {
    emit(NULL, false, "CVTIF",  "1","1","0", false);
    return true;
    }
  return true;
  }
  
bool CGen_UN(pAstNode stm_n) {
  int n_t  = stm_n->getNType();
  int in_t = stm_n->getDType(true);
  int out_t = stm_n->getDType(false);
  pAstNode kid = stm_n->getChild(0); 
  if (n_t == INCR || n_t == DECR) {
    emit_comment("++ or --: preload address");
    if (kid->getNType() == SUB) CGen_LSADR(kid->getLevel(), kid->getOffset(), kid->getDType(true));
      else if (kid->getNType() == ID) CGen_LVADR(kid->getLevel(), kid->getOffset());
      else { fprintf(stderr, "I am confused."); return false; }
    emit_comment("Push address to stack");
    emit(NULL, true, "ST","4","0","6", false);
    emit(NULL, true, "LDA","6","8","6", false);
    emit_comment("Load value");
    CGen_VAR_REF(kid);
    emit_comment("pop address");
    emit(NULL, true, "LD","4","-8","6", false);
    emit(NULL, true, "LDA","6","-8","6", false);    
    if (in_t == t_DOUBLE) {
      emit_comment("FP (inc/dec)rement");
      emit(NULL, true, "LDFC","2","1","0", false);
      if (n_t == INCR)  emit(NULL, false, "ADDF","1","2","1", false);
      if (n_t == DECR)  emit(NULL, false, "SUBF","1","2","1", false);
      emit(NULL, true, "STF","1","0","4", false);
      }
    else {
      emit_comment("INT (inc/dec)rement");
      emit(NULL, true, "LDC","2","1","0", false);
      if (n_t == INCR)  emit(NULL, false, "ADD","1","2","1", false);
      if (n_t == DECR)  emit(NULL, false, "SUB","1","2","1", false);
      emit(NULL, true, "ST","1","0","4", false);
      }
    }    
  else  {
    emit_comment("Load value for unary op.");
    if (CGen_EXPR(kid) == false) return false;
    if (n_t == UMINUS) {
      if (in_t != t_DOUBLE) {
        emit_comment("UMINUS: and subtract from 0");
        emit(NULL, true, "LDC","3","0","5", false);
        emit(NULL, false,"SUB","1","3","1", false);
        }
      else {
        emit_comment("UMINUS: and subtract from 0 (FP)");
        emit(NULL, true, "LDFC","3","0","5", false);
        emit(NULL, false,"SUBF","1","3","1", false);
        }
      }
    else if (n_t == '!') {
      if (in_t != t_DOUBLE) {
         emit_comment("BANG: (FP) convert to int");
         emit(NULL, false, "CVTFI","1","1","0", false);
        }
      emit_comment("BANG: invert");
      emit(NULL, true, "JEQ","1","2","7", false);
      emit(NULL, true, "LDC","1","0","0", false);
      emit(NULL, true, "LDA","7","1","7", false);
      emit(NULL, true, "LDC","1","1","0", false);
      }
    }  
  if (in_t == out_t) return true;          
  if ((in_t == t_CHAR && out_t == t_INT) || (in_t == t_INT && out_t == t_CHAR)) return true;          
  if ((in_t == t_DOUBLE && out_t == t_INT) || (in_t == t_DOUBLE && out_t == t_CHAR)) {
    emit(NULL, false, "CVTFI",  "2","1","0", false);
    emit(NULL, true, "LDA",  "1","0","2", false);
    return true;
    }
  if ((in_t == t_INT && out_t == t_DOUBLE) || (in_t == t_INT && out_t == t_DOUBLE)) {
    emit(NULL, false, "CVTIF",  "1","1","0", false);
    return true;
    }
  }
  
bool CGen_LVADR(int lv, int off) {
  emit_comment("CGen_LVADR: Load Variable ADDR into REG 4");
  if (lv > current_level) {
    fprintf(stderr, "ACK! Accessing level %d from level %d!", lv, current_level);
    getchar();    
    return false;
    }
  char temps[32];
  sprintf(temps, "LVADR: l %d, o %d",lv,off); 
  emit_comment(temps);
  sprintf(temps, "%d", off);
  if (lv == current_level) {
    emit(NULL, true, "LDA","4",temps,"5", false);
    }
  else if (lv == 0) {
    emit(NULL, true, "LDC","3","0","0", false);
    emit(NULL, true, "LDA","4",temps,"3", false);
    }
  else {
    int tlv = current_level;
    emit(NULL, true, "LDA","3","0","5", false);
    while (tlv > lv) {
      sprintf(temps, "Backing up to l %d",tlv); 
      emit_comment(temps);
      sprintf(temps, "%d", off);
      emit(NULL, true, "LD","3","-4","3", false);
      tlv--;
      }
    emit(NULL, true, "LDA","4",temps,"3", false);
    }
  return true;
  }

bool CGen_LSADR(int lv, int off, int in_t) {
  emit_comment("CGen_LSADR: Load Subscripted ADDR into REG 4");
  if (lv > current_level) {
    fprintf(stderr, "ACK! Accessing level %d from level %d!", lv, current_level);
    getchar();    
    return false;
    }
  char temps[80];
  sprintf(temps, "LSADR: l %d, o %d, t %s, sub in r1",lv,off,dt_text[in_t]); 
  emit_comment(temps);
  sprintf(temps, "%d", off);
  if (lv == current_level) {
    emit(NULL, true, "LDA","4",temps,"5", false);
    }
  else if (lv == 0) {
    emit(NULL, true, "LDC","3","0","0", false);
    emit(NULL, true, "LDA","4",temps,"3", false);
    }
  else {
    int tlv = current_level;
    emit(NULL, true, "LDA","3","0","5", false);
    while (tlv > lv) {
      sprintf(temps, "Backing up to l %d",tlv); 
      emit_comment(temps);
      sprintf(temps, "%d", off);
      emit(NULL, true, "LD","3","-4","3", false);
      tlv--;
      }
    emit(NULL, true, "LDA","4",temps,"3", false);
    }
  emit(NULL, true, "LDA", "3","0","1", false);
  if (in_t == t_DOUBLE)    emit(NULL, true, "LDC", "1","8","0", false);
  else if (in_t == t_CHAR) emit(NULL, true, "LDC", "1","1","0", false);
  else                     emit(NULL, true, "LDC", "1","4","0", false); 
  emit(NULL, false, "MUL", "2","1","3", false);
  emit(NULL, false, "ADD", "1","2","4", false);
  emit(NULL, true,  "LDA", "4","0","1", false);
  return true;
  }
  
bool CGen_VAR_REF(pAstNode stm_n) {
  int out_t = stm_n->getDType(false);
  int in_t = stm_n->getDType(false);
  CGen_LVADR(stm_n->getLevel(), stm_n->getOffset());
  if (stm_n->getNType() == ID) {
    const char *name=getName(stm_n->getID());
    emit_comment(name);
    CGen_LVADR(stm_n->getLevel(), stm_n->getOffset());
    if (in_t == t_DOUBLE)    emit(NULL, true, "LDF","1","0","4", false);
    else if (in_t == t_CHAR) emit(NULL, true, "LDB","1","0","4", false);
    else                     emit(NULL, true, "LD", "1","0","4", false);
    }
  else if (out_t == SUB) {
    pAstNode kid = stm_n->getChild(0);
    const char *name=getName(stm_n->getID());
    emit_comment(name);
    emit_comment("Subscript:");
    if (CGen_EXPR(kid) == false) return false;    
    CGen_LSADR(stm_n->getLevel(), stm_n->getOffset(), stm_n->getDType(true));
    if (in_t == t_DOUBLE)    emit(NULL, true, "LDB", "1","0","4", false);
    else if (in_t == t_CHAR) emit(NULL, true, "LDF", "1","0","4", false);
    else                     emit(NULL, true, "LD",  "1","0","4", false); 
    }
  if (in_t == out_t) return true;          
  if ((in_t == t_CHAR && out_t == t_INT) || (in_t == t_INT && out_t == t_CHAR)) return true;          
  if ((in_t == t_DOUBLE && out_t == t_INT) || (in_t == t_DOUBLE && out_t == t_CHAR)) {
    emit(NULL, false, "CVTFI",  "2","1","0", false);
    emit(NULL, true, "LDA",  "1","0","2", false);
    return true;
    }
  if ((in_t == t_INT && out_t == t_DOUBLE) || (in_t == t_INT && out_t == t_DOUBLE)) {
    emit(NULL, false, "CVTIF",  "1","1","0", false);
    return true;
    }
  }  

bool CGen_CONST(pAstNode stm_n) {
  emit_comment("CONSTANT");
  int c_type = stm_n->getNType();
  char temps[64];
  if (c_type == NUM || c_type == CNUM) {
    sprintf(temps, "INT Const: %d", stm_n->getNVal());
    emit_comment(temps);
    sprintf(temps, "%d", stm_n->getNVal());
    emit(NULL, true, "LDC","1",temps,"0", false);
    }  
  else {
    sprintf(temps, "FLOAT Const: %f", stm_n->getFVal());
    emit_comment(temps);
    sprintf(temps, "%f", stm_n->getFVal());
    emit(NULL, true, "LDFC","1",temps,"0", false);
    }              
  return true;
  }
