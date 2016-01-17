// symTab.cc
// expanded from Cormen's Symtab.cc
// n. edel

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "symTab.h"

// Default SymNode constructor.
// SymNode constructor, given a string
SymNode::SymNode(const char *s = NULL, int s_type = -1, bool s_fn = false, bool s_defn = false) {
  if (s == NULL)  name = NULL;
    else          name = strdup(s);
  next  = NULL;
  type  = s_type;
  is_fn = s_fn;
  is_defn = s_defn; 
  is_array = false;
  dimen   = -1;
  n_param = 0;
  param_t[0] = -1;
  }

// SymNode destructor.
SymNode::~SymNode() {
  if (name != NULL)
    delete [] name;
  }

// Add a name to a SymNode.
void SymNode::setName(const char *s) {
  if (name != NULL)
    delete [] name;
  name = strdup(s);
  }


// Return true iff this SymNode has same name as s.
bool SymNode::isEqual(const char *s) {
  return (strcmp(name, s) == 0);
  }


// Get the level of a SymNode.
int SymNode::getLevel() {
  return level;
  }

int SymNode::getType() {
  return type;
  }
  
bool SymNode::isFunc() {
  return is_fn;
  }

bool SymNode::isArray() {
  return is_array;
  }
  
bool SymNode::beenDefn() {
  return is_defn;
  }

void SymNode::setType(int s_type) {
  type = s_type;
  }
  
void SymNode::setFn() {
  is_fn = true;
  }

void SymNode::resetFn() {
  is_fn = false;
  }

void SymNode::setDefn() {
  is_defn = true;
  }

void SymNode::resetDefn() {
  is_defn = false;
  }

void SymNode::setArray() {
  is_array = true;
  }

void SymNode::resetArray() {
  is_array = false;
  }

void SymNode::setDim(int s_dim) {
  dimen = s_dim;
  }

int SymNode::getDim() {
  return dimen;
  }

// Constructor for a SymHashTable.
SymHashTable::SymHashTable()
{
  for (int i = 0; i < HASHSIZE; i++)
    table[i] = NULL;
  data_sz = 0;
}


// Destructor for a SymHashTable.
SymHashTable::~SymHashTable() {
  for (int i = 0; i < HASHSIZE; i++) {
    SymNode *node, *next;

    for (node = table[i]; node != NULL; node = next) {
  	  next = node->next;
	    delete node;
	    }  
    }
  }


// Peter Weinberger's hash function, from Aho, Sethi, & Ullman p. 436.
static int HashPJW(const char *s) {
  unsigned h = 0, g;
  for (char *p = (char *) s; *p != '\0'; p++) {
      h = (h << 4) + *p;
      if ((g = (h & 0xf0000000)) != 0)  h ^= (g >> 24) ^ g;
      }
  return h % HASHSIZE;
  }

// Insert a new entry.  First make sure it's not already there.
// Return a pointer to the entry.
SymNode *SymHashTable::Insert(const char *s = NULL, int s_type = -1, bool s_fn = false, bool s_defn = false) {
  int slot = HashPJW(s);

  SymNode *node = Lookup(s, slot);
  if (node == NULL) {
    node = new SymNode(s, s_type, s_fn, s_defn);
    node->level = level;
    node->next = table[slot];
    table[slot] = node;
    }

  return node;
  }


// Lookup a hash table entry.  Return a pointer to it if found, NULL
// otherwise.  slot is where to look; if slot == NOHASHSLOT, then have
// to apply the hash function to figure it out.
SymNode *SymHashTable::Lookup(const char *s, int slot) {
  SymNode *node;

  if (slot == NOHASHSLOT) slot = HashPJW(s);

  for (node = table[slot];
       node != NULL && !node->isEqual(s);
       node = node->next)
    ;
  return node;
  }


// Construct a symbol table.  It points to an empty SymHashTable.
SymbolTable::SymbolTable() {
  innerScope = new SymHashTable;
  innerScope->outerScope = NULL;
  innerScope->level = 0;
  }


// Destructor for a symbol table.  Destroys each SymHashTable.
SymbolTable::~SymbolTable() {
  SymHashTable *t, *outer;

  for (t = innerScope; t != NULL; t = outer) {
    outer = t->outerScope;
    delete t;
    }
  }


// Insert an entry into the innermost scope of symbol table.  First
// make sure it's not already in that scope.  Return a pointer to the
// entry.
SymNode *SymbolTable::Insert(const char *s = NULL, int s_type = -1, bool s_fn = false, bool s_defn = false) {
  if (innerScope == NULL) {
    fprintf(stderr, "Error: inserting into an empty symbol table\n");
    exit(1);
    }

  SymNode *node = innerScope->Lookup(s);

  if (node == NULL)
    node = innerScope->Insert(s, s_type, s_fn, s_defn);
  else return NULL;

  return node;
  }


// Lookup an entry in a symbol table.  Return a pointer to it if
// found, NULL otherwise.
SymNode *SymbolTable::Lookup(const char *s) {
  SymNode *node;
  SymHashTable *t;

  for (node = NULL, t = innerScope;
       node == NULL && t != NULL;
       t = t->outerScope)
    node = t->Lookup(s);

  return node;
  }



int SymbolTable::getDataSz() {
  return innerScope->data_sz;
  }

int SymbolTable::addVar(int bytes) {
  int t = innerScope->data_sz;
  int s = bytes;
  if ((s % 8) != 0) s += (8 - (s % 8)); // pad to 8 byte boundary
  innerScope->data_sz += s;
  return t;
  }

// Enter a new scope.
void SymbolTable::EnterScope() {
  SymHashTable *t = new SymHashTable;
  t->outerScope = innerScope;
  t->level = innerScope->level + 1;
  innerScope = t;
  }

int SymbolTable::getLevel() {
  return innerScope->level;
  }


// Leave a scope.
void SymbolTable::LeaveScope() {
  SymHashTable *t = innerScope;
  innerScope = t->outerScope;
  delete t;
  }

int SymNode::getOffset() {
  return offset;
  }

void SymNode::setOffset(int s_off) {
  offset = s_off;
  }
int SymNode::getSize() {
  return size;
  }

void SymNode::setSize(int s_sz) {
  size = s_sz;
  }

int SymNode::getNParam() {
  return n_param;
  }
    
int SymNode::getParamT(int which) {
  if (which < n_param) return param_t[which];
  return -1;
  }

void SymNode::addParam(int p_t) {
  if (n_param < MAX_PARAMS) {
    param_t[n_param] = p_t;
    n_param++;
    }
  else { 
    fprintf(stderr, "Max # params exceeded! Crashing!\n\n");
    exit(1);
    }
  }
