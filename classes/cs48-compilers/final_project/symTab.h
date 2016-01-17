// symTab.h
// expanded from Cormen's Symtab.h
// n. edel

#ifndef SYMTAB_H_
#define SYMTAB_H_

// Node in a linked list within the symbol table.

const int HASHSIZE = 211;
const int NOHASHSLOT = -1;
const int MAX_PARAMS = 32; // 32 params max per function seems like enough -- may in fact be overkill.

class SymNode {
public:
  SymNode(const char *s = NULL, int s_type = -1, bool s_fn = false, bool s_defn = false);
                            // constructor with string
  ~SymNode();               // destructor
  void setName(const char *s);    // set the name
  bool isEqual(const char *s);    // does this SymNode have same name as s?
  int getLevel();           // get the scope level
  int getType();            // get the type
  bool isFunc();            // is this a function?
  bool beenDefn();          // has it been defined?
  bool isArray();           // is it an array?
  void setType(int s_type); // set the type
  void setFn();             // set is_fn to true
  void resetFn();           // set is_fn to false
  void setDefn();           // set is_defn to true
  void resetDefn();         // set is_defn to false
  void setArray();          // set is_array to true
  void resetArray();        // set is_array to false
  void setDim(int s_dim);   // set array dimension
  int  getDim();            // get array dimension
  int  getNParam();         // get number of parameters
  int  getParamT(int which);// get a parameter type
  void addParam(int p_t);   // add a parameter
  int  getOffset();         // get offset
  void setOffset(int s_off);// set offset
  int  getSize();           // get size
  void setSize(int s_sz);   // set size

  friend class SymHashTable;

private:
  char *name;     // name in this SymNode
  int level;      // scope level, same for all in this hash table
  SymNode *next;  // next SymNode in list
  int type;       // type of the var/fn
  bool is_fn;     // true if fn, false if var
  bool is_defn;   // true if defined, false if only declared
  bool is_array;  // true if array
  int  dimen;     // dimension of array
  int  n_param;   // number of parameters
  int  param_t[MAX_PARAMS];  // type of each parameter 
  int  offset;    // offset of a var
  int  size;
};


// A hash table for a given scope in a symbol table.

class SymHashTable {
public:
  SymHashTable();     // constructor, makes an empty table
  ~SymHashTable();    // destructor
  SymNode *Insert(const char *s = NULL, int s_type = -1, bool s_fn = false, bool s_decl = false);  
                      // insert a new entry
  SymNode *Lookup(const char *s, int slot = NOHASHSLOT); 
                      // lookup an entry

  friend class SymbolTable;

private:
  SymNode *table[HASHSIZE];  // hash table
  SymHashTable *outerScope;  // ptr to SymHashTable in next outer scope
  int level;      // level of scope, 0 is outermost
  int data_sz;    // size of data associated w/ scope  
};


// A symbol table.

class SymbolTable {
public:
  SymbolTable();            // constructor, makes an empty table
  ~SymbolTable();           // destructor
  SymNode *Insert(const char *s, int s_type, bool s_fn = false, bool s_decl = false);  
                            // insert a new entry
  SymNode *Lookup(const char *s); // lookup an entry
  void EnterScope();        // enter a new scope
  void LeaveScope();        // leave a scope
  int  getDataSz();         // # bytes of vars in inner scope
  int  addVar(int bytes);   // add a variable to inner scope
  int  getLevel();          // get the level of the inner scope
private:
  SymHashTable *innerScope;  // ptr to SymHashTable of innermost scope
};

#endif
