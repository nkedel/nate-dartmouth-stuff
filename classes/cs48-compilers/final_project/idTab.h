// idTab.h
// very loosely inspired Cormen's symtab.h

#ifndef IDTAB_H_
#define IDTAB_H_
#include <stdlib.h>

#define HASH_SZ 32
// hash size of 32, since ASCII blocks aligned on 32, and we're mostly
//                  worrying about alpha values...

typedef struct sListNode {
  struct sListNode *next;
  char   name[127];
  int    id;
  } listNode;

typedef listNode *pListNode;

typedef struct sIdTab {
  int n_ids;
  pListNode hash[HASH_SZ];
  } tIdTab;

#ifdef IDTAB_C
const char *getName(pListNode Node);
void InitIdTab(void);
pListNode FindInsert(const char *s);
void dumpTable(void);
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef IDTAB_C
extern const char *getName(pListNode Node);
extern void InitIdTab(void);
extern pListNode FindInsert(const char *s);
extern void dumpTable(void);
extern tIdTab IdTab;
#endif

#ifdef __cplusplus
}
#endif

#endif
