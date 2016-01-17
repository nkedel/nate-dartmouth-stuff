// label.h
// very loosely inspired Cormen's symtab.h, via idTab.h

#ifndef LABEL_H_
#define LABEL_H_
#include <stdlib.h>

#define HASH_SZ 32
// hash size of 32, since ASCII blocks aligned on 32, and we're mostly
//                  worrying about alpha values...

#ifndef LABEL_CC
extern int current_line;
extern void dump_asm(void);
extern char *tlName(int tln);
extern int gentl(void);
extern void pushtl(int b, int e);
extern int toptl(bool ise);
extern int poptl(void);
#endif 
 
typedef struct sLabel {
  struct sLabel *next;
  char   *name;
  int    id;
  int    on_line;
  } Label;

typedef Label *pLabel;

typedef struct sInst {
  bool RM;
  bool isc;
  char *comment;
  int  address;
  pLabel label;
  bool needlp;
  char *instr;
  char *val1;
  char *val2;
  char *val3;
  struct sInst *next;
  } Inst;

typedef Inst *pInst;

#ifndef LABEL_CC
extern pInst outprog;
extern int emit(const char *lab, bool srm, const char *ins, const char *v1, const char *v2, const char *v3, bool np);
extern int emit_comment(const char *com);
#endif



typedef struct slabTab {
  int n_labs;
  pLabel hash[HASH_SZ];
  } tLabTab;

#ifdef LABEL_CC
void InitLabTab(void);
int FindLab(const char *s);
pLabel InsertLab(const char *s, int ln);
void dumpLTable(void);
#endif

#ifndef LabelCC_C
extern void InitLabTab(void);
extern int FindLab(const char *s);
extern pLabel InsertLab(const char *s, int ln);
extern void dumpLTable(void);
#endif
#endif
