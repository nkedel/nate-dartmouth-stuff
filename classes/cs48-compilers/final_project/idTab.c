// idTab.c
// very loosely inspired Cormen's symtab.h

#ifndef IDTAB_C
#define IDTAB_C

#include <stdlib.h>
#include "idTab.h"

// THIS IS IMPLEMENTED IN C, NOT C++ AS IT MUST BE ACCESSED FROM THE LEXER
// uses a trivial hash -- should probably use a more efficient one, but as
//                        it stands, this should be good enough.

static tIdTab IdTab;

void InitIdTab(void) {
  int i;
  IdTab.n_ids = 0;
  for (i=0; i<HASH_SZ; i++) IdTab.hash[i] = NULL;
  }
  
pListNode FindInsert(const char *s) {
  int hval;
  pListNode tNode, pNode;
  hval = s[0] % HASH_SZ;
  pNode = NULL;
  tNode = IdTab.hash[hval];
  while (tNode != NULL) { 
    pNode = tNode;
    if (strcmp(s, tNode->name) == 0) break;
    tNode = tNode->next;
    }
  if (tNode != NULL) return tNode;
  tNode = (pListNode) malloc(sizeof(listNode));
  tNode->id = IdTab.n_ids;
  IdTab.n_ids++;
  if (pNode == NULL) IdTab.hash[hval] = tNode;
  else pNode->next = tNode;
  strcpy(tNode->name, s);
  return tNode;
  }

const char *getName(pListNode Node) {
  if (Node == NULL) return NULL;
  return (const char*) Node->name;
  }

int getNId (pListNode Node) {
  if (Node == NULL) return -1;
  return Node->id;
  }

void dumpTable(void) { 
  int i;
  pListNode tNode;
  printf("* idTable:\n");
  for (i = 0; i < HASH_SZ; i++) {
    tNode = IdTab.hash[i];
    while(tNode!=NULL) {
      printf("* h%d i%d: %s\n", i, tNode->id, tNode->name);
      tNode = tNode->next;
      }
    }
  printf("\n");
  }
  
#endif
 
