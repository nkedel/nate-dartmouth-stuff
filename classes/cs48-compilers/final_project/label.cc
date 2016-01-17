// label.cc
// very loosely inspired Cormen's symtab.h

#define LABEL_CC

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "label.h"

int current_line;
pInst outprog = NULL;
pInst lastins = NULL;

void dump_asm(void) {
  pInst foo = outprog;
  while(foo!=NULL) {
    int i;
    if (foo->isc != false) printf("* %s\n",foo->comment); 
    else {
      if (foo->label != NULL) printf("* (%s)\n",foo->label->name);
      if (foo->needlp && (i = FindLab(foo->val2)) != -1) {
        if (foo->RM) printf("%d: %s %s,%d(%s) * (%s)\n",foo->address,foo->instr,foo->val1,i,foo->val3,foo->val2);    
              else printf("%d: %s %s,%d,%s  * (%s)\n",foo->address,foo->instr,foo->val1,i,foo->val3,foo->val2);
        }
      else {
        if (foo->RM) printf("%d: %s %s,%s(%s)\n",foo->address,foo->instr,foo->val1,foo->val2,foo->val3);    
              else printf("%d: %s %s,%s,%s\n",foo->address,foo->instr,foo->val1,foo->val2,foo->val3);
        }
      }
    foo = foo->next;
    }
  }

int emit(const char *lab, const bool srm, const char *ins, const char *v1, const char *v2, const char *v3, bool np) {
  pInst temp = new Inst;
  if (lab == NULL) temp->label = NULL;
  else             {
    if (FindLab(lab) != -1) { fprintf(stderr,"Dupe label: %s, l: %d\n\n",lab,current_line); getchar(); exit(0); }
    temp->label = InsertLab(lab, current_line);
    }
  temp->address = current_line;
  temp->RM = srm;
  if (ins == NULL) {fprintf(stderr, "ACK!"); getchar(); }
  else             temp->instr = strdup(ins);
  if (v1 == NULL)  {fprintf(stderr, "ACK1!"); getchar(); }
  else             temp->val1 = strdup(v1);
  if (v2 == NULL)  {fprintf(stderr, "ACK2!"); getchar(); }
  else             temp->val2 = strdup(v2);
  if (v3 == NULL)  {fprintf(stderr, "ACK3!"); getchar(); }
  else             temp->val3 = strdup(v3);
  temp->needlp = np;
  temp->comment= NULL;
  temp->isc    = false;
  temp->next = NULL;
  if (outprog == NULL) {
    outprog=temp;
    lastins=temp;
    }
  else {
    lastins->next = temp;
    lastins = temp;
    }
  current_line++;
  }

int emit_comment(const char *com) {
  pInst temp = new Inst;
  temp->isc = true;
  temp->next=NULL;
  temp->comment = strdup(com);
  if (outprog == NULL) {
    outprog=temp;
    lastins=temp;
    }
  else {
    lastins->next = temp;
    lastins = temp;
    }
  }

static tLabTab LabTab;

void InitIdTab(void) {
  int i;
  LabTab.n_labs = 0;
  for (i=0; i<HASH_SZ; i++) LabTab.hash[i] = NULL;
  }
  
pLabel InsertLab(const char *s, int ln) {
  int hval;
  pLabel tNode, pNode;
  hval = s[1] % HASH_SZ;
  pNode = NULL;
  tNode = LabTab.hash[hval];
  while (tNode != NULL) { 
    pNode = tNode;
    if (strcmp(s, tNode->name) == 0) break;
    tNode = tNode->next;
    }
  if (tNode != NULL) return NULL;
  tNode = new Label;
  tNode->id = LabTab.n_labs;
  LabTab.n_labs++;
  if (pNode == NULL) LabTab.hash[hval] = tNode;
  else pNode->next = tNode;
  tNode->name=strdup(s);
  tNode->on_line = ln;
  return tNode;
  }

int FindLab(const char *s) {
  int hval;
  pLabel tNode, pNode;
  hval = s[1] % HASH_SZ;
  pNode = NULL;
  tNode = LabTab.hash[hval];
  while (tNode != NULL) { 
    pNode = tNode;
    if (strcmp(s, tNode->name) == 0) break;
    tNode = tNode->next;
    }
  if (tNode == NULL) return -1;
  return tNode->on_line;
  }

void dumpLTable(void) { 
  int i;
  pLabel tNode;
  printf("* Label Table:\n");
  for (i = 0; i < HASH_SZ; i++) {
    tNode = LabTab.hash[i];
    while(tNode!=NULL) {
      printf("* h%d i%d: %s ln:%d\n", i, tNode->id, tNode->name, tNode->on_line);
      tNode = tNode->next;
      }
    }
  printf("\n");
  }
  
char *tlName(int tln) {
   char *tlnm = new char[16];
   sprintf(tlnm, "_$%06d", tln);
   return tlnm;
   }

int lasttl = 0;

int gentl(void) {
  return lasttl++;
  }
  
typedef struct stls {
  int beg;
  int end;
  struct stls *prev;
  } ttls;

ttls *tls = NULL; 

void pushtl(int b, int e) {
  ttls *temp = new ttls;
  temp->beg = b;
  temp->end = e;
  temp->prev = tls;
  tls = temp;
  }

int toptl(bool ise) {
  if (tls == NULL) return -1;
  if (ise) return (tls->end);
  return (tls->beg);
  }

int poptl(void) {
  if (tls != NULL) tls = tls->prev;
  return 0;
  }
